#pragma once

using stdx::mem::Pointers;
using stdx::mem::UniquePointer;
using stdx::sync::Atomic;
using stdx::time::TimeSpecification;
using stdx::thread::Thread;
using stdx::os::linux::alsa::SoundControl;
using stdx::os::linux::alsa::SoundPcm;
using stdx::os::linux::alsa::SoundPcmAccess;
using stdx::os::linux::alsa::SoundPcmFormat;
using stdx::os::linux::alsa::SoundPcmInfo;
using stdx::os::linux::alsa::SoundPcmHwParams;
using stdx::os::linux::alsa::SoundPcmSFrames;
using stdx::os::linux::alsa::SoundPcmStream;
using stdx::os::linux::alsa::SoundPcmSwParams;
using stdx::os::linux::alsa::SoundPcmUFrames;

using namespace stdx::os;

namespace stdx::audio::sampled {
    [[nodiscard]]
    inline SoundPcmFormat to_alsa_format(SampleFormat fmt) noexcept {
        switch (fmt) {
            case SampleFormat::FLOAT:
            case SampleFormat::FLOAT_PLANAR:
                return SoundPcmFormat::FLOAT_LE;
            case SampleFormat::SHORT:
                return SoundPcmFormat::S16_LE;
            case SampleFormat::MEDIUM:
                return SoundPcmFormat::S24_3LE;
            case SampleFormat::INT:
                return SoundPcmFormat::S32_LE;
            default:
                Ops::unreachable();
        }
        Ops::unreachable();
    }

    [[nodiscard]]
    inline u64 monotonic_ns() noexcept {
        TimeSpecification ts{};
        unix::clock_gettime(unix::CLOCK_MONOTONIC, &ts);
        return static_cast<u64>(ts.tv_sec) * 1'000'000'000ull
            + static_cast<u64>(ts.tv_nsec);
    }

    /**
     * @brief Configures an open snd_pcm_t for the requested format and
     * commits hw/sw params. On success, @p actual_rate and the
     * chosen period/buffer sizes are filled in.
     */
    inline void configure_pcm(
        SoundPcm* pcm,
        AudioFormat& fmt,
        SoundPcmUFrames& period_frames,
        SoundPcmUFrames& buffer_frames
    ) throws (UnsupportedAudioFormatException) {
        SoundPcmHwParams* hw = nullptr;
        linux::alsa::snd_pcm_hw_params_malloc(&hw);

        try {
            i32 r = linux::alsa::snd_pcm_hw_params_any(pcm, hw);
            if (r < 0) {
                throw UnsupportedAudioFormatException(linux::alsa::snd_strerror(r));
            }

            r = linux::alsa::snd_pcm_hw_params_set_access(pcm, hw, SoundPcmAccess::RW_INTERLEAVED);
            if (r < 0) {
                throw UnsupportedAudioFormatException(linux::alsa::snd_strerror(r));
            }

            r = linux::alsa::snd_pcm_hw_params_set_format(pcm, hw, to_alsa_format(fmt.format));
            if (r < 0) {
                throw UnsupportedAudioFormatException(linux::alsa::snd_strerror(r));
            }

            u32 rate = fmt.sample_rate;
            r = linux::alsa::snd_pcm_hw_params_set_rate_near(pcm, hw, &rate, nullptr);
            if (r < 0) {
                throw UnsupportedAudioFormatException(linux::alsa::snd_strerror(r));
            }

            u32 channels = fmt.channels;
            r = linux::alsa::snd_pcm_hw_params_set_channels_near(pcm, hw, &channels);
            if (r < 0) {
                throw UnsupportedAudioFormatException(linux::alsa::snd_strerror(r));
            }

            period_frames = 1024;
            r = linux::alsa::snd_pcm_hw_params_set_period_size_near(pcm, hw, &period_frames, nullptr);
            if (r < 0) {
                throw UnsupportedAudioFormatException(linux::alsa::snd_strerror(r));
            }

            buffer_frames = 4096;
            r = linux::alsa::snd_pcm_hw_params_set_buffer_size_near(pcm, hw, &buffer_frames);
            if (r < 0) {
                throw UnsupportedAudioFormatException(linux::alsa::snd_strerror(r));
            }

            r = linux::alsa::snd_pcm_hw_params(pcm, hw);
            if (r < 0) {
                throw UnsupportedAudioFormatException(linux::alsa::snd_strerror(r));
            }

            fmt.sample_rate = rate;
            fmt.channels = static_cast<u16>(channels);
        } catch (...) {
            linux::alsa::snd_pcm_hw_params_free(hw);
            throw;
        }
        linux::alsa::snd_pcm_hw_params_free(hw);

        SoundPcmSwParams* sw = nullptr;
        linux::alsa::snd_pcm_sw_params_malloc(&sw);
        try {
            i32 r = linux::alsa::snd_pcm_sw_params_current(pcm, sw);
            if (r < 0) {
                throw UnsupportedAudioFormatException(linux::alsa::snd_strerror(r));
            }
            r = linux::alsa::snd_pcm_sw_params_set_start_threshold(pcm, sw, period_frames);
            if (r < 0) {
                throw UnsupportedAudioFormatException(linux::alsa::snd_strerror(r));
            }
            r = linux::alsa::snd_pcm_sw_params_set_avail_min(pcm, sw, period_frames);
            if (r < 0) {
                throw UnsupportedAudioFormatException(linux::alsa::snd_strerror(r));
            }
            r = linux::alsa::snd_pcm_sw_params(pcm, sw);
            if (r < 0) {
                throw UnsupportedAudioFormatException(linux::alsa::snd_strerror(r));
            }
        } catch (...) {
            linux::alsa::snd_pcm_sw_params_free(sw);
            throw;
        }
        linux::alsa::snd_pcm_sw_params_free(sw);
    }

