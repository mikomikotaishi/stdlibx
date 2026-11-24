#pragma once

using stdx::collections::Vector;
using stdx::mem::Pointers;
using stdx::mem::UniquePointer;

/**
 * @namespace stdx::audio::midi
 * @brief MIDI 2.0 Universal MIDI Packet (UMP) core types.
 *
 * This layer sits on top of the MIDI 1.0 message model in message.inl. A
 * Universal MIDI Packet is the MIDI 2.0 transport word-group: one to four
 * big-endian 32-bit words whose leading nibble (the Message Type) selects
 * both the packet size and the family of message it carries.
 *
 * Following the javax.sound.midi vocabulary this library mirrors, UMP packets
 * are surfaced as a branch of the @ref MidiMessage hierarchy (@ref UmpMessage)
 * rather than as free-standing structs, so the existing Receiver / Transmitter
 * routing keeps working unchanged.
 */
export namespace stdx::audio::midi {
    /**
     * @enum MessageType
     * @brief UMP Message Type - the high nibble of word 0.
     *
     * The Message Type also fixes the packet's word count, so it doubles as a
     * size discriminator. See @ref UniversalMidiPacket::word_count.
     */
    enum class MessageType: u8 {
        UTILITY = 0x0,            ///< 32-bit: NOOP, JR Clock, JR Timestamp.
        SYSTEM = 0x1,             ///< 32-bit: system real-time and common.
        MIDI1_CHANNEL_VOICE = 0x2, ///< 32-bit: MIDI 1.0 channel voice in UMP form.
        DATA_64 = 0x3,            ///< 64-bit: SysEx7.
        MIDI2_CHANNEL_VOICE = 0x4, ///< 64-bit: MIDI 2.0 channel voice.
        DATA_128 = 0x5,           ///< 128-bit: SysEx8 and Mixed Data Set.
        RESERVED_6 = 0x6,         ///< 32-bit: reserved.
        RESERVED_7 = 0x7,         ///< 32-bit: reserved.
        RESERVED_8 = 0x8,         ///< 64-bit: reserved.
        RESERVED_9 = 0x9,         ///< 64-bit: reserved.
        RESERVED_A = 0xA,         ///< 64-bit: reserved.
        RESERVED_B = 0xB,         ///< 96-bit: reserved.
        RESERVED_C = 0xC,         ///< 96-bit: reserved.
        FLEX_DATA = 0xD,          ///< 128-bit: Flex Data.
        RESERVED_E = 0xE,         ///< 128-bit: reserved.
        STREAM = 0xF,             ///< 128-bit: UMP Stream (endpoint / function block).
    };

    /**
     * @enum Midi2Status
     * @brief Status bytes for MIDI 2.0 channel-voice messages.
     *
     * These parallel @ref MidiStatus but cover the opcodes that only exist in
     * MIDI 2.0. Shared opcodes keep the same numeric nibble as their 1.0
     * counterparts so translation is a straight copy of the high nibble.
     */
    enum class Midi2Status: u8 {
        REGISTERED_PER_NOTE_CONTROLLER = 0x00,
        ASSIGNABLE_PER_NOTE_CONTROLLER = 0x10,
        REGISTERED_CONTROLLER = 0x20,   ///< RPN, 32-bit value.
        ASSIGNABLE_CONTROLLER = 0x30,   ///< NRPN, 32-bit value.
        RELATIVE_REGISTERED_CONTROLLER = 0x40,
        RELATIVE_ASSIGNABLE_CONTROLLER = 0x50,
        PER_NOTE_PITCH_BEND = 0x60,
        NOTE_OFF = 0x80,
        NOTE_ON = 0x90,
        POLY_PRESSURE = 0xA0,
        CONTROL_CHANGE = 0xB0,
        PROGRAM_CHANGE = 0xC0,
        CHANNEL_PRESSURE = 0xD0,
        PITCH_BEND = 0xE0,
        PER_NOTE_MANAGEMENT = 0xF0,
    };

    /**
     * @enum UmpProtocol
     * @brief Which channel-voice protocol a UMP endpoint is speaking.
     *
     * MIDI 1.0 data travels as 32-bit MIDI 1.0 channel-voice packets; MIDI 2.0
     * as 64-bit MIDI 2.0 channel-voice packets. Everything else (system,
     * SysEx, flex data, stream) is the same under both.
     */
    enum class UmpProtocol: u8 {
        MIDI1, ///< Channel voice as Message Type 0x2.
        MIDI2, ///< Channel voice as Message Type 0x4.
    };

    /**
     * @class UniversalMidiPacket
     * @brief A single Universal MIDI Packet: one to four big-endian 32-bit
     * words.
     *
     * This is a small value type (16 bytes of storage plus a length). It is
     * copyable and trivially movable; it carries no ownership. Word 0 always
     * holds the Message Type in its top nibble and, for most types, the UMP
     * group in the next nibble.
     */
    class UniversalMidiPacket {
    protected:
        u32 _words[4] = {0, 0, 0, 0};
        u8 _count = 1; ///< Number of valid words, 1-4.
    public:
        UniversalMidiPacket() = default;

        /// 32-bit packet.
        explicit constexpr UniversalMidiPacket(u32 w0) noexcept:
            _words{w0, 0, 0, 0}, _count{1} {}

        /// 64-bit packet.
        constexpr UniversalMidiPacket(u32 w0, u32 w1) noexcept:
            _words{w0, w1, 0, 0}, _count{2} {}

        /// 96-bit packet (only reserved Message Types use this size today).
        constexpr UniversalMidiPacket(u32 w0, u32 w1, u32 w2) noexcept:
            _words{w0, w1, w2, 0}, _count{3} {}

        /// 128-bit packet.
        constexpr UniversalMidiPacket(u32 w0, u32 w1, u32 w2, u32 w3) noexcept:
            _words{w0, w1, w2, w3}, _count{4} {}

        /**
         * @brief Build a packet from @p words, taking as many as the Message
         * Type in word 0 calls for.
         * @return The packet, or empty if @p words is empty or too short for
         * the size word 0 declares.
         */
        [[nodiscard]]
        static constexpr Optional<UniversalMidiPacket> from_words(Span<const u32> words) noexcept {
            if (words.empty()) {
                return {};
            }
            const u8 n = words_for(static_cast<MessageType>((words[0] >> 28) & 0x0F));
            if (words.size() < n) {
                return {};
            }
            UniversalMidiPacket p;
            for (u8 i = 0; i < n; ++i) {
                p._words[i] = words[i];
            }
            p._count = n;
            return p;
        }

        /**
         * @brief The Message Type carried in the top nibble of word 0.
         */
        [[nodiscard]]
        constexpr MessageType type() const noexcept {
            return static_cast<MessageType>((_words[0] >> 28) & 0x0F);
        }

        /**
         * @brief The UMP group (0-15) in the second nibble of word 0.
         *
         * Utility messages carry no group; the value is still readable but
         * reserved there by the spec.
         */
        [[nodiscard]]
        constexpr u8 group() const noexcept {
            return static_cast<u8>((_words[0] >> 24) & 0x0F);
        }

        /**
         * @brief Number of valid 32-bit words in this packet (1-4).
         */
        [[nodiscard]]
        constexpr u8 word_count() const noexcept {
            return _count;
        }

        /**
         * @param i Word index, 0-3.
         * @return The 32-bit word at @p i, or 0 if out of range.
         */
        [[nodiscard]]
        constexpr u32 word(usize i) const noexcept {
            return i < 4 ? _words[i] : 0;
        }

        /**
         * @brief The canonical word count for a given Message Type.
         *
         * UMP fixes every Message Type, reserved ones included, to one of 1,
         * 2, 3, or 4 words. This lets a decoder know how many more words to
         * read after seeing word 0 - and skip a reserved packet whole rather
         * than misreading its tail as the start of the next one.
         */
        [[nodiscard]]
        static constexpr u8 words_for(MessageType t) noexcept {
            switch (t) {
                case MessageType::UTILITY:
                case MessageType::SYSTEM:
                case MessageType::MIDI1_CHANNEL_VOICE:
                case MessageType::RESERVED_6:
                case MessageType::RESERVED_7:
                    return 1;
                case MessageType::DATA_64:
                case MessageType::MIDI2_CHANNEL_VOICE:
                case MessageType::RESERVED_8:
                case MessageType::RESERVED_9:
                case MessageType::RESERVED_A:
                    return 2;
                case MessageType::RESERVED_B:
                case MessageType::RESERVED_C:
                    return 3;
                case MessageType::DATA_128:
                case MessageType::FLEX_DATA:
                case MessageType::RESERVED_E:
                case MessageType::STREAM:
                    return 4;
            }
            return 1;
        }

        /**
         * @brief Append this packet's words to @p out as big-endian wire bytes
         * (MSB first), the canonical UMP serialization order.
         */
        void to_bytes(Vector<u8>& out) const {
            for (u8 i = 0; i < _count; ++i) {
                const u32 w = _words[i];
                out.push_back(static_cast<u8>((w >> 24) & 0xFF));
                out.push_back(static_cast<u8>((w >> 16) & 0xFF));
                out.push_back(static_cast<u8>((w >> 8) & 0xFF));
                out.push_back(static_cast<u8>(w & 0xFF));
            }
        }

        /**
         * @brief Assemble a 32-bit word from four big-endian bytes.
         *
         * Reads MSB first, so the result is correct on any host regardless of
         * the CPU's own byte order - the right primitive for moving UMP wire
         * bytes into the native-endian words an OS UMP API expects.
         */
        [[nodiscard]]
        static constexpr u32 word_from_be(const u8* p) noexcept {
            return (static_cast<u32>(p[0]) << 24)
                | (static_cast<u32>(p[1]) << 16)
                | (static_cast<u32>(p[2]) << 8)
                | static_cast<u32>(p[3]);
        }

        [[nodiscard]]
        friend constexpr bool operator==(const UniversalMidiPacket& a, const UniversalMidiPacket& b) noexcept {
            if (a._count != b._count) {
                return false;
            }
            for (u8 i = 0; i < a._count; ++i) {
                if (a._words[i] != b._words[i]) {
                    return false;
                }
            }
            return true;
        }
    };

    /**
     * @class UmpMessage
     * @brief A MIDI 2.0 message carried as one Universal MIDI Packet.
     * @extends MidiMessage
     *
     * Bridges the packet-oriented MIDI 2.0 world into the byte-oriented
     * @ref MidiMessage hierarchy. The inherited @c data() exposes the packet
     * as big-endian bytes (MSB first, per the UMP wire order) so a byte-stream
     * Receiver can forward it, while @ref packet() hands back the structured
     * form.
     */
    class UmpMessage: public MidiMessage {
    protected:
        UniversalMidiPacket _packet;

        /// Refresh the inherited byte buffer from the current packet words.
        void sync_bytes() {
            _bytes.clear();
            const u8 n = _packet.word_count();
            for (u8 i = 0; i < n; ++i) {
                const u32 w = _packet.word(i);
                _bytes.push_back(static_cast<u8>((w >> 24) & 0xFF));
                _bytes.push_back(static_cast<u8>((w >> 16) & 0xFF));
                _bytes.push_back(static_cast<u8>((w >> 8) & 0xFF));
                _bytes.push_back(static_cast<u8>(w & 0xFF));
            }
        }
    public:
        UmpMessage() = default;

        explicit UmpMessage(UniversalMidiPacket packet):
            _packet{packet} {
            sync_bytes();
        }

        [[nodiscard]]
        const UniversalMidiPacket& packet() const noexcept {
            return _packet;
        }

        [[nodiscard]]
        MessageType type() const noexcept {
            return _packet.type();
        }

        [[nodiscard]]
        u8 group() const noexcept {
            return _packet.group();
        }
    };

    /**
     * @enum UtilityStatus
     * @brief Status field (second nibble of word 0) for Utility messages.
     */
    enum class UtilityStatus: u8 {
        NOOP = 0x0,         ///< No operation; used for padding.
        JR_CLOCK = 0x1,     ///< Jitter-reduction clock.
        JR_TIMESTAMP = 0x2, ///< Jitter-reduction timestamp.
        DELTA_CLOCKSTAMP_TPQ = 0x3, ///< UMP 1.1: Delta Clockstamp ticks per quarter note (16-bit).
        DELTA_CLOCKSTAMP = 0x4,     ///< UMP 1.1: ticks since the previous event (20-bit).
    };

