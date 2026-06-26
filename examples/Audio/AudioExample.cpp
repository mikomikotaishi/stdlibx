import stdx;

using stdx::audio::AudioException;
using stdx::audio::sampled::AudioBackend;
using stdx::audio::sampled::AudioFormat;
using stdx::audio::sampled::AudioInputStream;
using stdx::audio::sampled::AudioSystem;
using stdx::audio::sampled::AudioTime;
using stdx::audio::sampled::Clip;
using stdx::audio::sampled::DeviceInfo;
using stdx::audio::sampled::OutputLine;
using stdx::audio::sampled::SampleFormat;
using stdx::audio::sampled::StreamDirection;
using stdx::audio::sampled::UnsupportedAudioFileException;
using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::mem::UniquePointer;
using stdx::util::ArgumentParser;
using stdx::util::DefaultArguments;

#ifdef __GNUC__
using namespace stdx::core;
using namespace stdx::literals;
#endif

/**
 * @struct SineRenderer
 * @brief 440 Hz sine generator. Stateful across render callbacks via the
 * phase field; the audio thread is the sole owner.
 */
struct SineRenderer {
    f32 phase = 0.0f;
    f32 phase_inc;
    u16 channels;

    void operator()(Span<f32> out, AudioTime) noexcept {
        static constexpr f32 TWO_PI = static_cast<f32>(Math::PI * 2.0);
        const usize frames = out.size() / channels;
        for (usize i = 0; i < frames; ++i) {
            const f32 s = 0.2f * Math::sin(phase);
            phase += phase_inc;
            if (phase >= TWO_PI) {
                phase -= TWO_PI;
            }
            for (u16 c = 0; c < channels; ++c) {
                out[i * channels + c] = s;
            }
        }
    }
};

void list_devices() {
    Vector<DeviceInfo> devices;
    try {
        devices = AudioSystem::output_devices();
    } catch (const AudioException& e) {
        System::err.println("output_devices() threw: {}", e.what());
        return;
    }

    System::out.println("Output devices: {}", devices.size());
    for (const DeviceInfo& d: devices) {
        System::out.println(
            "  - {} (id={}, default={})",
            d.name,
            d.id,
            d.is_default ? "yes" : "no"
        );
    }
}

void play_sine(const String& requested_id) {
    // Empty requested_id => use AudioSystem::default_output() so we go
    // through the backend's preferred-default resolver (which on Linux
    // auto-probes for the leak-free "pulse" shim). Explicit names like
    // "null" or "plughw:0,3" bypass the resolver - useful as controls.
    DeviceInfo dev;
    if (requested_id.empty()) {
        try {
            dev = AudioSystem::default_output();
        } catch (const AudioException& e) {
            System::out.println("No default output: {}", e.what());
            return;
        }
    } else {
        dev = DeviceInfo {
            .id = requested_id,
            .name = requested_id,
            .direction = StreamDirection::OUTPUT,
            .backend = AudioBackend::ALSA,
            .is_default = false,
        };
    }
    System::out.println("Target output: {} (id={})", dev.name, dev.id);

    AudioFormat fmt{
        .sample_rate = 48000,
        .channels = 2,
        .format = SampleFormat::FLOAT,
    };

    SineRenderer renderer{
        .phase = 0.0f,
        .phase_inc = static_cast<f32>(Math::PI * 440.0) / static_cast<f32>(fmt.sample_rate),
        .channels = fmt.channels,
    };

    try {
        UniquePointer<OutputLine> line = AudioSystem::open_output(dev, fmt, renderer);
        System::out.println(
            "Opened line: {} Hz, {} ch, {} frame latency",
            line->format().sample_rate,
            line->format().channels,
            line->latency_frames()
        );
        line->start();
        System::Thread::sleep_for(500ms);
        line->stop();
        System::out.println("Underruns: {}", line->underrun_count());
    } catch (const AudioException& e) {
        System::err.println("Playback failed: {}", e.what());
    }
}

void play_file(const String& path_str) {
    Path path{path_str};
    UniquePointer<AudioInputStream> stream;
    try {
        stream = AudioSystem::open_audio_file(path);
    } catch (const UnsupportedAudioFileException& e) {
        System::err.println("Cannot decode {}: {}", path_str, e.what());
        return;
    } catch (const AudioException& e) {
        System::err.println("Audio error opening {}: {}", path_str, e.what());
        return;
    }

    const AudioFormat fmt = stream->format();
    const u64 total = stream->total_frames();
    System::out.println(
        "Decoded {}: {} Hz, {} ch, {} frames ({:.2f} s)",
        path_str,
        fmt.sample_rate,
        fmt.channels,
        total,
        static_cast<f64>(total) / static_cast<f64>(fmt.sample_rate)
    );

    try {
        Clip clip{Ops::move(stream)};
        clip.play();
        clip.wait_for_completion();
        clip.close();
        System::out.println("Playback finished.");
    } catch (const AudioException& e) {
        System::err.println("Playback failed: {}", e.what());
    }
}

int main(int argc, char* argv[]) {
    ArgumentParser parser("AudioTest", "1.0", DefaultArguments::NONE);
    parser.add_argument("--play")
        .flag()
        .help("Render a 440 Hz sine wave to the default output for 500 ms.");
    parser.add_argument("--file")
        .default_value("")
        .help(
            "Path to an audio file (currently WAV PCM s16/s24/s32 or IEEE "
            "float). When given, plays the file to completion instead of the "
            "440 Hz sine."
        );
    parser.add_argument("--repeat")
        .default_value(1)
        .scan<'d', i32>()
        .help(
            "Number of times to repeat the chosen action. Used to compare "
            "LSan totals across runs and confirm any leaks are one-time "
            "process-init cost (constant total) and not per-open (linear in N)."
        );
    parser.add_argument("--device")
        .default_value("")
        .help(
            "ALSA PCM name to open (e.g. \"default\", \"null\", \"plughw:0,0\"). "
            "Defaults to \"default\", which on PipeWire-managed systems routes "
            "through the libasound_module_pcm_pipewire.so shim. Explicit names "
            "like \"plughw:N,M\" bypass the shim."
        );

    parser.parse_args(argc, argv);

    list_devices();

    const String file_arg = parser.get("--file");
    const i32 repeat = parser.get<i32>("--repeat");
    const String device = parser.get("--device");

    if (!file_arg.empty()) {
        for (i32 i = 0; i < repeat; ++i) {
            System::out.println("\n[run {}/{}] Playing file: {}", i + 1, repeat, file_arg);
            play_file(file_arg);
        }
        return System::EXIT_SUCCESS;
    }

    if (parser.get<bool>("--play")) {
        for (i32 i = 0; i < repeat; ++i) {
            System::out.println("\n[run {}/{}] Playing 440 Hz sine for 500 ms...", i + 1, repeat);
            play_sine(device);
        }
    } else {
        System::out.println(
            "\n(Pass --play to render a sine wave, or --file <path> to play "
            "an audio file.)"
        );
    }
    return System::EXIT_SUCCESS;
}
