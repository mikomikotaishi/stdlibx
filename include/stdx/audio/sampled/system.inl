#pragma once

using stdx::fs::Path;
using stdx::mem::UniquePointer;

/**
 * @namespace stdx::audio::sampled
 * @brief AudioSystem - the entry point for enumerating devices and opening lines.
 *
 * The implementation dispatches to whichever backend is built on this
 * platform (ALSA on Linux, WASAPI on Windows, CoreAudio on macOS). Backend
 * implementations live in stdx/audio/sampled/{linux,win32,darwin}/.
 */
export namespace stdx::audio::sampled {
    /**
     * @class AudioSystem
     * @brief Entry point for enumerating devices and opening lines.
     */
    class AudioSystem final {
    public:
        AudioSystem() = delete("AudioSystem is a static utility class and cannot be instantiated.");

        /**
         * @brief Returns every output (playback) device the host advertises.
         * @throws AudioException if the query fails (e.g. backend error).
         */
        [[nodiscard]]
        THROWS(AudioException)
        static Vector<DeviceInfo> output_devices();

        /**
         * @brief Returns every input (capture) device the host advertises.
         * @throws AudioException if the query fails (e.g. backend error).
         */
        [[nodiscard]]
        THROWS(AudioException)
        static Vector<DeviceInfo> input_devices();

        /**
         * @brief The system's preferred default output device.
         * @throws AudioDeviceNotAvailableException if none is configured.
         */
        [[nodiscard]]
        THROWS(AudioDeviceNotAvailableException)
        static DeviceInfo default_output();

        /**
         * @brief The system's preferred default input device.
         * @throws AudioDeviceNotAvailableException if none is configured.
         */
        [[nodiscard]]
        THROWS(AudioDeviceNotAvailableException)
        static DeviceInfo default_input();

        /**
         * @brief Open an output line on the supplied device with the requested
         * format, calling @p callback on the backend render thread.
         * @param device The device to open. Must be an output device returned by
         * output_devices() or default_output().
         * @param format The requested audio format. The device may not support
         * all formats; if it doesn't support the requested one, this call fails with
         * UnsupportedAudioFormatException.
         * @param callback The render callback to use for the line.
         * @return An open line ready for start().
         * @throws UnsupportedAudioFormatException if the device cannot honour
         * the requested format.
         * @throws LineUnavailableException if the device is busy or cannot be
         * opened.
         */
        [[nodiscard]]
        THROWS(UnsupportedAudioFormatException, LineUnavailableException)
        static UniquePointer<OutputLine> open_output(const DeviceInfo& device, AudioFormat format, RenderCallback callback);

        /**
         * @brief Open an input line. See open_output() for failure modes.
         * @param device The device to open. Must be an input device returned by
         * input_devices() or default_input().
         * @param format The requested audio format. The device may not support
         * all formats; if it doesn't support the requested one, this call fails with
         * UnsupportedAudioFormatException.
         * @param callback The capture callback to use for the line.
         * @return An open line ready for start().
         * @throws UnsupportedAudioFormatException if the device cannot honour
         * the requested format.
         * @throws LineUnavailableException if the device is busy or cannot be
         * opened.
         */
        [[nodiscard]]
        THROWS(UnsupportedAudioFormatException, LineUnavailableException)
        static UniquePointer<InputLine> open_input(const DeviceInfo& device, AudioFormat format, CaptureCallback callback);

        /**
         * @brief Convenience: open the default output line for @p format.
         * @param format The requested audio format. The default output may not support
         * all formats; if it doesn't support the requested one, this call fails with
         * UnsupportedAudioFormatException. To open the default output with a fallback
         * format, query default_output() and call open_output() directly.
         * @param callback The render callback to use for the line.
         * @return An open line ready for start().
         * @throws UnsupportedAudioFormatException if the default output cannot honor
         * the requested format.
         * @throws LineUnavailableException if the default output is busy or cannot be
         * opened.
         *
         * @note On Linux, the default PCM is auto-resolved: the backend probes
         * libasound's PulseAudio shim ("pulse") first and uses it when
         * available, otherwise falls back to ALSA's plain "default". The PA
         * shim routes to the same PipeWire daemon via pipewire-pulse on
         * PipeWire systems, or to the PulseAudio daemon on classic systems -
         * either way without the per-open allocation leak that
         * libasound_module_pcm_pipewire.so exhibits when opened directly.
         * Set the @c STDLIBX_AUDIO_DEFAULT_PCM env var to override (e.g.
         * "default" to force the native PipeWire shim, or "hw:0,0" for
         * direct hardware).
         */
        [[nodiscard]]
        THROWS(UnsupportedAudioFormatException, LineUnavailableException)
        static UniquePointer<OutputLine> open_default_output(AudioFormat format, RenderCallback callback) {
            return open_output(default_output(), format, Ops::move(callback));
        }

        /**
         * @brief Open an audio file as a pull-style f32 stream.
         *
         * The current build supports WAV (PCM s16/s24/s32 + IEEE float).
         * Other containers throw UnsupportedAudioFileException - they will
         * become available as optional extensions in future revisions.
         * @param path The file to open.
         * @return An AudioInputStream for the file, positioned at the start of
         * the audio data.
         * @throws UnsupportedAudioFileException if the file can't be opened or
         * isn't a supported format.
         */
        [[nodiscard]]
        THROWS(UnsupportedAudioFileException)
        static UniquePointer<AudioInputStream> open_audio_file(const Path& path);
    };
}

#if defined(_WIN32) && __has_include(<mmsystem.h>)

#elif defined(__APPLE__)

#elif defined(__linux__) && __has_include(<alsa/asoundlib.h>)
#include "stdx/audio/sampled/linux/alsa_pcm.inl"
#else
export namespace stdx::audio::sampled {
    Vector<DeviceInfo> AudioSystem::output_devices() {
        return {};
    }

    Vector<DeviceInfo> AudioSystem::input_devices() {
        return {};
    }

    DeviceInfo AudioSystem::default_output() {
        throw AudioDeviceNotAvailableException("no sampled backend wired in this build");
    }

    DeviceInfo AudioSystem::default_input() {
        throw AudioDeviceNotAvailableException("no sampled backend wired in this build");
    }

    UniquePointer<OutputLine> AudioSystem::open_output(
        [[maybe_unused]] const DeviceInfo& device,
        [[maybe_unused]] AudioFormat format,
        [[maybe_unused]] RenderCallback callback
    ) {
        throw LineUnavailableException("no sampled backend wired in this build");
    }

    UniquePointer<InputLine> AudioSystem::open_input(
        [[maybe_unused]] const DeviceInfo& device,
        [[maybe_unused]] AudioFormat format,
        [[maybe_unused]] CaptureCallback callback
    ) {
        throw LineUnavailableException("no sampled backend wired in this build");
    }
}
#endif