    /**
     * @class UtilityMessage
     * @brief 32-bit Utility packet: NOOP and the two jitter-reduction forms.
     * @extends UmpMessage
     *
     * JR Clock and JR Timestamp both carry a 16-bit sender clock value with a
     * fixed 1/31250 s tick (so the field wraps about every 2.1 s). NOOP carries
     * no payload.
     */
    class UtilityMessage final: public UmpMessage {
        static constexpr u32 encode(UtilityStatus status, u16 value) noexcept {
            return (static_cast<u32>(MessageType::UTILITY) << 28)
                | (static_cast<u32>(status) << 20)
                | static_cast<u32>(value);
        }
    public:
        UtilityMessage() = default;

        explicit UtilityMessage(UniversalMidiPacket packet):
            UmpMessage{packet} {}

        UtilityMessage(UtilityStatus status, u16 value = 0):
            UmpMessage{UniversalMidiPacket{encode(status, value)}} {}

        [[nodiscard]]
        static UtilityMessage noop() {
            return UtilityMessage{UtilityStatus::NOOP};
        }

        [[nodiscard]]
        static UtilityMessage jr_clock(u16 ticks) {
            return UtilityMessage{UtilityStatus::JR_CLOCK, ticks};
        }

        [[nodiscard]]
        static UtilityMessage jr_timestamp(u16 ticks) {
            return UtilityMessage{UtilityStatus::JR_TIMESTAMP, ticks};
        }

        /// Largest tick delta one Delta Clockstamp can carry (20 bits).
        static constexpr u32 MAX_DELTA_TICKS = 0xFFFFF;

        /**
         * @brief Delta Clockstamp Ticks Per Quarter Note: sets the resolution
         * the Delta Clockstamps that follow are counted in.
         * @throws InvalidMidiDataException if @p ticks_per_quarter is 0.
         */
        [[nodiscard]]
        THROWS(InvalidMidiDataException)
        static UtilityMessage delta_clockstamp_tpq(u16 ticks_per_quarter) {
            if (ticks_per_quarter == 0) {
                throw InvalidMidiDataException("ticks per quarter note must be positive");
            }
            return UtilityMessage{UtilityStatus::DELTA_CLOCKSTAMP_TPQ, ticks_per_quarter};
        }

        /**
         * @brief Delta Clockstamp: the ticks elapsed since the previous event.
         * Longer gaps are written as several consecutive clockstamps.
         * @throws InvalidMidiDataException if @p ticks exceeds MAX_DELTA_TICKS.
         */
        [[nodiscard]]
        THROWS(InvalidMidiDataException)
        static UtilityMessage delta_clockstamp(u32 ticks) {
            if (ticks > MAX_DELTA_TICKS) {
                throw InvalidMidiDataException("delta clockstamp exceeds 20 bits");
            }
            return UtilityMessage{UniversalMidiPacket{
                (static_cast<u32>(MessageType::UTILITY) << 28)
                | (static_cast<u32>(UtilityStatus::DELTA_CLOCKSTAMP) << 20)
                | ticks
            }};
        }

        /// The 20-bit tick count of a Delta Clockstamp (0 for other statuses).
        [[nodiscard]]
        u32 delta_ticks() const noexcept {
            return utility_status() == UtilityStatus::DELTA_CLOCKSTAMP
                ? (_packet.word(0) & MAX_DELTA_TICKS)
                : 0;
        }

        [[nodiscard]]
        UtilityStatus utility_status() const noexcept {
            return static_cast<UtilityStatus>((_packet.word(0) >> 20) & 0x0F);
        }

        /// The 16-bit payload (meaningful for the JR forms; 0 for NOOP).
        [[nodiscard]]
        u16 value() const noexcept {
            return static_cast<u16>(_packet.word(0) & 0xFFFF);
        }
    };

    /**
     * @class SystemMessage
     * @brief 32-bit System packet: real-time and system-common, carried as
     * UMP with a group.
     * @extends UmpMessage
     *
     * The status byte reuses @ref MidiStatus (0xF1-0xFF). Word 0 lays out as
     * type|group|status|data1|data2; data bytes are zero for statuses that
     * take none (e.g. Timing Clock, Tune Request).
     */
    class SystemMessage final: public UmpMessage {
        static constexpr u32 encode(u8 group, u8 status, u8 data1, u8 data2) noexcept {
            return (static_cast<u32>(MessageType::SYSTEM) << 28)
                | (static_cast<u32>(group & 0x0F) << 24)
                | (static_cast<u32>(status) << 16)
                | (static_cast<u32>(data1 & 0x7F) << 8)
                | static_cast<u32>(data2 & 0x7F);
        }
    public:
        SystemMessage() = default;

        explicit SystemMessage(UniversalMidiPacket packet):
            UmpMessage{packet} {}

        /**
         * @param group UMP group, 0-15.
         * @param status System status byte, e.g. MidiStatus::TIMING_CLOCK.
         * @param data1 First data byte (0-127); 0 for statuses without data.
         * @param data2 Second data byte (0-127); 0 for statuses without data.
         * @throws InvalidMidiDataException if @p group or a data byte is out of range.
         */
        THROWS(InvalidMidiDataException)
        SystemMessage(u8 group, MidiStatus status, u8 data1 = 0, u8 data2 = 0) {
            if (group > 15) {
                throw InvalidMidiDataException("UMP group out of range");
            }
            if (data1 > 127 || data2 > 127) {
                throw InvalidMidiDataException("system data byte out of range");
            }
            _packet = UniversalMidiPacket{encode(group, static_cast<u8>(status), data1, data2)};
            sync_bytes();
        }

        [[nodiscard]]
        MidiStatus system_status() const noexcept {
            return static_cast<MidiStatus>((_packet.word(0) >> 16) & 0xFF);
        }

        [[nodiscard]]
        u8 data1() const noexcept {
            return static_cast<u8>((_packet.word(0) >> 8) & 0x7F);
        }

        [[nodiscard]]
        u8 data2() const noexcept {
            return static_cast<u8>(_packet.word(0) & 0x7F);
        }
    };

    /**
     * @namespace stdx::audio::midi::scale
     * @brief Resolution-scaling helpers for MIDI 1.0 <-> 2.0 data.
     *
     * MIDI 2.0 widens 7-bit controller / velocity values to 16 or 32 bits.
     * The MIDI 2.0 spec's down/up conversion is not a plain shift: it keeps
     * the minimum, centre, and maximum of the range exact so a centred 1.0
     * value (0x40) maps to the exact 2.0 centre and back without drift.
     */
    namespace scale {
        /**
         * @brief The MIDI 2.0 Min-Center-Max up-scaling algorithm, generalised
         * over source and destination bit widths.
         *
         * Shifts @p value (a @p src_bits-wide field) into the top of a
         * @p dst_bits-wide field. Values at or below the centre are left as
         * the pure shift (so min->min and centre->centre exactly). Values
         * above the centre have their "extended bits" (value minus centre,
         * @p src_bits-1 bits wide) replicated down the remaining low bits, so
         * the maximum fills the destination completely (max->max exactly).
         *
         * @param value Source value, already masked to @p src_bits.
         * @param src_bits Source field width (e.g. 7, 14).
         * @param dst_bits Destination field width (e.g. 16, 32).
         */
        [[nodiscard]]
        constexpr u32 up(u32 value, u32 src_bits, u32 dst_bits) noexcept {
            const u32 bit_shift = dst_bits - src_bits;
            u32 scaled = value << bit_shift;
            const u32 centre = 1u << (src_bits - 1);
            if (value <= centre) {
                return scaled; // bottom half (incl. centre): pure shift.
            }
            // Top half: replicate the extended bits down the remaining field.
            const u32 repeat_bits = src_bits - 1;
            const u32 extended = value - centre;
            u32 repeat_shift = bit_shift;
            while (repeat_shift >= repeat_bits) {
                repeat_shift -= repeat_bits;
                scaled |= extended << repeat_shift;
            }
            if (repeat_shift != 0) {
                scaled |= extended >> (repeat_bits - repeat_shift);
            }
            return scaled;
        }

        /// Widen a 7-bit value to 16 bits (min/centre/max preserving).
        [[nodiscard]]
        constexpr u16 up_7_to_16(u8 v7) noexcept {
            return static_cast<u16>(up(v7 & 0x7F, 7, 16));
        }

        /// Widen a 7-bit value to 32 bits.
        [[nodiscard]]
        constexpr u32 up_7_to_32(u8 v7) noexcept {
            return up(v7 & 0x7F, 7, 32);
        }

        /// Widen a 14-bit (MIDI 1.0 pitch bend) value to 32 bits.
        [[nodiscard]]
        constexpr u32 up_14_to_32(u16 v14) noexcept {
            return up(v14 & 0x3FFF, 14, 32);
        }

        /// Narrow a 16-bit value to 7 bits (truncating the low bits).
        [[nodiscard]]
        constexpr u8 down_16_to_7(u16 v16) noexcept {
            return static_cast<u8>(v16 >> (16 - 7));
        }

        /// Narrow a 32-bit value to 7 bits.
        [[nodiscard]]
        constexpr u8 down_32_to_7(u32 v32) noexcept {
            return static_cast<u8>(v32 >> (32 - 7));
        }

        /// Narrow a 32-bit value to 14 bits (MIDI 1.0 pitch bend).
        [[nodiscard]]
        constexpr u16 down_32_to_14(u32 v32) noexcept {
            return static_cast<u16>(v32 >> (32 - 14));
        }
    }

    /**
     * @class Midi1ChannelVoiceMessage
     * @brief 32-bit UMP carrying a MIDI 1.0 channel-voice message.
     * @extends UmpMessage
     *
     * This is MIDI 1.0 data in MIDI 2.0 clothing: the same 7-bit values and
     * status nibbles, wrapped in a UMP with a group. It is the natural target
     * for up-converting a @ref ShortMessage onto a UMP stream.
     *
     * Word 0 layout: type|group|status(incl. channel)|data1|data2.
     */
    class Midi1ChannelVoiceMessage final: public UmpMessage {
        static constexpr u32 encode(u8 group, u8 status, u8 channel, u8 d1, u8 d2) noexcept {
            return (static_cast<u32>(MessageType::MIDI1_CHANNEL_VOICE) << 28)
                | (static_cast<u32>(group & 0x0F) << 24)
                | (static_cast<u32>((status & 0xF0) | (channel & 0x0F)) << 16)
                | (static_cast<u32>(d1 & 0x7F) << 8)
                | static_cast<u32>(d2 & 0x7F);
        }
    public:
        Midi1ChannelVoiceMessage() = default;

        explicit Midi1ChannelVoiceMessage(UniversalMidiPacket packet):
            UmpMessage{packet} {}

        /**
         * @param group UMP group, 0-15.
         * @param status Channel-voice status WITHOUT channel bits, e.g. MidiStatus::NOTE_ON.
         * @param channel 0-15.
         * @param data1 First data byte (0-127).
         * @param data2 Second data byte (0-127).
         * @throws InvalidMidiDataException on out-of-range group/channel/data.
         */
        THROWS(InvalidMidiDataException)
        Midi1ChannelVoiceMessage(u8 group, MidiStatus status, u8 channel, u8 data1, u8 data2 = 0) {
            if (group > 15 || channel > 15) {
                throw InvalidMidiDataException("UMP group or channel out of range");
            }
            if (data1 > 127 || data2 > 127) {
                throw InvalidMidiDataException("MIDI 1.0 data byte out of range");
            }
            _packet = UniversalMidiPacket{encode(group, static_cast<u8>(status), channel, data1, data2)};
            sync_bytes();
        }

        [[nodiscard]]
        MidiStatus command() const noexcept {
            return static_cast<MidiStatus>((_packet.word(0) >> 16) & 0xF0);
        }

        [[nodiscard]]
        u8 channel() const noexcept {
            return static_cast<u8>((_packet.word(0) >> 16) & 0x0F);
        }

        [[nodiscard]]
        u8 data1() const noexcept {
            return static_cast<u8>((_packet.word(0) >> 8) & 0x7F);
        }

        [[nodiscard]]
        u8 data2() const noexcept {
            return static_cast<u8>(_packet.word(0) & 0x7F);
        }
    };

