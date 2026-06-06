#pragma once

/**
 * @namespace stdx::audio::sampled
 * @brief Audio device discovery.
 */
export namespace stdx::audio::sampled {
    /**
     * @enum StreamDirection
     * @brief Direction of audio flow for a device or line.
     */
    enum class StreamDirection: u8 {
        OUTPUT, ///< Playback / render
        INPUT, ///< Capture / record
    };

    /**
     * @enum AudioBackend
     * @brief Backend identifier - which OS audio system a device comes from.
     */
    enum class AudioBackend: u8 {
        NONE, ///< No audio support; placeholder for error cases.
        ALSA, ///< Linux: Advanced Linux Sound Architecture
        WASAPI, ///< Windows: Windows Audio Session API
        WINMM, ///< Windows: legacy Multimedia API
        CORE_AUDIO, ///< macOS/iOS: Core Audio
    };

    /**
     * @struct DeviceInfo
     * @brief Description of one audio endpoint reported by the host OS.
     *
     * Holds the data needed to identify a device. Concrete handles for
     * opening lines live behind the AudioSystem factory methods.
     */
    struct DeviceInfo {
        String id; ///< Backend-stable identifier (used to re-open the same device).
        String name; ///< Human-readable display name.
        StreamDirection direction;
        AudioBackend backend = AudioBackend::NONE;
        bool is_default = false;

        /**
         * @brief Formats the device advertises support for. Empty means "ask the
         * device to negotiate"; backends like ALSA enumerate lazily.
         */
        Vector<AudioFormat> supported_formats;

        [[nodiscard]]
        friend bool operator==(const DeviceInfo& a, const DeviceInfo& b) noexcept {
            return a.id == b.id && a.direction == b.direction && a.backend == b.backend;
        }
    };
}