    /**
     * @class AlsaOutputLine
     * @brief Concrete OutputLine that writes interleaved f32 frames to
     * an open snd_pcm_t playback handle.
     * @extends OutputLine
     */
    class AlsaOutputLine final: public OutputLine {
    private:
        SoundPcm* pcm;
        AudioFormat fmt;
        u32 period;
        u32 buffer;
        RenderCallback callback;
        Vector<f32> mix;
        Thread worker;
        Atomic<bool> running{false};
        Atomic<u64> underruns{0};

        void render_loop() noexcept {
            u64 frames_done = 0;
            while (running.load()) {
                const AudioTime t{frames_done, monotonic_ns()};
                callback(Span<f32>(mix.data(), mix.size()), t);
                SoundPcmSFrames r = linux::alsa::snd_pcm_writei(pcm, mix.data(), period);
                if (r < 0) {
                    underruns.fetch_add(1);
                    if (linux::alsa::snd_pcm_recover(pcm, static_cast<i32>(r), 1) < 0) {
                        running.store(false);
                        break;
                    }
                } else {
                    frames_done += static_cast<u64>(r);
                }
            }
        }
    public:
        AlsaOutputLine(
            SoundPcm* pcm,
            AudioFormat fmt,
            u32 period_frames,
            u32 buffer_frames,
            RenderCallback cb
        ):
            pcm{pcm}, fmt{fmt}, period{period_frames}, buffer{buffer_frames},
            callback{Ops::move(cb)} {
            mix.resize(static_cast<usize>(period) * fmt.channels);
        }

        ~AlsaOutputLine() override {
            close();
        }

        void start() throws (LineUnavailableException) override {
            if (running.load()) {
                return;
            }
            if (!pcm) {
                throw LineUnavailableException("line closed");
            }
            if (i32 r = linux::alsa::snd_pcm_prepare(pcm); r < 0) {
                throw LineUnavailableException(linux::alsa::snd_strerror(r));
            }
            running.store(true);
            worker = Thread([this] -> void { render_loop(); });
        }

        void stop() throws (LineUnavailableException) override {
            if (!running.exchange(false)) {
                return;
            }
            if (worker.joinable()) {
                worker.join();
            }
            if (pcm) {
                linux::alsa::snd_pcm_drain(pcm);
            }
        }

        void close() noexcept override {
            running.store(false);
            if (worker.joinable()) {
                worker.join();
            }
            if (pcm) {
                linux::alsa::snd_pcm_close(pcm);
                pcm = nullptr;
            }
        }

        [[nodiscard]]
        const AudioFormat& format() const noexcept override {
            return fmt;
        }

        [[nodiscard]]
        u32 latency_frames() const noexcept override {
            return buffer;
        }
        [[nodiscard]]
        u64 underrun_count() const noexcept override {
            return underruns.load();
        }

        [[nodiscard]]
        bool is_running() const noexcept override {
            return running.load();
        }
    };

    /**
     * @class AlsaInputLine
     * @brief Mirror of AlsaOutputLine for capture devices.
     * @extends InputLine
     */
    class AlsaInputLine final: public InputLine {
    private:
        SoundPcm* pcm;
        AudioFormat fmt;
        u32 period;
        u32 buffer;
        CaptureCallback callback;
        Vector<f32> mix;
        Thread worker;
        Atomic<bool> running{false};
        Atomic<u64> overruns{0};