    /**
     * @class Midi2ChannelVoiceMessage
     * @brief 64-bit UMP carrying a MIDI 2.0 channel-voice message.
     * @extends UmpMessage
     *
     * The high-resolution channel-voice family: 16-bit velocity, 32-bit
     * controller / pitch-bend values, note attributes, and the per-note
     * forms. Word 0 is type|group|status(incl. channel)|index1|index2; word 1
     * carries the wide value. Factory statics build the common messages; the
     * raw constructor is available for the rest.
     */
    class Midi2ChannelVoiceMessage final: public UmpMessage {
        static constexpr u32 encode_w0(u8 group, u8 status, u8 channel, u8 b2, u8 b3) noexcept {
            return (static_cast<u32>(MessageType::MIDI2_CHANNEL_VOICE) << 28)
                | (static_cast<u32>(group & 0x0F) << 24)
                | (static_cast<u32>((status & 0xF0) | (channel & 0x0F)) << 16)
                | (static_cast<u32>(b2) << 8)
                | static_cast<u32>(b3);
        }

        THROWS(InvalidMidiDataException)
        static void check_channel(u8 group, u8 channel) {
            if (group > 15 || channel > 15) {
                throw InvalidMidiDataException("UMP group or channel out of range");
            }
        }
    public:
        Midi2ChannelVoiceMessage() = default;

        explicit Midi2ChannelVoiceMessage(UniversalMidiPacket packet):
            UmpMessage{packet} {}

        /**
         * @brief Raw constructor: supply both words directly.
         * @param group UMP group, 0-15.
         * @param status Status nibble WITHOUT channel, e.g. Midi2Status::NOTE_ON.
         * @param channel 0-15.
         * @param index1 Byte 2 of word 0 (note number, controller index, ...).
         * @param index2 Byte 3 of word 0 (attribute type, reserved, ...).
         * @param value Word 1: the wide data value.
         * @throws InvalidMidiDataException on out-of-range group/channel.
         */
        THROWS(InvalidMidiDataException)
        Midi2ChannelVoiceMessage(u8 group, Midi2Status status, u8 channel, u8 index1, u8 index2, u32 value) {
            check_channel(group, channel);
            _packet = UniversalMidiPacket{
                encode_w0(group, static_cast<u8>(status), channel, index1, index2),
                value
            };
            sync_bytes();
        }

        /**
         * @brief MIDI 2.0 Note On with 16-bit velocity.
         * @param attribute_type Note attribute type (0 = none).
         * @param attribute_data 16-bit attribute payload.
         * @throws InvalidMidiDataException on out-of-range group/channel/note.
         */
        THROWS(InvalidMidiDataException)
        static Midi2ChannelVoiceMessage note_on(
            u8 group, u8 channel, u8 note, u16 velocity,
            u8 attribute_type = 0, u16 attribute_data = 0
        ) {
            check_channel(group, channel);
            if (note > 127) {
                throw InvalidMidiDataException("note number out of range");
            }
            const u32 value = (static_cast<u32>(velocity) << 16) | attribute_data;
            return Midi2ChannelVoiceMessage{
                UniversalMidiPacket{
                    encode_w0(group, static_cast<u8>(Midi2Status::NOTE_ON), channel, note, attribute_type),
                    value
                }
            };
        }

        /**
         * @brief MIDI 2.0 Note Off with 16-bit velocity.
         * @throws InvalidMidiDataException on out-of-range group/channel/note.
         */
        THROWS(InvalidMidiDataException)
        static Midi2ChannelVoiceMessage note_off(
            u8 group, u8 channel, u8 note, u16 velocity,
            u8 attribute_type = 0, u16 attribute_data = 0
        ) {
            check_channel(group, channel);
            if (note > 127) {
                throw InvalidMidiDataException("note number out of range");
            }
            const u32 value = (static_cast<u32>(velocity) << 16) | attribute_data;
            return Midi2ChannelVoiceMessage{
                UniversalMidiPacket{
                    encode_w0(group, static_cast<u8>(Midi2Status::NOTE_OFF), channel, note, attribute_type),
                    value
                }
            };
        }

        /**
         * @brief MIDI 2.0 Control Change with a 32-bit value.
         * @throws InvalidMidiDataException on out-of-range group/channel/controller.
         */
        THROWS(InvalidMidiDataException)
        static Midi2ChannelVoiceMessage control_change(u8 group, u8 channel, u8 controller, u32 value) {
            check_channel(group, channel);
            if (controller > 127) {
                throw InvalidMidiDataException("controller index out of range");
            }
            return Midi2ChannelVoiceMessage{
                UniversalMidiPacket{
                    encode_w0(group, static_cast<u8>(Midi2Status::CONTROL_CHANGE), channel, controller, 0),
                    value
                }
            };
        }

        /**
         * @brief MIDI 2.0 Pitch Bend with a 32-bit value (0x80000000 = centre).
         * @throws InvalidMidiDataException on out-of-range group/channel.
         */
        THROWS(InvalidMidiDataException)
        static Midi2ChannelVoiceMessage pitch_bend(u8 group, u8 channel, u32 value) {
            check_channel(group, channel);
            return Midi2ChannelVoiceMessage{
                UniversalMidiPacket{
                    encode_w0(group, static_cast<u8>(Midi2Status::PITCH_BEND), channel, 0, 0),
                    value
                }
            };
        }

        /**
         * @brief MIDI 2.0 Poly Pressure (per-note aftertouch) with a 32-bit value.
         * @throws InvalidMidiDataException on out-of-range group/channel/note.
         */
        THROWS(InvalidMidiDataException)
        static Midi2ChannelVoiceMessage poly_pressure(u8 group, u8 channel, u8 note, u32 value) {
            check_channel(group, channel);
            if (note > 127) {
                throw InvalidMidiDataException("note number out of range");
            }
            return Midi2ChannelVoiceMessage{
                UniversalMidiPacket{
                    encode_w0(group, static_cast<u8>(Midi2Status::POLY_PRESSURE), channel, note, 0),
                    value
                }
            };
        }

        /**
         * @brief MIDI 2.0 Channel Pressure with a 32-bit value.
         * @throws InvalidMidiDataException on out-of-range group/channel.
         */
        THROWS(InvalidMidiDataException)
        static Midi2ChannelVoiceMessage channel_pressure(u8 group, u8 channel, u32 value) {
            check_channel(group, channel);
            return Midi2ChannelVoiceMessage{
                UniversalMidiPacket{
                    encode_w0(group, static_cast<u8>(Midi2Status::CHANNEL_PRESSURE), channel, 0, 0),
                    value
                }
            };
        }

        /**
         * @brief MIDI 2.0 Program Change, optionally selecting a bank at the
         * same time.
         *
         * MIDI 2.0 folds MIDI 1.0's Bank Select MSB/LSB (CC 0 / CC 32) into the
         * program change itself. Bit 0 of byte 3 is the Bank Valid flag; word 1
         * is [program][reserved][bank MSB][bank LSB].
         *
         * @param bank_msb Bank MSB (0-127), or empty for no bank change.
         * @param bank_lsb Bank LSB (0-127); ignored when @p bank_msb is empty.
         * @throws InvalidMidiDataException on out-of-range group/channel/program/bank.
         */
        THROWS(InvalidMidiDataException)
        static Midi2ChannelVoiceMessage program_change(
            u8 group, u8 channel, u8 program,
            Optional<u8> bank_msb = {}, u8 bank_lsb = 0
        ) {
            check_channel(group, channel);
            if (program > 127) {
                throw InvalidMidiDataException("program number out of range");
            }
            if (bank_msb.has_value() && (*bank_msb > 127 || bank_lsb > 127)) {
                throw InvalidMidiDataException("bank number out of range");
            }
            const u8 flags = bank_msb.has_value() ? 0x01 : 0x00;
            const u32 value = (static_cast<u32>(program) << 24)
                | (bank_msb.has_value()
                    ? (static_cast<u32>(*bank_msb) << 8) | static_cast<u32>(bank_lsb)
                    : 0u);
            return Midi2ChannelVoiceMessage{
                UniversalMidiPacket{
                    encode_w0(group, static_cast<u8>(Midi2Status::PROGRAM_CHANGE), channel, 0, flags),
                    value
                }
            };
        }

        /**
         * @brief MIDI 2.0 Registered Controller (the successor to RPN) with a
         * 32-bit value.
         * @param bank The controller bank (RPN MSB, CC 101), 0-127.
         * @param index The controller index (RPN LSB, CC 100), 0-127.
         * @throws InvalidMidiDataException on out-of-range group/channel/bank/index.
         */
        THROWS(InvalidMidiDataException)
        static Midi2ChannelVoiceMessage registered_controller(u8 group, u8 channel, u8 bank, u8 index, u32 value) {
            check_channel(group, channel);
            if (bank > 127 || index > 127) {
                throw InvalidMidiDataException("controller bank or index out of range");
            }
            return Midi2ChannelVoiceMessage{
                UniversalMidiPacket{
                    encode_w0(group, static_cast<u8>(Midi2Status::REGISTERED_CONTROLLER), channel, bank, index),
                    value
                }
            };
        }

        /**
         * @brief MIDI 2.0 Assignable Controller (the successor to NRPN) with a
         * 32-bit value.
         * @param bank The controller bank (NRPN MSB, CC 99), 0-127.
         * @param index The controller index (NRPN LSB, CC 98), 0-127.
         * @throws InvalidMidiDataException on out-of-range group/channel/bank/index.
         */
        THROWS(InvalidMidiDataException)
        static Midi2ChannelVoiceMessage assignable_controller(u8 group, u8 channel, u8 bank, u8 index, u32 value) {
            check_channel(group, channel);
            if (bank > 127 || index > 127) {
                throw InvalidMidiDataException("controller bank or index out of range");
            }
            return Midi2ChannelVoiceMessage{
                UniversalMidiPacket{
                    encode_w0(group, static_cast<u8>(Midi2Status::ASSIGNABLE_CONTROLLER), channel, bank, index),
                    value
                }
            };
        }

        /// Program Change: the program number (top byte of the value word).
        [[nodiscard]]
        u8 program() const noexcept {
            return static_cast<u8>((_packet.word(1) >> 24) & 0x7F);
        }

        /// Program Change: whether the message also selects a bank.
        [[nodiscard]]
        bool bank_valid() const noexcept {
            return (_packet.word(0) & 0x01) != 0;
        }

        /// Program Change: the bank MSB (meaningful only when bank_valid()).
        [[nodiscard]]
        u8 bank_msb() const noexcept {
            return static_cast<u8>((_packet.word(1) >> 8) & 0x7F);
        }

        /// Program Change: the bank LSB (meaningful only when bank_valid()).
        [[nodiscard]]
        u8 bank_lsb() const noexcept {
            return static_cast<u8>(_packet.word(1) & 0x7F);
        }

        [[nodiscard]]
        Midi2Status command() const noexcept {
            return static_cast<Midi2Status>((_packet.word(0) >> 16) & 0xF0);
        }

        [[nodiscard]]
        u8 channel() const noexcept {
            return static_cast<u8>((_packet.word(0) >> 16) & 0x0F);
        }

        /// Byte 2 of word 0: note number, controller index, etc.
        [[nodiscard]]
        u8 index1() const noexcept {
            return static_cast<u8>((_packet.word(0) >> 8) & 0xFF);
        }

        /// Byte 3 of word 0: attribute type, reserved, etc.
        [[nodiscard]]
        u8 index2() const noexcept {
            return static_cast<u8>(_packet.word(0) & 0xFF);
        }

        /// The full 32-bit value word.
        [[nodiscard]]
        u32 value() const noexcept {
            return _packet.word(1);
        }

        /// 16-bit velocity (high half of the value word) for Note On/Off.
        [[nodiscard]]
        u16 velocity() const noexcept {
            return static_cast<u16>(_packet.word(1) >> 16);
        }

        /// 16-bit note attribute data (low half of the value word).
        [[nodiscard]]
        u16 attribute_data() const noexcept {
            return static_cast<u16>(_packet.word(1) & 0xFFFF);
        }
    };

    /**
     * @class MidiTranslator
     * @brief Default MIDI 1.0 <-> 2.0 translation.
     *
     * Implements the spec's default, stateless translation for every
     * channel-voice message that has a direct counterpart. Two rules keep note
     * semantics intact across the velocity resolution change:
     *
     * - a MIDI 1.0 Note On with velocity 0 means Note Off, so it up-converts
     *   to a MIDI 2.0 Note Off;
     * - a MIDI 2.0 Note On whose 16-bit velocity would narrow to 0 narrows to
     *   1 instead, since 0 would turn it into a Note Off on MIDI 1.0.
     *
     * Translations that need state across messages - Bank Select before a
     * Program Change, RPN/NRPN parameter selection before Data Entry - live in
     * @ref Midi1ToMidi2Translator for the up direction, and in
     * @ref to_midi1_messages (which expands to several MIDI 1.0 messages) for
     * the down direction.
     */
    class MidiTranslator final {
    public:
        MidiTranslator() = DELETE_METHOD("MidiTranslator is a static utility class and cannot be instantiated.");

