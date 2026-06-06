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
        using IOException::IOException;
    };

    /**
     * @class UnsupportedAudioFormatException
     * @brief Thrown when a requested AudioFormat cannot be honored by a device.
     * @extends AudioException
     */
    class UnsupportedAudioFormatException: public AudioException {
    public:
        using AudioException::AudioException;
    };

    /**
     * @class AudioDeviceNotAvailableException
     * @brief Thrown when the requested audio device cannot be opened.
     * @extends AudioException
     */
    class AudioDeviceNotAvailableException: public AudioException {
    public:
        using AudioException::AudioException;
    };

    /**
     * @namespace sampled
     */
    namespace sampled {
        /**
         * @class LineUnavailableException
         * @brief Thrown when a sampled Line cannot be opened or started.
         * @extends AudioException
         */
        class LineUnavailableException: public AudioException {
        public:
            using AudioException::AudioException;
        };

        /**
         * @class UnsupportedAudioFileException
         * @brief Thrown when an audio file cannot be decoded - unrecognized
         * container, missing required chunks, or a compression codec the
         * built-in decoders don't support.
         * @extends AudioException
         */
        class UnsupportedAudioFileException: public AudioException {
        public:
            using AudioException::AudioException;
        };
    }

    /**
     * @namespace midi
     */
    namespace midi {
        /**
         * @class MidiException
         * @brief Base class for MIDI subsystem failures.
         * @extends AudioException
         */
        class MidiException: public AudioException {
        public:
            using AudioException::AudioException;
        };

        /**
         * @class InvalidMidiDataException
         * @brief Thrown when a MidiMessage / Sequence is malformed.
         * @extends MidiException
         */
        class InvalidMidiDataException: public MidiException {
        public:
            using MidiException::MidiException;
        };
    }
}
