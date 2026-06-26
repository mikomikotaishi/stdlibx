#pragma once

using stdx::collections::Vector;

/**
 * @namespace stdx::audio::midi
 * @brief MIDI 1.0 message types
 */
export namespace stdx::audio::midi {
    /**
     * @enum Status
     * @brief MIDI 1.0 status bytes for channel messages. The low nibble holds
     * the channel (0-15) and is masked out of the constant values
     * themselves.
     */
    enum class Status: u8 {
        NOTE_OFF = 0x80,
        NOTE_ON = 0x90,
        POLY_KEY_PRESSURE = 0xA0, ///< Aftertouch per-key
        CONTROL_CHANGE = 0xB0,
        PROGRAM_CHANGE = 0xC0,
        CHANNEL_PRESSURE = 0xD0, ///< Aftertouch channel-wide
        PITCH_BEND = 0xE0,
        SYSTEM_EXCLUSIVE = 0xF0,
        MIDI_TIME_CODE = 0xF1,
        SONG_POSITION = 0xF2,
        SONG_SELECT = 0xF3,
        TUNE_REQUEST = 0xF6,
        END_OF_EXCLUSIVE = 0xF7,
        TIMING_CLOCK = 0xF8,
        START = 0xFA,
        CONTINUE = 0xFB,
        STOP = 0xFC,
        ACTIVE_SENSING = 0xFE,
        SYSTEM_RESET = 0xFF,
        META = 0xFF, ///< In a Standard MIDI File context
    };

    /**
     * @class MidiMessage
     * @brief Base class for any MIDI message - short, meta, or sysex.
     *
     * Owns the raw bytes. @ref ShortMessage and @ref SysexMessage refine the
     * accessor surface; nothing prevents direct construction from a byte
     * buffer either.
     */
    class MidiMessage {
    protected:
        Vector<u8> bytes;
    public:
        MidiMessage() = default;

        explicit MidiMessage(Vector<u8> raw):
            bytes{Ops::move(raw)} {}

        virtual ~MidiMessage() = default;

        [[nodiscard]]
        Span<const u8> data() const noexcept {
            return Span{bytes.data(), bytes.size()};
        }

        [[nodiscard]]
        usize length() const noexcept {
            return bytes.size();
        }

        [[nodiscard]]
        u8 status() const noexcept {
            return bytes.empty() ? 0 : bytes[0];
        }
    };

    /**
     * @class ShortMessage
     * @brief Three-byte (or two-byte) channel-voice message: Note On/Off,
     * Control Change, Pitch Bend, etc.
     * @extends MidiMessage
     */
    class ShortMessage final: public MidiMessage {
    public:
        ShortMessage() = default;

        /**
         * @param status Status byte WITHOUT channel bits set, e.g. Status::NoteOn.
         * @param channel 0-15.
         * @param data1 First data byte (note number, controller index, ...).
         * @param data2 Second data byte (velocity, controller value, ...). 0 for two-byte messages.
         */
        THROWS(InvalidMidiDataException)
        ShortMessage(Status status, u8 channel, u8 data1, u8 data2 = 0) {
            set_message(status, channel, data1, data2);
        }

        THROWS(InvalidMidiDataException)
        void set_message(Status status, u8 channel, u8 data1, u8 data2 = 0) {
            if (channel > 15) {
                throw InvalidMidiDataException("channel out of range");
            }
            if (data1 > 127 || data2 > 127) {
                throw InvalidMidiDataException("data byte out of range");
            }
            const u8 s = static_cast<u8>(status);
            const bool two_byte = (s == static_cast<u8>(Status::PROGRAM_CHANGE))
                || (s == static_cast<u8>(Status::CHANNEL_PRESSURE));
            bytes.clear();
            bytes.push_back(static_cast<u8>(s | (channel & 0x0F)));
            bytes.push_back(data1);
            if (!two_byte) {
                bytes.push_back(data2);
            }
        }

        [[nodiscard]]
        u8 channel() const noexcept {
            return bytes.empty() ? 0 : static_cast<u8>(bytes[0] & 0x0F);
        }

        [[nodiscard]]
        Status command() const noexcept {
            return bytes.empty()
                ? Status::SYSTEM_RESET
                : static_cast<Status>(bytes[0] & 0xF0);
        }

        [[nodiscard]]
        u8 data1() const noexcept {
            return bytes.size() > 1 ? bytes[1] : 0;
        }

        [[nodiscard]]
        u8 data2() const noexcept {
            return bytes.size() > 2 ? bytes[2] : 0;
        }
    };

    /**
     * @class SysexMessage
     * @brief Variable-length sysex/system-common message; raw bytes include
     * the leading 0xF0 and trailing 0xF7.
     * @extends MidiMessage
     */
    class SysexMessage final: public MidiMessage {
    public:
        SysexMessage() = default;

        explicit SysexMessage(Span<const u8> raw):
            MidiMessage(Vector<u8>(raw.begin(), raw.end())) {}
    };

    /**
     * @brief Standard MIDI File meta event (tempo, time signature, track
     * name, ...). Only meaningful inside a Sequence - wire MIDI never
     * transmits these.
     * @extends MidiMessage
     */
    class MetaMessage final: public MidiMessage {
    public:
        static constexpr u8 META_STATUS = 0xFF;

        MetaMessage() = default;

        THROWS(InvalidMidiDataException)
        MetaMessage(u8 type, Span<const u8> payload) {
            if (type > 0x7F) {
                throw InvalidMidiDataException("meta type out of range");
            }
            bytes.push_back(META_STATUS);
            bytes.push_back(type);
            // Variable-length payload size, 7-bit groups MSB-first.
            usize n = payload.size();
            u8 buf[5];
            usize bi = 0;
            buf[bi++] = static_cast<u8>(n & 0x7F);
            while ((n >>= 7) != 0) {
                buf[bi++] = static_cast<u8>(0x80 | (n & 0x7F));
            }
            for (usize i = bi; i > 0; --i) {
                bytes.push_back(buf[i - 1]);
            }
            bytes.insert(bytes.end(), payload.begin(), payload.end());
        }

        [[nodiscard]]
        u8 type() const noexcept {
            return bytes.size() > 1 ? bytes[1] : 0;
        }
    };
}