        /**
         * @brief Up-convert a MIDI 1.0 @ref ShortMessage to a 64-bit MIDI 2.0
         * channel-voice message on the given group.
         *
         * Program Change is translated without a bank (Bank Select arrives as
         * separate CCs, which need @ref Midi1ToMidi2Translator to fold in).
         *
         * @return The translated message, or empty if the status has no
         * MIDI 2.0 channel-voice counterpart (e.g. system messages).
         */
        [[nodiscard]]
        THROWS(InvalidMidiDataException)
        static Optional<Midi2ChannelVoiceMessage> to_midi2(const ShortMessage& msg, u8 group = 0) {
            const u8 channel = msg.channel();
            switch (msg.command()) {
                case MidiStatus::NOTE_ON:
                    if (msg.data2() == 0) {
                        // Velocity 0 is MIDI 1.0's running-status Note Off.
                        return Midi2ChannelVoiceMessage::note_off(group, channel, msg.data1(), 0);
                    }
                    return Midi2ChannelVoiceMessage::note_on(
                        group, channel, msg.data1(), scale::up_7_to_16(msg.data2())
                    );
                case MidiStatus::NOTE_OFF:
                    return Midi2ChannelVoiceMessage::note_off(
                        group, channel, msg.data1(), scale::up_7_to_16(msg.data2())
                    );
                case MidiStatus::POLY_KEY_PRESSURE:
                    return Midi2ChannelVoiceMessage::poly_pressure(
                        group, channel, msg.data1(), scale::up_7_to_32(msg.data2())
                    );
                case MidiStatus::CONTROL_CHANGE:
                    return Midi2ChannelVoiceMessage::control_change(
                        group, channel, msg.data1(), scale::up_7_to_32(msg.data2())
                    );
                case MidiStatus::PROGRAM_CHANGE:
                    return Midi2ChannelVoiceMessage::program_change(group, channel, msg.data1());
                case MidiStatus::CHANNEL_PRESSURE:
                    return Midi2ChannelVoiceMessage::channel_pressure(
                        group, channel, scale::up_7_to_32(msg.data1())
                    );
                case MidiStatus::PITCH_BEND: {
                    const u16 bend14 = static_cast<u16>((msg.data1() & 0x7F) | ((msg.data2() & 0x7F) << 7));
                    return Midi2ChannelVoiceMessage::pitch_bend(group, channel, scale::up_14_to_32(bend14));
                }
                default:
                    return {};
            }
        }

        /**
         * @brief Down-convert a 64-bit MIDI 2.0 channel-voice message to a
         * single MIDI 1.0 @ref ShortMessage.
         *
         * A Program Change comes back without its bank; use
         * @ref to_midi1_messages to get the Bank Select CCs as well.
         *
         * @return The translated message, or empty if the MIDI 2.0 message has
         * no single-message MIDI 1.0 counterpart (per-note controllers,
         * per-note pitch bend, registered/assignable controllers, note
         * management).
         */
        [[nodiscard]]
        THROWS(InvalidMidiDataException)
        static Optional<ShortMessage> to_midi1(const Midi2ChannelVoiceMessage& msg) {
            const u8 channel = msg.channel();
            switch (msg.command()) {
                case Midi2Status::NOTE_ON: {
                    // Never let a sounding note narrow into a velocity-0 Note Off.
                    const u8 velocity = scale::down_16_to_7(msg.velocity());
                    return ShortMessage{
                        MidiStatus::NOTE_ON, channel, msg.index1(), velocity == 0 ? u8{1} : velocity
                    };
                }
                case Midi2Status::NOTE_OFF:
                    return ShortMessage{
                        MidiStatus::NOTE_OFF, channel, msg.index1(), scale::down_16_to_7(msg.velocity())
                    };
                case Midi2Status::POLY_PRESSURE:
                    return ShortMessage{
                        MidiStatus::POLY_KEY_PRESSURE, channel, msg.index1(), scale::down_32_to_7(msg.value())
                    };
                case Midi2Status::CONTROL_CHANGE:
                    return ShortMessage{
                        MidiStatus::CONTROL_CHANGE, channel, msg.index1(), scale::down_32_to_7(msg.value())
                    };
                case Midi2Status::PROGRAM_CHANGE:
                    return ShortMessage{MidiStatus::PROGRAM_CHANGE, channel, msg.program()};
                case Midi2Status::CHANNEL_PRESSURE:
                    return ShortMessage{
                        MidiStatus::CHANNEL_PRESSURE, channel, scale::down_32_to_7(msg.value())
                    };
                case Midi2Status::PITCH_BEND: {
                    const u16 bend14 = scale::down_32_to_14(msg.value());
                    return ShortMessage{
                        MidiStatus::PITCH_BEND, channel,
                        static_cast<u8>(bend14 & 0x7F), static_cast<u8>((bend14 >> 7) & 0x7F)
                    };
                }
                default:
                    return {};
            }
        }

        /**
         * @brief Down-convert a MIDI 2.0 channel-voice message to the full
         * sequence of MIDI 1.0 messages that expresses it.
         *
         * Like @ref to_midi1, but also covers the messages MIDI 1.0 can only
         * say in several steps:
         *
         * - Program Change with Bank Valid -> CC 0, CC 32, Program Change;
         * - Registered Controller -> CC 101, CC 100, CC 6, CC 38 (RPN);
         * - Assignable Controller -> CC 99, CC 98, CC 6, CC 38 (NRPN).
         *
         * The 32-bit controller value narrows to 14 bits split across Data
         * Entry MSB (CC 6) and LSB (CC 38).
         *
         * @return The messages in send order; empty if there is no MIDI 1.0
         * form at all.
         */
        [[nodiscard]]
        THROWS(InvalidMidiDataException)
        static Vector<ShortMessage> to_midi1_messages(const Midi2ChannelVoiceMessage& msg) {
            Vector<ShortMessage> out;
            const u8 channel = msg.channel();
            const auto cc = [&out, channel](u8 controller, u8 value) -> void {
                out.push_back(ShortMessage{MidiStatus::CONTROL_CHANGE, channel, controller, value});
            };
            const auto parameter = [&cc, &msg](u8 msb_cc, u8 lsb_cc) -> void {
                const u16 value14 = scale::down_32_to_14(msg.value());
                cc(msb_cc, static_cast<u8>(msg.index1() & 0x7F));
                cc(lsb_cc, static_cast<u8>(msg.index2() & 0x7F));
                cc(6, static_cast<u8>((value14 >> 7) & 0x7F));
                cc(38, static_cast<u8>(value14 & 0x7F));
            };

            switch (msg.command()) {
                case Midi2Status::PROGRAM_CHANGE:
                    if (msg.bank_valid()) {
                        cc(0, msg.bank_msb());
                        cc(32, msg.bank_lsb());
                    }
                    out.push_back(ShortMessage{MidiStatus::PROGRAM_CHANGE, channel, msg.program()});
                    return out;
                case Midi2Status::REGISTERED_CONTROLLER:
                    parameter(101, 100);
                    return out;
                case Midi2Status::ASSIGNABLE_CONTROLLER:
                    parameter(99, 98);
                    return out;
                default:
                    if (Optional<ShortMessage> single = to_midi1(msg); single.has_value()) {
                        out.push_back(Ops::move(*single));
                    }
                    return out;
            }
        }

        /**
         * @brief Whether @p msg is a UMP-family message (any @ref UmpMessage,
         * @ref Sysex7Message, or @ref Sysex8Message) rather than a MIDI 1.0
         * byte-stream message.
         */
        [[nodiscard]]
        static bool is_ump(const MidiMessage& msg) noexcept;

        /**
         * @brief Down-convert any UMP-family message to MIDI 1.0 byte-stream
         * messages, for a MIDI 1.0 transport.
         *
         * Channel voice (either protocol), system, and SysEx7 messages
         * translate; a fragment of a multi-packet SysEx7 cannot on its own (feed
         * packets through @ref UmpParser to reassemble them first). Utility,
         * Flex Data, Stream, SysEx8 and reserved packets have no MIDI 1.0 wire
         * form and yield nothing.
         *
         * @param msg A UMP-family message; a MIDI 1.0 message is returned as-is.
         * @return Each complete MIDI 1.0 message's bytes, in send order.
         */
        [[nodiscard]]
        THROWS(InvalidMidiDataException)
        static Vector<Vector<u8>> to_midi1_bytes(const MidiMessage& msg);

        /**
         * @brief The number of data bytes that follow a system status byte
         * (0xF1-0xFF) on a MIDI 1.0 wire.
         */
        [[nodiscard]]
        static constexpr u8 system_data_length(u8 status) noexcept {
            switch (status) {
                case 0xF1: // MIDI Time Code quarter frame
                case 0xF3: // Song Select
                    return 1;
                case 0xF2: // Song Position Pointer
                    return 2;
                default:
                    return 0;
            }
        }
    };

    /**
     * @class Midi1ToMidi2Translator
     * @brief Stateful MIDI 1.0 -> 2.0 channel-voice translation.
     *
     * Some MIDI 1.0 meanings span several messages, which a stateless
     * translator cannot see:
     *
     * - **Bank Select.** CC 0 (MSB) and CC 32 (LSB) are remembered per
     *   channel and folded into the next Program Change as its bank. As in
     *   MIDI 1.0 the selected bank persists across later program changes.
     * - **RPN / NRPN.** CC 101/100 (RPN) or CC 99/98 (NRPN) select a
     *   parameter; Data Entry CC 6 (MSB) and CC 38 (LSB) then set it. Each
     *   Data Entry emits a MIDI 2.0 Registered / Assignable Controller with
     *   the 14-bit value widened to 32 bits: CC 6 emits with the LSB taken as
     *   0, so a sender that only ever sends the MSB still gets through, and a
     *   following CC 38 emits again with the full value. Selecting the null
     *   parameter (127/127) ends the selection.
     *
     * The CCs that only carry this state (0, 32, 98-101, and 6/38 while a
     * parameter is selected) are consumed and produce nothing. Everything
     * else goes through @ref MidiTranslator::to_midi2. State is kept per
     * group and channel.
     */
    class Midi1ToMidi2Translator final {
    private:
        /**
         * @internal
         * @brief What one MIDI 1.0 channel has said so far.
         */
        struct ChannelState {
            enum class Parameter: u8 { NONE, REGISTERED, ASSIGNABLE };

            Optional<u8> bank_msb; ///< CC 0, once seen.
            u8 bank_lsb = 0; ///< CC 32; 0 until seen.
            Parameter parameter = Parameter::NONE; ///< Which kind of parameter CC 6/38 addresses.
            u8 parameter_msb = 127; ///< CC 101 or CC 99.
            u8 parameter_lsb = 127; ///< CC 100 or CC 98.
            u8 data_msb = 0; ///< CC 6, the last Data Entry MSB.
        };

        /// One per group and channel, at group * 16 + channel. A Vector rather
        /// than a ChannelState[16][16] member: GCC 16 mis-reads that nested
        /// array type back out of the module and ICEs (in complete_array_type)
        /// in any TU that imports it and constructs a translator.
        Vector<ChannelState> _state;

        [[nodiscard]]
        ChannelState& state_for(u8 group, u8 channel) noexcept {
            return _state[static_cast<usize>(group) * 16 + channel];
        }

        /**
         * @internal
         * @brief Record a parameter-number CC, ending the selection on the
         * null parameter.
         */
        static void select(ChannelState& state, ChannelState::Parameter kind, bool msb, u8 value) noexcept {
            if (state.parameter != kind) {
                // Switching between RPN and NRPN starts a fresh selection.
                state.parameter_msb = 127;
                state.parameter_lsb = 127;
            }
            (msb ? state.parameter_msb : state.parameter_lsb) = value;
            state.parameter = (state.parameter_msb == 127 && state.parameter_lsb == 127)
                ? ChannelState::Parameter::NONE
                : kind;
        }