        void capture_loop() noexcept {
            u64 frames_done = 0;
            while (running.load()) {
                SoundPcmSFrames r = linux::alsa::snd_pcm_readi(pcm, mix.data(), period);
                if (r < 0) {
                    overruns.fetch_add(1);
                    if (linux::alsa::snd_pcm_recover(pcm, static_cast<i32>(r), 1) < 0) {
                        running.store(false);
                        break;
                    }
                    continue;
                }
                frames_done += static_cast<u64>(r);
                const AudioTime t{frames_done, monotonic_ns()};
                const usize samples = static_cast<usize>(r) * fmt.channels;
                callback(Span<const f32>(mix.data(), samples), t);
            }
        }
    public:
        AlsaInputLine(
            SoundPcm* pcm,
            AudioFormat fmt,
            u32 period_frames,
            u32 buffer_frames,
            CaptureCallback cb
        ):
            pcm{pcm}, fmt{fmt}, period{period_frames}, buffer{buffer_frames},
            callback{Ops::move(cb)} {
            mix.resize(static_cast<usize>(period) * fmt.channels);
        }

        ~AlsaInputLine() override {
            close();
        }

        void start() throws (LineUnavailableException) override {
            if (running.load()) {
                return;
            }
            if (!pcm) {
                throw LineUnavailableException("line closed");
            }
            if (i32 r = linux::alsa::snd_pcm_prepare(pcm); r < 0) {
                throw LineUnavailableException(linux::alsa::snd_strerror(r));
            }
            if (i32 r = linux::alsa::snd_pcm_start(pcm); r < 0) {
                throw LineUnavailableException(linux::alsa::snd_strerror(r));
            }
            running.store(true);
            worker = Thread([this] -> void { capture_loop(); });
        }

        void stop() throws (LineUnavailableException) override {
            if (!running.exchange(false)) {
                return;
            }
            if (worker.joinable()) {
                worker.join();
            }
            if (pcm) {
                linux::alsa::snd_pcm_drop(pcm);
            }
        }

        void close() noexcept override {
            running.store(false);
            if (worker.joinable()) {
                worker.join();
            }
            if (pcm) {
                linux::alsa::snd_pcm_close(pcm);
                pcm = nullptr;
            }
        }

        [[nodiscard]]
        const AudioFormat& format() const noexcept override {
            return fmt;
        }

        [[nodiscard]]
        u32 latency_frames() const noexcept override {
            return buffer;
        }

        [[nodiscard]]
        u64 overrun_count() const noexcept override {
            return overruns.load();
        }

        [[nodiscard]]
        bool is_running() const noexcept override {
            return running.load();
        }
    };

    inline Vector<DeviceInfo> enumerate_alsa(StreamDirection dir) {
        Vector<DeviceInfo> out;

        // Synthetic "default" entry so users can always open something
        // without picking a card. Maps to the ALSA "default" PCM.
        out.emplace_back(
            "default",
            (dir == StreamDirection::OUTPUT) ? "Default output" : "Default input",
            dir,
            AudioBackend::ALSA,
            true
        );

        const SoundPcmStream stream =
            (dir == StreamDirection::OUTPUT)
                ? SoundPcmStream::PLAYBACK
                : SoundPcmStream::CAPTURE;

        i32 card = -1;
        while (linux::alsa::snd_card_next(&card) >= 0 && card >= 0) {
            char hw[32];
            stdx::io::cstdio::snprintf(hw, sizeof(hw), "hw:%d", card);

            SoundControl* ctl = nullptr;
            if (linux::alsa::snd_ctl_open(&ctl, hw, 0) < 0) {
                continue;
            }

            i32 dev = -1;
            while (linux::alsa::snd_ctl_pcm_next_device(ctl, &dev) >= 0 && dev >= 0) {
                SoundPcmInfo* info = nullptr;
                linux::alsa::snd_pcm_info_malloc(&info);
                linux::alsa::snd_pcm_info_set_device(info, static_cast<u32>(dev));
                linux::alsa::snd_pcm_info_set_subdevice(info, 0);
                linux::alsa::snd_pcm_info_set_stream(info, stream);

                if (linux::alsa::snd_ctl_pcm_info(ctl, info) < 0) {
                    linux::alsa::snd_pcm_info_free(info);
                    continue;
                }

                char id_buf[64];
                stdx::io::cstdio::snprintf(id_buf, sizeof(id_buf), "hw:%d,%d", card, dev);

                out.emplace_back(
                    id_buf,
                    linux::alsa::snd_pcm_info_get_name(info),
                    dir,
                    AudioBackend::ALSA,
                    false
                );
                linux::alsa::snd_pcm_info_free(info);
            }
            linux::alsa::snd_ctl_close(ctl);
        }
        return out;
    }

