#pragma once

using stdx::io::IOException;

/**
 * @namespace stdx::audio
 * @brief Audio module exceptions.
 */
export namespace stdx::audio {
    /**
     * @class AudioException
     * @brief Base class for failures in the audio subsystem.
     * @extends IOException
     */
    class AudioException: public IOException {
    public:
        explicit AudioException(const String& msg = ""):
            IOException(msg) {}
    };

    /**
     * @class UnsupportedAudioFormatException
     * @brief Thrown when a requested AudioFormat cannot be honored by a device.
     * @extends AudioException
     */
    class UnsupportedAudioFormatException: public AudioException {
    public:
        explicit UnsupportedAudioFormatException(const String& msg = ""):
            AudioException(msg) {}
    };

    /**
     * @class AudioDeviceNotAvailableException
     * @brief Thrown when the requested audio device cannot be opened.
     * @extends AudioException
     */
    class AudioDeviceNotAvailableException: public AudioException {
    public:
        explicit AudioDeviceNotAvailableException(const String& msg = ""):
            AudioException(msg) {}
    };

    /**
     * @class LineUnavailableException
     * @brief Thrown when a sampled Line cannot be opened or started.
     * @extends AudioException
     */
    class LineUnavailableException: public AudioException {
    public:
        explicit LineUnavailableException(const String& msg = ""):
            AudioException(msg) {}
    };

    /**
     * @class MidiException
     * @brief Base class for MIDI subsystem failures.
     * @extends AudioException
     */
    class MidiException: public AudioException {
    public:
        explicit MidiException(const String& msg = ""):
            AudioException(msg) {}
    };

    /**
     * @class InvalidMidiDataException
     * @brief Thrown when a MidiMessage / Sequence is malformed.
     * @extends MidiException
     */
    class InvalidMidiDataException: public MidiException {
    public:
        explicit InvalidMidiDataException(const String& msg = ""):
            MidiException(msg) {}
    };
}