        /**
         * @internal
         * @brief The controller message for the selected parameter's value.
         */
        [[nodiscard]]
        static Midi2ChannelVoiceMessage parameter_value(const ChannelState& state, u8 group, u8 channel, u8 lsb) {
            const u32 value = scale::up_14_to_32(static_cast<u16>((state.data_msb << 7) | (lsb & 0x7F)));
            return state.parameter == ChannelState::Parameter::REGISTERED
                ? Midi2ChannelVoiceMessage::registered_controller(
                    group, channel, state.parameter_msb, state.parameter_lsb, value
                )
                : Midi2ChannelVoiceMessage::assignable_controller(
                    group, channel, state.parameter_msb, state.parameter_lsb, value
                );
        }
    public:
        Midi1ToMidi2Translator():
            _state(16 * 16) {}

        /**
         * @brief Translate one MIDI 1.0 message, updating the channel's state.
         * @param msg The MIDI 1.0 channel-voice message.
         * @param group The UMP group to emit on, 0-15.
         * @return The MIDI 2.0 message, or empty if @p msg only updated state
         * or has no MIDI 2.0 counterpart.
         * @throws InvalidMidiDataException on an out-of-range group.
         */
        [[nodiscard]]
        THROWS(InvalidMidiDataException)
        Optional<Midi2ChannelVoiceMessage> translate(const ShortMessage& msg, u8 group = 0) {
            if (group > 15) {
                throw InvalidMidiDataException("UMP group out of range");
            }
            const u8 channel = msg.channel();
            ChannelState& state = state_for(group, channel);

            if (msg.command() == MidiStatus::PROGRAM_CHANGE) {
                return Midi2ChannelVoiceMessage::program_change(
                    group, channel, msg.data1(), state.bank_msb, state.bank_lsb
                );
            }
            if (msg.command() != MidiStatus::CONTROL_CHANGE) {
                return MidiTranslator::to_midi2(msg, group);
            }

            using Parameter = ChannelState::Parameter;
            const u8 value = msg.data2();
            switch (msg.data1()) {
                case 0:
                    state.bank_msb = value;
                    return {};
                case 32:
                    state.bank_lsb = value;
                    return {};
                case 101:
                    select(state, Parameter::REGISTERED, true, value);
                    return {};
                case 100:
                    select(state, Parameter::REGISTERED, false, value);
                    return {};
                case 99:
                    select(state, Parameter::ASSIGNABLE, true, value);
                    return {};
                case 98:
                    select(state, Parameter::ASSIGNABLE, false, value);
                    return {};
                case 6:
                    if (state.parameter == Parameter::NONE) {
                        break;
                    }
                    state.data_msb = value;
                    return parameter_value(state, group, channel, 0);
                case 38:
                    if (state.parameter == Parameter::NONE) {
                        break;
                    }
                    return parameter_value(state, group, channel, value);
                default:
                    break;
            }
            return MidiTranslator::to_midi2(msg, group);
        }

        /**
         * @brief Forget every channel's bank and parameter selection.
         */
        void reset() noexcept {
            for (ChannelState& state: _state) {
                state = ChannelState{};
            }
        }
    };

    /**
     * @enum SysexStatus
     * @brief The packet-role nibble shared by SysEx7 and SysEx8.
     *
     * A logical System Exclusive message is carried as a run of UMP packets.
     * Each packet declares its role: a message that fits in one packet is
     * COMPLETE; a longer one is START, zero or more CONTINUE, then END.
     */
    enum class SysexStatus: u8 {
        COMPLETE = 0x0, ///< The whole SysEx fits in this single packet.
        START = 0x1,    ///< First packet of a multi-packet SysEx.
        CONTINUE = 0x2, ///< A middle packet.
        END = 0x3,      ///< Last packet of a multi-packet SysEx.
    };

    /**
     * @class Sysex7Message
     * @brief A System Exclusive message carried as a run of 64-bit SysEx7 UMP
     * packets (Message Type 0x3).
     *
     * SysEx7 carries 7-bit data, six bytes per packet. This is the UMP
     * transport for classic MIDI 1.0 SysEx content: the payload here is the
     * body WITHOUT the 0xF0 / 0xF7 wrappers, which UMP does not transmit.
     *
     * Unlike the single-packet messages, a Sysex7Message owns a vector of
     * packets; @c data() concatenates them in wire order so a byte-stream
     * Receiver can still forward the whole thing.
     */
    class Sysex7Message final: public MidiMessage {
        static constexpr u8 MAX_BYTES_PER_PACKET = 6;

        Vector<UniversalMidiPacket> _packets;

        static UniversalMidiPacket encode(u8 group, SysexStatus status, Span<const u8> chunk) noexcept {
            const u8 n = static_cast<u8>(chunk.size());
            u32 w0 = (static_cast<u32>(MessageType::DATA_64) << 28)
                | (static_cast<u32>(group & 0x0F) << 24)
                | (static_cast<u32>(status) << 20)
                | (static_cast<u32>(n & 0x0F) << 16);
            // Bytes 2-3 of word 0 are the first two data bytes.
            if (n > 0) { w0 |= static_cast<u32>(chunk[0] & 0x7F) << 8; }
            if (n > 1) { w0 |= static_cast<u32>(chunk[1] & 0x7F); }
            u32 w1 = 0;
            for (u8 i = 2; i < n; ++i) {
                w1 |= static_cast<u32>(chunk[i] & 0x7F) << (24 - (i - 2) * 8);
            }
            return UniversalMidiPacket{w0, w1};
        }

        void sync_bytes() {
            _bytes.clear();
            for (const UniversalMidiPacket& p: _packets) {
                for (u8 i = 0; i < p.word_count(); ++i) {
                    const u32 w = p.word(i);
                    _bytes.push_back(static_cast<u8>((w >> 24) & 0xFF));
                    _bytes.push_back(static_cast<u8>((w >> 16) & 0xFF));
                    _bytes.push_back(static_cast<u8>((w >> 8) & 0xFF));
                    _bytes.push_back(static_cast<u8>(w & 0xFF));
                }
            }
        }
    public:
        Sysex7Message() = default;

        /**
         * @brief Pack a 7-bit SysEx body into one or more SysEx7 packets.
         * @param group UMP group, 0-15.
         * @param payload The SysEx body, excluding the 0xF0/0xF7 wrappers.
         * @throws InvalidMidiDataException on an out-of-range group or a data
         * byte with its high bit set (SysEx7 is 7-bit only).
         */
        THROWS(InvalidMidiDataException)
        static Sysex7Message from_bytes(u8 group, Span<const u8> payload) {
            if (group > 15) {
                throw InvalidMidiDataException("UMP group out of range");
            }
            for (const u8 b: payload) {
                if (b > 0x7F) {
                    throw InvalidMidiDataException("SysEx7 data byte is not 7-bit");
                }
            }
            Sysex7Message msg;
            const usize total = payload.size();
            if (total <= MAX_BYTES_PER_PACKET) {
                msg._packets.push_back(encode(group, SysexStatus::COMPLETE, payload));
            } else {
                usize offset = 0;
                bool first = true;
                while (offset < total) {
                    const usize remaining = total - offset;
                    const u8 take = static_cast<u8>(
                        remaining < MAX_BYTES_PER_PACKET ? remaining : MAX_BYTES_PER_PACKET
                    );
                    const bool last = (offset + take) >= total;
                    const SysexStatus status = first
                        ? SysexStatus::START
                        : (last ? SysexStatus::END : SysexStatus::CONTINUE);
                    msg._packets.push_back(
                        encode(group, status, Span<const u8>{payload.data() + offset, take})
                    );
                    offset += take;
                    first = false;
                }
            }
            msg.sync_bytes();
            return msg;
        }

        /**
         * @brief Wrap an already-packetised SysEx7 run, as received off a UMP
         * stream (see @ref UmpParser, which assembles the run).
         * @param packets The run's 64-bit SysEx7 packets, in order.
         */
        [[nodiscard]]
        static Sysex7Message from_packets(Vector<UniversalMidiPacket> packets) {
            Sysex7Message msg;
            msg._packets = Ops::move(packets);
            msg.sync_bytes();
            return msg;
        }

        [[nodiscard]]
        const Vector<UniversalMidiPacket>& packets() const noexcept {
            return _packets;
        }

        [[nodiscard]]
        u8 group() const noexcept {
            return _packets.empty() ? 0 : static_cast<u8>((_packets[0].word(0) >> 24) & 0x0F);
        }

        /**
         * @brief Reassemble the 7-bit SysEx body from all packets.
         */
        [[nodiscard]]
        Vector<u8> payload() const {
            Vector<u8> out;
            for (const UniversalMidiPacket& p: _packets) {
                const u8 n = static_cast<u8>((p.word(0) >> 16) & 0x0F);
                for (u8 i = 0; i < n; ++i) {
                    u8 byte;
                    if (i == 0) {
                        byte = static_cast<u8>((p.word(0) >> 8) & 0x7F);
                    } else if (i == 1) {
                        byte = static_cast<u8>(p.word(0) & 0x7F);
                    } else {
                        byte = static_cast<u8>((p.word(1) >> (24 - (i - 2) * 8)) & 0x7F);
                    }
                    out.push_back(byte);
                }
            }
            return out;
        }
    };

    /**
     * @class Sysex8Message
     * @brief A System Exclusive message carried as a run of 128-bit SysEx8 UMP
     * packets (Message Type 0x5).
     *
     * SysEx8 carries full 8-bit data, thirteen bytes per packet, and tags each
     * run with an 8-bit Stream ID so multiple interleaved SysEx8 streams can
     * share a group. Payload bytes are unrestricted (0-255).
     */
    class Sysex8Message final: public MidiMessage {
        static constexpr u8 MAX_BYTES_PER_PACKET = 13;

        Vector<UniversalMidiPacket> _packets;

        static UniversalMidiPacket encode(u8 group, SysexStatus status, u8 stream_id, Span<const u8> chunk) noexcept {
            // n counts the stream-id byte plus the data bytes, per the spec.
            const u8 n = static_cast<u8>(chunk.size() + 1);
            u8 b[16] = {0};
            b[0] = static_cast<u8>((static_cast<u8>(MessageType::DATA_128) << 4) | (group & 0x0F));
            b[1] = static_cast<u8>((static_cast<u8>(status) << 4) | (n & 0x0F));
            b[2] = stream_id;
            for (usize i = 0; i < chunk.size(); ++i) {
                b[3 + i] = chunk[i];
            }
            const u32 w0 = (static_cast<u32>(b[0]) << 24) | (static_cast<u32>(b[1]) << 16)
                | (static_cast<u32>(b[2]) << 8) | static_cast<u32>(b[3]);
            const u32 w1 = (static_cast<u32>(b[4]) << 24) | (static_cast<u32>(b[5]) << 16)
                | (static_cast<u32>(b[6]) << 8) | static_cast<u32>(b[7]);
            const u32 w2 = (static_cast<u32>(b[8]) << 24) | (static_cast<u32>(b[9]) << 16)
                | (static_cast<u32>(b[10]) << 8) | static_cast<u32>(b[11]);
            const u32 w3 = (static_cast<u32>(b[12]) << 24) | (static_cast<u32>(b[13]) << 16)
                | (static_cast<u32>(b[14]) << 8) | static_cast<u32>(b[15]);
            return UniversalMidiPacket{w0, w1, w2, w3};
        }

        void sync_bytes() {
            _bytes.clear();
            for (const UniversalMidiPacket& p: _packets) {
                for (u8 i = 0; i < p.word_count(); ++i) {
                    const u32 w = p.word(i);
                    _bytes.push_back(static_cast<u8>((w >> 24) & 0xFF));
                    _bytes.push_back(static_cast<u8>((w >> 16) & 0xFF));
                    _bytes.push_back(static_cast<u8>((w >> 8) & 0xFF));
                    _bytes.push_back(static_cast<u8>(w & 0xFF));
                }
            }
        }

        [[nodiscard]]
        static u8 byte_at(const UniversalMidiPacket& p, u8 index) noexcept {
            const u32 w = p.word(index / 4);
            const u8 shift = static_cast<u8>(24 - (index % 4) * 8);
            return static_cast<u8>((w >> shift) & 0xFF);
        }
    public:
        Sysex8Message() = default;