    /**
     * @brief Decide which libasound PCM name to use for "the default device."
     *
     * Resolution order, evaluated once and memoized via function-local-static:
     *   1. STDLIBX_AUDIO_DEFAULT_PCM env var if set - opt-out hatch.
     *   2. "pulse" if libasound's PulseAudio shim can open it. On PipeWire
     *      systems pipewire-pulse serves this protocol; on classic
     *      PulseAudio systems the PA daemon does. Either way the audio
     *      reaches the same backend the user has configured, and the
     *      libasound-side shim (libasound_module_pcm_pulse.so) doesn't
     *      have the per-open leak that libasound_module_pcm_pipewire.so does.
     *   3. "default" - original ALSA behaviour, used when neither PA nor
     *      PipeWire-pulse is reachable (pure dmix/hw systems, headless
     *      servers, sandboxed environments).
     */
    [[nodiscard]]
    inline String preferred_default_pcm_name() noexcept {
        static StringView result = [] noexcept -> StringView {
            if (
                Optional<StringView> env = Environment::get("STDLIBX_AUDIO_DEFAULT_PCM");
                env.has_value() && !env->empty()
            ) {
                return *env;
            }
            SoundPcm* probe = nullptr;
            const i32 r = linux::alsa::snd_pcm_open(
                &probe, "pulse",
                SoundPcmStream::PLAYBACK,
                0
            );
            if (r >= 0 && probe) {
                linux::alsa::snd_pcm_close(probe);
                return "pulse";
            }
            return "default";
        }();
        return String(result);
    }
}

export namespace stdx::audio::sampled {

    Vector<DeviceInfo> AudioSystem::output_devices() throws (AudioException) {
        return enumerate_alsa(StreamDirection::OUTPUT);
    }

    Vector<DeviceInfo> AudioSystem::input_devices() throws (AudioException) {
        return enumerate_alsa(StreamDirection::INPUT);
    }

    DeviceInfo AudioSystem::default_output() throws (AudioDeviceNotAvailableException) {
        return DeviceInfo {
            .id = preferred_default_pcm_name(),
            .name = "Default output",
            .direction = StreamDirection::OUTPUT,
            .backend = AudioBackend::ALSA,
            .is_default = true,
        };
    }

    DeviceInfo AudioSystem::default_input() throws (AudioDeviceNotAvailableException) {
        return DeviceInfo {
            .id = preferred_default_pcm_name(),
            .name = "Default input",
            .direction = StreamDirection::INPUT,
            .backend = AudioBackend::ALSA,
            .is_default = true,
        };
    }

    UniquePointer<OutputLine> AudioSystem::open_output(
        const DeviceInfo& device,
        AudioFormat fmt,
        RenderCallback callback
    ) throws (UnsupportedAudioFormatException, LineUnavailableException) {
        if (fmt.format != SampleFormat::FLOAT) {
            throw UnsupportedAudioFormatException(
                "ALSA backend currently supports only SampleFormat::FLOAT"
            );
        }
        SoundPcm* pcm = nullptr;
        const char* name = device.id.empty() ? "default" : device.id.c_str();
        i32 r = linux::alsa::snd_pcm_open(&pcm, name, SoundPcmStream::PLAYBACK, 0);
        if (r < 0) {
            throw LineUnavailableException(linux::alsa::snd_strerror(r));
        }

        AudioFormat actual = fmt;
        SoundPcmUFrames period = 0;
        SoundPcmUFrames buffer = 0;
        try {
            configure_pcm(pcm, actual, period, buffer);
        } catch (...) {
            linux::alsa::snd_pcm_close(pcm);
            throw;
        }
        return Pointers::unique<AlsaOutputLine>(
            pcm, actual,
            static_cast<u32>(period),
            static_cast<u32>(buffer),
            Ops::move(callback)
        );
    }

    UniquePointer<InputLine> AudioSystem::open_input(
        const DeviceInfo& device,
        AudioFormat fmt,
        CaptureCallback callback
    ) throws (UnsupportedAudioFormatException, LineUnavailableException) {
        if (fmt.format != SampleFormat::FLOAT) {
            throw UnsupportedAudioFormatException(
                "ALSA backend currently supports only SampleFormat::FLOAT"
            );
        }
        SoundPcm* pcm = nullptr;
        const char* name = device.id.empty() ? "default" : device.id.c_str();
        i32 r = linux::alsa::snd_pcm_open(&pcm, name, SoundPcmStream::CAPTURE, 0);
        if (r < 0) {
            throw LineUnavailableException(linux::alsa::snd_strerror(r));
        }

        AudioFormat actual = fmt;
        SoundPcmUFrames period = 0;
        SoundPcmUFrames buffer = 0;
        try {
            configure_pcm(pcm, actual, period, buffer);
        } catch (...) {
            linux::alsa::snd_pcm_close(pcm);
            throw;
        }
        return Pointers::unique<AlsaInputLine>(
            pcm, actual,
            static_cast<u32>(period),
            static_cast<u32>(buffer),
            Ops::move(callback)
        );
    }
}