        /**
         * @brief Pack an 8-bit SysEx body into one or more SysEx8 packets.
         * @param group UMP group, 0-15.
         * @param stream_id Stream identifier tagging this SysEx8 run.
         * @param payload The SysEx body (arbitrary 8-bit bytes).
         * @throws InvalidMidiDataException on an out-of-range group.
         */
        THROWS(InvalidMidiDataException)
        static Sysex8Message from_bytes(u8 group, u8 stream_id, Span<const u8> payload) {
            if (group > 15) {
                throw InvalidMidiDataException("UMP group out of range");
            }
            Sysex8Message msg;
            const usize total = payload.size();
            if (total <= MAX_BYTES_PER_PACKET) {
                msg._packets.push_back(encode(group, SysexStatus::COMPLETE, stream_id, payload));
            } else {
                usize offset = 0;
                bool first = true;
                while (offset < total) {
                    const usize remaining = total - offset;
                    const u8 take = static_cast<u8>(
                        remaining < MAX_BYTES_PER_PACKET ? remaining : MAX_BYTES_PER_PACKET
                    );
                    const bool last = (offset + take) >= total;
                    const SysexStatus status = first
                        ? SysexStatus::START
                        : (last ? SysexStatus::END : SysexStatus::CONTINUE);
                    msg._packets.push_back(
                        encode(group, status, stream_id, Span<const u8>{payload.data() + offset, take})
                    );
                    offset += take;
                    first = false;
                }
            }
            msg.sync_bytes();
            return msg;
        }

        /**
         * @brief Wrap an already-packetised SysEx8 run, as received off a UMP
         * stream (see @ref UmpParser, which assembles the run).
         * @param packets The run's 128-bit SysEx8 packets, in order, all with
         * the same stream id.
         */
        [[nodiscard]]
        static Sysex8Message from_packets(Vector<UniversalMidiPacket> packets) {
            Sysex8Message msg;
            msg._packets = Ops::move(packets);
            msg.sync_bytes();
            return msg;
        }

        [[nodiscard]]
        const Vector<UniversalMidiPacket>& packets() const noexcept {
            return _packets;
        }

        [[nodiscard]]
        u8 group() const noexcept {
            return _packets.empty() ? 0 : static_cast<u8>((_packets[0].word(0) >> 24) & 0x0F);
        }

        [[nodiscard]]
        u8 stream_id() const noexcept {
            return _packets.empty() ? 0 : byte_at(_packets[0], 2);
        }

        /**
         * @brief Reassemble the 8-bit SysEx body from all packets.
         */
        [[nodiscard]]
        Vector<u8> payload() const {
            Vector<u8> out;
            for (const UniversalMidiPacket& p: _packets) {
                // Low nibble of byte 1 is n: stream-id byte + data bytes.
                const u8 n = static_cast<u8>(byte_at(p, 1) & 0x0F);
                // Data starts at byte 3 (byte 2 is the stream id).
                for (u8 i = 1; i < n; ++i) {
                    out.push_back(byte_at(p, static_cast<u8>(2 + i)));
                }
            }
            return out;
        }
    };

    /**
     * @class MixedDataSetHeader
     * @brief 128-bit Mixed Data Set header packet (Message Type 0x5, status 0x8).
     * @extends UmpMessage
     *
     * A Mixed Data Set moves large, arbitrary 8-bit data as a series of
     * chunks; each chunk is one header followed by payload packets
     * (@ref MixedDataSetPayload) sharing the same 4-bit MDS id. Layout:
     *
     *   w0: [4b type=5][4b group][4b status=8][4b mds id][16b valid bytes in this chunk]
     *   w1: [16b number of chunks in the set][16b number of this chunk]
     *   w2: [16b manufacturer id][16b device id]
     *   w3: [16b sub id #1][16b sub id #2]
     *
     * Each packet is surfaced on its own; chunks are not reassembled.
     */
    class MixedDataSetHeader final: public UmpMessage {
    public:
        static constexpr u8 STATUS = 0x8;

        MixedDataSetHeader() = default;

        explicit MixedDataSetHeader(UniversalMidiPacket packet):
            UmpMessage{packet} {}

        /**
         * @throws InvalidMidiDataException on an out-of-range group or MDS id.
         */
        [[nodiscard]]
        THROWS(InvalidMidiDataException)
        static MixedDataSetHeader create(
            u8 group, u8 mds_id, u16 valid_bytes, u16 chunk_count, u16 chunk_number,
            u16 manufacturer_id, u16 device_id, u16 sub_id1, u16 sub_id2
        ) {
            if (group > 15 || mds_id > 15) {
                throw InvalidMidiDataException("UMP group or MDS id out of range");
            }
            return MixedDataSetHeader{UniversalMidiPacket{
                (static_cast<u32>(MessageType::DATA_128) << 28)
                    | (static_cast<u32>(group) << 24)
                    | (static_cast<u32>(STATUS) << 20)
                    | (static_cast<u32>(mds_id) << 16)
                    | valid_bytes,
                (static_cast<u32>(chunk_count) << 16) | chunk_number,
                (static_cast<u32>(manufacturer_id) << 16) | device_id,
                (static_cast<u32>(sub_id1) << 16) | sub_id2
            }};
        }

        [[nodiscard]]
        u8 mds_id() const noexcept {
            return static_cast<u8>((_packet.word(0) >> 16) & 0x0F);
        }

        [[nodiscard]]
        u16 valid_bytes() const noexcept {
            return static_cast<u16>(_packet.word(0) & 0xFFFF);
        }

        [[nodiscard]]
        u16 chunk_count() const noexcept {
            return static_cast<u16>(_packet.word(1) >> 16);
        }

        [[nodiscard]]
        u16 chunk_number() const noexcept {
            return static_cast<u16>(_packet.word(1) & 0xFFFF);
        }

        [[nodiscard]]
        u16 manufacturer_id() const noexcept {
            return static_cast<u16>(_packet.word(2) >> 16);
        }

        [[nodiscard]]
        u16 device_id() const noexcept {
            return static_cast<u16>(_packet.word(2) & 0xFFFF);
        }

        [[nodiscard]]
        u16 sub_id1() const noexcept {
            return static_cast<u16>(_packet.word(3) >> 16);
        }

        [[nodiscard]]
        u16 sub_id2() const noexcept {
            return static_cast<u16>(_packet.word(3) & 0xFFFF);
        }
    };

    /**
     * @class MixedDataSetPayload
     * @brief 128-bit Mixed Data Set payload packet (Message Type 0x5, status 0x9).
     * @extends UmpMessage
     *
     * Carries fourteen bytes of a chunk announced by a @ref MixedDataSetHeader
     * with the same MDS id: two in the low half of word 0, twelve in words 1-3.
     * How many of a chunk's bytes are meaningful is the header's valid_bytes().
     */
    class MixedDataSetPayload final: public UmpMessage {
    public:
        static constexpr u8 STATUS = 0x9;
        static constexpr usize BYTES_PER_PACKET = 14;

        MixedDataSetPayload() = default;

        explicit MixedDataSetPayload(UniversalMidiPacket packet):
            UmpMessage{packet} {}

        /**
         * @param bytes Up to 14 payload bytes; a shorter span is zero-padded.
         * @throws InvalidMidiDataException on an out-of-range group or MDS id,
         * or more than 14 bytes.
         */
        [[nodiscard]]
        THROWS(InvalidMidiDataException)
        static MixedDataSetPayload create(u8 group, u8 mds_id, Span<const u8> bytes) {
            if (group > 15 || mds_id > 15) {
                throw InvalidMidiDataException("UMP group or MDS id out of range");
            }
            if (bytes.size() > BYTES_PER_PACKET) {
                throw InvalidMidiDataException("too many bytes for one MDS payload packet");
            }
            u8 b[16] = {};
            b[0] = static_cast<u8>((static_cast<u8>(MessageType::DATA_128) << 4) | group);
            b[1] = static_cast<u8>((STATUS << 4) | mds_id);
            for (usize i = 0; i < bytes.size(); ++i) {
                b[2 + i] = bytes[i];
            }
            return MixedDataSetPayload{UniversalMidiPacket{
                UniversalMidiPacket::word_from_be(b),
                UniversalMidiPacket::word_from_be(b + 4),
                UniversalMidiPacket::word_from_be(b + 8),
                UniversalMidiPacket::word_from_be(b + 12)
            }};
        }

        [[nodiscard]]
        u8 mds_id() const noexcept {
            return static_cast<u8>((_packet.word(0) >> 16) & 0x0F);
        }

        /// All fourteen payload bytes, in wire order.
        [[nodiscard]]
        Array<u8, BYTES_PER_PACKET> bytes() const noexcept {
            Array<u8, BYTES_PER_PACKET> out = {};
            for (usize i = 0; i < BYTES_PER_PACKET; ++i) {
                const usize at = i + 2; // byte index within the 16-byte packet
                out[i] = static_cast<u8>((_packet.word(at / 4) >> (24 - (at % 4) * 8)) & 0xFF);
            }
            return out;
        }
    };

    /**
     * @enum FlexDataAddress
     * @brief The 2-bit "address" field of a Flex Data message - whether the
     * message targets a whole group or a specific channel within it.
     */
    enum class FlexDataAddress: u8 {
        CHANNEL = 0x0, ///< Addressed to the channel in the channel field.
        GROUP = 0x1,   ///< Addressed to the whole group; channel field ignored.
    };

    /**
     * @class FlexDataMessage
     * @brief 128-bit Flex Data message (Message Type 0xD).
     * @extends UmpMessage
     *
     * Flex Data is the UMP home for the performance and metadata events that
     * MIDI 1.0 could only express as Standard-MIDI-File meta events: Set Tempo,
     * Set Time Signature, Set Key Signature, and text events such as lyrics and
     * composer name. Word 0 is laid out as:
     *
     *   [4b type=0xD][4b group][2b form][2b address][4b channel]
     *   [8b status bank][8b status]
     *
     * and words 1-3 carry the 96-bit payload. A single (unfragmented) message
     * uses form 0.
     */
    class FlexDataMessage final: public UmpMessage {
        static constexpr u32 encode_w0(
            u8 group, u8 form, FlexDataAddress address, u8 channel, u8 status_bank, u8 status
        ) noexcept {
            return (static_cast<u32>(MessageType::FLEX_DATA) << 28)
                | (static_cast<u32>(group & 0x0F) << 24)
                | (static_cast<u32>(form & 0x03) << 22)
                | (static_cast<u32>(static_cast<u8>(address) & 0x03) << 20)
                | (static_cast<u32>(channel & 0x0F) << 16)
                | (static_cast<u32>(status_bank) << 8)
                | static_cast<u32>(status);
        }
    public:
        /// Common status-bank values.
        static constexpr u8 BANK_SETUP_AND_PERFORMANCE = 0x00; ///< Set Tempo, Time/Key Signature, etc.
        static constexpr u8 BANK_METADATA_TEXT = 0x01;         ///< Composer, lyricist, ...
        static constexpr u8 BANK_PERFORMANCE_TEXT = 0x02;      ///< Lyrics, ...

        /// Common statuses within BANK_SETUP_AND_PERFORMANCE.
        static constexpr u8 STATUS_SET_TEMPO = 0x00;
        static constexpr u8 STATUS_SET_TIME_SIGNATURE = 0x01;
        static constexpr u8 STATUS_SET_METRONOME = 0x02;
        static constexpr u8 STATUS_SET_KEY_SIGNATURE = 0x05;

        FlexDataMessage() = default;

        explicit FlexDataMessage(UniversalMidiPacket packet):
            UmpMessage{packet} {}

        /**
         * @brief Build a single (form 0) Flex Data message.
         * @param group UMP group, 0-15.
         * @param address Whether this targets a channel or the whole group.
         * @param channel 0-15 (ignored by consumers when address is GROUP).
         * @param status_bank One of the BANK_* constants.
         * @param status Status within the bank, e.g. STATUS_SET_TEMPO.
         * @param data The 96-bit payload as three 32-bit words (word 1-3).
         * @throws InvalidMidiDataException on an out-of-range group or channel.
         */
        THROWS(InvalidMidiDataException)
        static FlexDataMessage create(
            u8 group, FlexDataAddress address, u8 channel,
            u8 status_bank, u8 status, u32 data1 = 0, u32 data2 = 0, u32 data3 = 0
        ) {
            if (group > 15 || channel > 15) {
                throw InvalidMidiDataException("UMP group or channel out of range");
            }
            return FlexDataMessage{
                UniversalMidiPacket{
                    encode_w0(group, 0, address, channel, status_bank, status),
                    data1, data2, data3
                }
            };
        }

        /**
         * @brief Set Tempo: a group-addressed message whose 32-bit payload is
         * the tempo in 10-nanosecond units per quarter note (per the spec).
         * @throws InvalidMidiDataException on an out-of-range group.
         */
        THROWS(InvalidMidiDataException)
        static FlexDataMessage set_tempo(u8 group, u32 ten_ns_per_quarter) {
            return create(
                group, FlexDataAddress::GROUP, 0,
                BANK_SETUP_AND_PERFORMANCE, STATUS_SET_TEMPO, ten_ns_per_quarter
            );
        }

        [[nodiscard]]
        u8 form() const noexcept {
            return static_cast<u8>((_packet.word(0) >> 22) & 0x03);
        }

        [[nodiscard]]
        FlexDataAddress address() const noexcept {
            return static_cast<FlexDataAddress>((_packet.word(0) >> 20) & 0x03);
        }

        [[nodiscard]]
        u8 channel() const noexcept {
            return static_cast<u8>((_packet.word(0) >> 16) & 0x0F);
        }

        [[nodiscard]]
        u8 status_bank() const noexcept {
            return static_cast<u8>((_packet.word(0) >> 8) & 0xFF);
        }

        [[nodiscard]]
        u8 status() const noexcept {
            return static_cast<u8>(_packet.word(0) & 0xFF);
        }

        /// Whether this is a Set Tempo message.
        [[nodiscard]]
        bool is_set_tempo() const noexcept {
            return status_bank() == BANK_SETUP_AND_PERFORMANCE && status() == STATUS_SET_TEMPO;
        }

        /**
         * @brief A Set Tempo's tempo in microseconds per quarter note - the
         * unit SMF meta 0x51 uses - or empty for any other message.
         */
        [[nodiscard]]
        Optional<u32> tempo_us_per_quarter() const noexcept {
            if (!is_set_tempo()) {
                return {};
            }
            return _packet.word(1) / 100; // payload is in 10-nanosecond units
        }

        /// The first payload word (word 1); for Set Tempo this is the tempo.
        [[nodiscard]]
        u32 data1() const noexcept {
            return _packet.word(1);
        }

        [[nodiscard]]
        u32 data2() const noexcept {
            return _packet.word(2);
        }

        [[nodiscard]]
        u32 data3() const noexcept {
            return _packet.word(3);
        }
    };

    /**
     * @enum StreamStatus
     * @brief Status codes for UMP Stream messages (the 10-bit status field).
     *
     * Stream messages describe the endpoint itself rather than musical data:
     * discovery, endpoint/device identity, and function-block configuration.
     */
    enum class StreamStatus: u16 {
        ENDPOINT_DISCOVERY = 0x00,
        ENDPOINT_INFO_NOTIFICATION = 0x01,
        DEVICE_IDENTITY_NOTIFICATION = 0x02,
        ENDPOINT_NAME_NOTIFICATION = 0x03,
        PRODUCT_INSTANCE_ID_NOTIFICATION = 0x04,
        STREAM_CONFIGURATION_REQUEST = 0x05,
        STREAM_CONFIGURATION_NOTIFICATION = 0x06,
        FUNCTION_BLOCK_DISCOVERY = 0x10,
        FUNCTION_BLOCK_INFO_NOTIFICATION = 0x11,
        FUNCTION_BLOCK_NAME_NOTIFICATION = 0x12,
        START_OF_CLIP = 0x20, ///< Marks where a clip's sequence data begins.
        END_OF_CLIP = 0x21,   ///< Marks where it ends.
    };

    /**
     * @class UmpStreamMessage
     * @brief 128-bit UMP Stream message (Message Type 0xF).
     * @extends UmpMessage
     *
     * Stream messages are groupless - they describe the UMP endpoint and its
     * function blocks (discovery, naming, protocol negotiation) rather than
     * carrying notes. Word 0 is laid out as:
     *
     *   [4b type=0xF][2b format][10b status][16b data]
     *
     * and words 1-3 carry the remaining payload. "format" marks a message as
     * complete (0) or as the start/continue/end of a multi-packet run, mirroring
     * the SysEx fragmentation scheme.
     */
    class UmpStreamMessage final: public UmpMessage {
        static constexpr u32 encode_w0(u8 format, StreamStatus status, u16 data) noexcept {
            return (static_cast<u32>(MessageType::STREAM) << 28)
                | (static_cast<u32>(format & 0x03) << 26)
                | (static_cast<u32>(static_cast<u16>(status) & 0x03FF) << 16)
                | static_cast<u32>(data);
        }
    public:
        /// The "format" field values, shared with the SysEx fragmentation model.
        static constexpr u8 FORMAT_COMPLETE = 0x0;
        static constexpr u8 FORMAT_START = 0x1;
        static constexpr u8 FORMAT_CONTINUE = 0x2;
        static constexpr u8 FORMAT_END = 0x3;

        UmpStreamMessage() = default;

        explicit UmpStreamMessage(UniversalMidiPacket packet):
            UmpMessage{packet} {}

        /**
         * @brief Build a single (complete) UMP Stream message.
         * @param status One of the StreamStatus codes.
         * @param payload16 The 16-bit word-0 payload (e.g. UMP version for discovery).
         * @param data1 Payload word 1.
         * @param data2 Payload word 2.
         * @param data3 Payload word 3.
         */
        [[nodiscard]]
        static UmpStreamMessage create(
            StreamStatus status, u16 payload16 = 0, u32 data1 = 0, u32 data2 = 0, u32 data3 = 0
        ) {
            return UmpStreamMessage{
                UniversalMidiPacket{encode_w0(FORMAT_COMPLETE, status, payload16), data1, data2, data3}
            };
        }

        /**
         * @brief An Endpoint Discovery message requesting the given info.
         * @param ump_version_major UMP version major (1 for UMP 1.1).
         * @param ump_version_minor UMP version minor (1 for UMP 1.1).
         * @param filter_bitmap Which notifications are requested (bit flags).
         */
        [[nodiscard]]
        static UmpStreamMessage endpoint_discovery(
            u8 ump_version_major = 1, u8 ump_version_minor = 1, u8 filter_bitmap = 0x1F
        ) {
            const u16 version = static_cast<u16>((ump_version_major << 8) | ump_version_minor);
            return create(StreamStatus::ENDPOINT_DISCOVERY, version, static_cast<u32>(filter_bitmap));
        }

        /// Start of Clip: the clip's sequence data begins here.
        [[nodiscard]]
        static UmpStreamMessage start_of_clip() {
            return create(StreamStatus::START_OF_CLIP);
        }

        /// End of Clip: the clip's sequence data ends here.
        [[nodiscard]]
        static UmpStreamMessage end_of_clip() {
            return create(StreamStatus::END_OF_CLIP);
        }

        [[nodiscard]]
        u8 format() const noexcept {
            return static_cast<u8>((_packet.word(0) >> 26) & 0x03);
        }

        [[nodiscard]]
        StreamStatus stream_status() const noexcept {
            return static_cast<StreamStatus>((_packet.word(0) >> 16) & 0x03FF);
        }

        /// The 16-bit word-0 payload. Named to avoid hiding MidiMessage::data().
        [[nodiscard]]
        u16 payload16() const noexcept {
            return static_cast<u16>(_packet.word(0) & 0xFFFF);
        }

        [[nodiscard]]
        u32 data1() const noexcept {
            return _packet.word(1);
        }

        [[nodiscard]]
        u32 data2() const noexcept {
            return _packet.word(2);
        }

        [[nodiscard]]
        u32 data3() const noexcept {
            return _packet.word(3);
        }
    };

    inline bool MidiTranslator::is_ump(const MidiMessage& msg) noexcept {
        return dynamic_cast<const UmpMessage*>(&msg) != nullptr
            || dynamic_cast<const Sysex7Message*>(&msg) != nullptr
            || dynamic_cast<const Sysex8Message*>(&msg) != nullptr;
    }

    inline Vector<Vector<u8>> MidiTranslator::to_midi1_bytes(const MidiMessage& msg) {
        Vector<Vector<u8>> out;
        const auto wrap_sysex = [&out](const Vector<u8>& body) -> void {
            Vector<u8> bytes;
            bytes.reserve(body.size() + 2);
            bytes.push_back(0xF0);
            bytes.insert(bytes.end(), body.begin(), body.end());
            bytes.push_back(0xF7);
            out.push_back(Ops::move(bytes));
        };

        if (const Sysex7Message* sysex7 = dynamic_cast<const Sysex7Message*>(&msg)) {
            wrap_sysex(sysex7->payload());
            return out;
        }
        if (dynamic_cast<const Sysex8Message*>(&msg) != nullptr) {
            return out; // 8-bit data has no MIDI 1.0 wire form.
        }
        const UmpMessage* ump = dynamic_cast<const UmpMessage*>(&msg);
        if (ump == nullptr) {
            const Span<const u8> raw = msg.data();
            out.push_back(Vector<u8>(raw.begin(), raw.end()));
            return out;
        }

        const UniversalMidiPacket& packet = ump->packet();
        const u32 w0 = packet.word(0);
        const u8 status = static_cast<u8>((w0 >> 16) & 0xFF);
        const u8 data1 = static_cast<u8>((w0 >> 8) & 0x7F);
        const u8 data2 = static_cast<u8>(w0 & 0x7F);
        switch (packet.type()) {
            case MessageType::SYSTEM: {
                if (status < 0xF1) {
                    break;
                }
                Vector<u8> bytes;
                bytes.push_back(status);
                const u8 length = system_data_length(status);
                if (length >= 1) {
                    bytes.push_back(data1);
                }
                if (length >= 2) {
                    bytes.push_back(data2);
                }
                out.push_back(Ops::move(bytes));
                break;
            }
            case MessageType::MIDI1_CHANNEL_VOICE: {
                if (status < 0x80 || status >= 0xF0) {
                    break;
                }
                Vector<u8> bytes;
                bytes.push_back(status);
                bytes.push_back(data1);
                const u8 command = static_cast<u8>(status & 0xF0);
                if (command != 0xC0 && command != 0xD0) {
                    bytes.push_back(data2);
                }
                out.push_back(Ops::move(bytes));
                break;
            }
            case MessageType::MIDI2_CHANNEL_VOICE:
                for (const ShortMessage& short_msg: to_midi1_messages(Midi2ChannelVoiceMessage{packet})) {
                    const Span<const u8> raw = short_msg.data();
                    out.push_back(Vector<u8>(raw.begin(), raw.end()));
                }
                break;
            case MessageType::DATA_64:
                // Only a self-contained packet can be converted on its own.
                if (((w0 >> 20) & 0x0F) == static_cast<u32>(SysexStatus::COMPLETE)) {
                    Vector<UniversalMidiPacket> single;
                    single.push_back(packet);
                    wrap_sysex(Sysex7Message::from_packets(Ops::move(single)).payload());
                }
                break;
            default:
                break;
        }
        return out;
    }

    /**
     * @class UmpEncoder
     * @brief Turns any @ref MidiMessage into the Universal MIDI Packets a UMP
     * transport sends.
     *
     * The bridge for MIDI 1.0 code talking to a UMP endpoint. UMP-family
     * messages pass through as their own packets. MIDI 1.0 byte-stream
     * messages are converted onto the encoder's group:
     *
     * - channel voice becomes 32-bit MIDI 1.0 channel-voice packets under
     *   @ref UmpProtocol::MIDI1, or 64-bit MIDI 2.0 ones under
     *   @ref UmpProtocol::MIDI2 (via a @ref Midi1ToMidi2Translator, so Bank
     *   Select and RPN/NRPN are folded in);
     * - system common and real-time become System packets;
     * - SysEx (F0 ... F7) becomes SysEx7 packets;
     * - SMF meta events, which never go on a wire, produce nothing.
     *
     * The encoder is stateful under MIDI2 (see Midi1ToMidi2Translator), so
     * keep one per destination stream and do not share it across threads.
     */
    class UmpEncoder final {
    private:
        UmpProtocol _protocol;
        u8 _group;
        Midi1ToMidi2Translator _translator;
    public:
        /**
         * @param protocol Which channel-voice form to produce.
         * @param group The UMP group MIDI 1.0 messages are placed on, 0-15.
         * @throws InvalidMidiDataException on an out-of-range group.
         */
        THROWS(InvalidMidiDataException)
        explicit UmpEncoder(UmpProtocol protocol = UmpProtocol::MIDI1, u8 group = 0):
            _protocol{protocol}, _group{group} {
            if (group > 15) {
                throw InvalidMidiDataException("UMP group out of range");
            }
        }

        [[nodiscard]]
        UmpProtocol protocol() const noexcept {
            return _protocol;
        }

        [[nodiscard]]
        u8 group() const noexcept {
            return _group;
        }

        /**
         * @brief Switch protocol, e.g. after a Stream Configuration change.
         * Forgets any bank / RPN state gathered under the old one.
         */
        void set_protocol(UmpProtocol protocol) noexcept {
            _protocol = protocol;
            _translator.reset();
        }

        /**
         * @throws InvalidMidiDataException on an out-of-range group.
         */
        THROWS(InvalidMidiDataException)
        void set_group(u8 group) {
            if (group > 15) {
                throw InvalidMidiDataException("UMP group out of range");
            }
            _group = group;
        }

        /**
         * @brief The packets that carry @p msg, in send order.
         * @return Empty if @p msg has no UMP form (meta events, a bare EOX,
         * a running-status fragment, a MIDI 1.0 message that only updated
         * translator state).
         * @throws InvalidMidiDataException if @p msg's bytes are malformed (a
         * data byte above 0x7F, an 8-bit byte inside a SysEx).
         */
        [[nodiscard]]
        THROWS(InvalidMidiDataException)
        Vector<UniversalMidiPacket> encode(const MidiMessage& msg) {
            Vector<UniversalMidiPacket> out;
            if (const UmpMessage* ump = dynamic_cast<const UmpMessage*>(&msg)) {
                out.push_back(ump->packet());
                return out;
            }
            if (const Sysex7Message* sysex7 = dynamic_cast<const Sysex7Message*>(&msg)) {
                return sysex7->packets();
            }
            if (const Sysex8Message* sysex8 = dynamic_cast<const Sysex8Message*>(&msg)) {
                return sysex8->packets();
            }
            if (dynamic_cast<const MetaMessage*>(&msg) != nullptr) {
                return out;
            }

            const Span<const u8> bytes = msg.data();
            if (bytes.empty()) {
                return out;
            }
            const u8 status = bytes[0];
            const u8 data1 = bytes.size() > 1 ? bytes[1] : 0;
            const u8 data2 = bytes.size() > 2 ? bytes[2] : 0;

            if (status == 0xF0) {
                Span<const u8> payload = bytes.subspan(1);
                if (!payload.empty() && payload.back() == 0xF7) {
                    payload = payload.first(payload.size() - 1);
                }
                return Sysex7Message::from_bytes(_group, payload).packets();
            }
            if (status == 0xF7 || status < 0x80) {
                return out; // Stray EOX, or a running-status fragment.
            }
            if (status > 0xF0) {
                const u8 length = MidiTranslator::system_data_length(status);
                out.push_back(SystemMessage{
                    _group, static_cast<MidiStatus>(status),
                    length >= 1 ? data1 : u8{0},
                    length >= 2 ? data2 : u8{0}
                }.packet());
                return out;
            }

            const MidiStatus command = static_cast<MidiStatus>(status & 0xF0);
            const u8 channel = static_cast<u8>(status & 0x0F);
            const bool two_byte = command == MidiStatus::PROGRAM_CHANGE
                || command == MidiStatus::CHANNEL_PRESSURE;
            if (_protocol == UmpProtocol::MIDI1) {
                out.push_back(
                    Midi1ChannelVoiceMessage{_group, command, channel, data1, two_byte ? u8{0} : data2}.packet()
                );
                return out;
            }
            const ShortMessage short_msg{command, channel, data1, two_byte ? u8{0} : data2};
            if (Optional<Midi2ChannelVoiceMessage> up = _translator.translate(short_msg, _group); up.has_value()) {
                out.push_back(up->packet());
            }
            return out;
        }
    };

    /**
     * @class UmpParser
     * @brief Turns a stream of raw UMP words back into typed messages.
     *
     * Feed it words as they arrive, in any chunking: a packet split across
     * two reads is held until its remaining words come in. Each complete
     * packet is decoded into the matching message class (UtilityMessage,
     * SystemMessage, Midi1ChannelVoiceMessage, Midi2ChannelVoiceMessage,
     * FlexDataMessage, UmpStreamMessage), so a receiver can dynamic_cast on
     * what it gets.
     *
     * Multi-packet SysEx is reassembled: SysEx7 runs per group, SysEx8 runs
     * per group and stream id, each delivered once as a whole
     * @ref Sysex7Message / @ref Sysex8Message when its END packet arrives.
     * A CONTINUE or END with no open run is dropped; a new START replaces an
     * open run (its sender evidently abandoned it); a run longer than
     * @ref MAX_RUN_PACKETS is abandoned so a sender that never ends one
     * cannot grow memory without bound.
     *
     * Mixed Data Set packets come out one by one as @ref MixedDataSetHeader /
     * @ref MixedDataSetPayload, without chunk reassembly. Reserved Message
     * Types and statuses come out as a plain @ref UmpMessage; because the size of every
     * Message Type is fixed, they never desynchronise the words after them.
     */
    class UmpParser final {
    public:
        static constexpr usize MAX_RUN_PACKETS = 65'536; ///< Longest SysEx run kept, in packets.
    private:
        /**
         * @internal
         * @brief An open SysEx8 run, keyed by group and stream id.
         */
        struct Sysex8Run {
            u8 group;
            u8 stream_id;
            Vector<UniversalMidiPacket> packets;
        };

        u32 _partial[4] = {}; ///< Words of the packet being assembled.
        u8 _partial_count = 0; ///< How many of @ref _partial are filled.
        u8 _partial_needed = 0; ///< How many words that packet needs.
        Optional<Vector<UniversalMidiPacket>> _sysex7[16]; ///< Open SysEx7 run per group.
        Vector<Sysex8Run> _sysex8; ///< Open SysEx8 runs; few at once, so a flat list.

        [[nodiscard]]
        static Vector<UniversalMidiPacket> single(const UniversalMidiPacket& packet) {
            Vector<UniversalMidiPacket> packets;
            packets.push_back(packet);
            return packets;
        }

        /**
         * @internal
         * @brief Advance a SysEx run by one packet.
         * @return The finished run, if @p packet completed one.
         */
        [[nodiscard]]
        static Optional<Vector<UniversalMidiPacket>> advance(
            Optional<Vector<UniversalMidiPacket>>& run, SysexStatus status, const UniversalMidiPacket& packet
        ) {
            switch (status) {
                case SysexStatus::COMPLETE:
                    return single(packet);
                case SysexStatus::START:
                    run.emplace();
                    run->push_back(packet);
                    return {};
                case SysexStatus::CONTINUE:
                    if (run.has_value()) {
                        run->push_back(packet);
                        if (run->size() > MAX_RUN_PACKETS) {
                            run.reset();
                        }
                    }
                    return {};
                case SysexStatus::END: {
                    if (!run.has_value()) {
                        return {};
                    }
                    run->push_back(packet);
                    Optional<Vector<UniversalMidiPacket>> done = Ops::move(run);
                    run.reset();
                    return done;
                }
            }
            return {};
        }

        template <typename F>
        void dispatch(const UniversalMidiPacket& packet, F& on_message) {
            const u32 w0 = packet.word(0);
            switch (packet.type()) {
                case MessageType::UTILITY:
                    on_message(UtilityMessage{packet});
                    return;
                case MessageType::SYSTEM:
                    on_message(SystemMessage{packet});
                    return;
                case MessageType::MIDI1_CHANNEL_VOICE:
                    on_message(Midi1ChannelVoiceMessage{packet});
                    return;
                case MessageType::MIDI2_CHANNEL_VOICE:
                    on_message(Midi2ChannelVoiceMessage{packet});
                    return;
                case MessageType::FLEX_DATA:
                    on_message(FlexDataMessage{packet});
                    return;
                case MessageType::STREAM:
                    on_message(UmpStreamMessage{packet});
                    return;
                case MessageType::DATA_64: {
                    const u8 status = static_cast<u8>((w0 >> 20) & 0x0F);
                    if (status > static_cast<u8>(SysexStatus::END)) {
                        break;
                    }
                    Optional<Vector<UniversalMidiPacket>> done = advance(
                        _sysex7[packet.group()], static_cast<SysexStatus>(status), packet
                    );
                    if (done.has_value()) {
                        on_message(Sysex7Message::from_packets(Ops::move(*done)));
                    }
                    return;
                }
                case MessageType::DATA_128: {
                    const u8 status = static_cast<u8>((w0 >> 20) & 0x0F);
                    if (status == MixedDataSetHeader::STATUS) {
                        on_message(MixedDataSetHeader{packet});
                        return;
                    }
                    if (status == MixedDataSetPayload::STATUS) {
                        on_message(MixedDataSetPayload{packet});
                        return;
                    }
                    if (status > static_cast<u8>(SysexStatus::END)) {
                        break; // Reserved status.
                    }
                    const u8 group = packet.group();
                    const u8 stream_id = static_cast<u8>((w0 >> 8) & 0xFF);
                    usize index = 0;
                    while (index < _sysex8.size()
                        && !(_sysex8[index].group == group && _sysex8[index].stream_id == stream_id)) {
                        ++index;
                    }
                    Optional<Vector<UniversalMidiPacket>> run;
                    if (index < _sysex8.size()) {
                        run = Ops::move(_sysex8[index].packets);
                        _sysex8.erase(_sysex8.begin() + static_cast<isize>(index));
                    }
                    Optional<Vector<UniversalMidiPacket>> done = advance(
                        run, static_cast<SysexStatus>(status), packet
                    );
                    if (run.has_value()) {
                        _sysex8.push_back(Sysex8Run{group, stream_id, Ops::move(*run)});
                    }
                    if (done.has_value()) {
                        on_message(Sysex8Message::from_packets(Ops::move(*done)));
                    }
                    return;
                }
                default:
                    break;
            }
            on_message(UmpMessage{packet});
        }

        /**
         * @internal
         * @brief Assemble @p words into packets and dispatch each to @p emit,
         * which receives every decoded message as a prvalue of its own class.
         */
        template <typename Emit>
        void consume(Span<const u32> words, Emit& emit) {
            for (const u32 word: words) {
                if (_partial_count == 0) {
                    _partial_needed = UniversalMidiPacket::words_for(
                        static_cast<MessageType>((word >> 28) & 0x0F)
                    );
                }
                _partial[_partial_count++] = word;
                if (_partial_count < _partial_needed) {
                    continue;
                }
                const Optional<UniversalMidiPacket> packet = UniversalMidiPacket::from_words(
                    Span<const u32>{_partial, _partial_count}
                );
                _partial_count = 0;
                if (packet.has_value()) {
                    dispatch(*packet, emit);
                }
            }
        }
    public:
        UmpParser() = default;

        /**
         * @brief Consume @p words, calling @p on_message once per decoded
         * message.
         * @param words Native-endian UMP words, in arrival order.
         * @param on_message Invoked as `on_message(const MidiMessage&)`; the
         * message is a temporary, valid only for the duration of the call.
         */
        template <typename F>
        void feed(Span<const u32> words, F&& on_message) {
            auto emit = [&on_message](const MidiMessage& msg) -> void {
                on_message(msg);
            };
            consume(words, emit);
        }

        /**
         * @brief Like @ref feed, but hands over each message as an owned
         * pointer that keeps its decoded class - for callers that store what
         * they parse (a file reader building a Sequence, a queue).
         * @param on_message Invoked as `on_message(UniquePointer<MidiMessage>)`.
         */
        template <typename F>
        void feed_owned(Span<const u32> words, F&& on_message) {
            auto emit = [&on_message]<typename M>(M&& msg) -> void {
                on_message(UniquePointer<MidiMessage>{Pointers::unique<M>(Ops::move(msg))});
            };
            consume(words, emit);
        }

        /**
         * @brief Whether a packet is waiting on words not yet fed.
         */
        [[nodiscard]]
        bool has_partial_packet() const noexcept {
            return _partial_count != 0;
        }

        /**
         * @brief Drop any half-received packet and every open SysEx run, e.g.
         * after the transport was interrupted.
         */
        void reset() noexcept {
            _partial_count = 0;
            _partial_needed = 0;
            for (Optional<Vector<UniversalMidiPacket>>& run: _sysex7) {
                run.reset();
            }
            _sysex8.clear();
        }
    };
}
