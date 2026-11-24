#pragma once

using stdx::collections::Vector;
using stdx::random::MersenneTwister64;
using stdx::random::RandomDevice;

/**
 * @namespace stdx::audio::midi
 * @brief MIDI Capability Inquiry (MIDI-CI) messages.
 *
 * MIDI-CI lets two MIDI devices negotiate extended capabilities over a
 * bidirectional link. Every MIDI-CI message is a Universal System Exclusive
 * message (F0 7E ... F7) carrying a fixed header - device id, a category
 * sub-id, the MIDI-CI version, and the source/destination MUIDs - followed by
 * a category-specific body.
 *
 * This layer builds and parses the management messages (Discovery,
 * Invalidate MUID, ACK, NAK), Profile Inquiry and its reply, and Property
 * Exchange capabilities, in their MIDI-CI 1.2 layouts. Every message class
 * has a parse() that accepts either raw F0..F7 bytes or any MidiMessage -
 * including a Sysex7Message reassembled by UmpParser, which is how MIDI-CI
 * arrives over UMP. Parsing is lenient about version: a 1.1 body, which
 * lacks the fields 1.2 appended, parses and reports those fields as absent.
 *
 * MUIDs are 28-bit identifiers a device picks at random on power-up. On the
 * wire they are four 7-bit bytes, least-significant first.
 */
export namespace stdx::audio::midi {
    /**
     * @class Muid
     * @brief A 28-bit MIDI-CI identifier.
     *
     * Wire form is four 7-bit bytes, LSB first. The all-ones value
     * (0x0FFFFFFF) is the broadcast MUID that addresses every device; the 255
     * values just below it (0x0FFFFF00-0x0FFFFFFE) are reserved.
     */
    class Muid final {
        u32 _value = 0; ///< 28-bit value in the low bits.
    public:
        static constexpr u32 MASK = 0x0FFFFFFFu;
        static constexpr u32 RESERVED_FIRST = 0x0FFFFF00u; ///< First reserved value; broadcast is the last.

        constexpr Muid() = default;

        explicit constexpr Muid(u32 value) noexcept:
            _value{value & MASK} {}

        [[nodiscard]]
        constexpr u32 value() const noexcept {
            return _value;
        }

        /// The broadcast MUID (0x0FFFFFFF) - addresses every device on the link.
        [[nodiscard]]
        static constexpr Muid broadcast() noexcept {
            return Muid{MASK};
        }

        [[nodiscard]]
        constexpr bool is_broadcast() const noexcept {
            return _value == MASK;
        }

        /// Whether this is broadcast or one of the reserved values, i.e. not a device's own MUID.
        [[nodiscard]]
        constexpr bool is_reserved() const noexcept {
            return _value >= RESERVED_FIRST;
        }

        /**
         * @brief Draw a fresh random MUID, as a device does on power-up.
         *
         * Never returns broadcast or a reserved value, so the result always
         * names a single device.
         */
        [[nodiscard]]
        static Muid random() {
            static thread_local MersenneTwister64 engine{RandomDevice{}()};
            u32 v = static_cast<u32>(engine()) & MASK;
            if (v >= RESERVED_FIRST) {
                v -= 0x100; // fold the reserved block down onto ordinary values
            }
            return Muid{v};
        }

        /// Append the MUID as four 7-bit bytes, LSB first (wire order).
        void to_bytes(Vector<u8>& out) const {
            out.push_back(static_cast<u8>(_value & 0x7F));
            out.push_back(static_cast<u8>((_value >> 7) & 0x7F));
            out.push_back(static_cast<u8>((_value >> 14) & 0x7F));
            out.push_back(static_cast<u8>((_value >> 21) & 0x7F));
        }

        /// Read a MUID from four 7-bit wire bytes, LSB first.
        [[nodiscard]]
        static constexpr Muid from_bytes(const u8* p) noexcept {
            return Muid{
                static_cast<u32>(p[0] & 0x7F)
                | (static_cast<u32>(p[1] & 0x7F) << 7)
                | (static_cast<u32>(p[2] & 0x7F) << 14)
                | (static_cast<u32>(p[3] & 0x7F) << 21)
            };
        }

        // Written out rather than defaulted: GCC 16 ICEs (segfault) on a
        // defaulted friend operator== here when a TU that imports the module
        // compares two Muids.
        [[nodiscard]]
        friend constexpr bool operator==(const Muid& left, const Muid& right) noexcept {
            return left._value == right._value;
        }
    };

    /**
     * @enum CiCategory
     * @brief The MIDI-CI message category, carried as sub-ID#2 of the
     * Universal SysEx. The high nibble groups messages by purpose.
     */
    enum class CiCategory: u8 {
        PROFILE_INQUIRY = 0x20,
        PROFILE_INQUIRY_REPLY = 0x21,
        PROPERTY_EXCHANGE_CAPABILITIES = 0x30,
        PROPERTY_EXCHANGE_CAPABILITIES_REPLY = 0x31,
        DISCOVERY = 0x70,
        DISCOVERY_REPLY = 0x71,
        ACK = 0x7D, ///< MIDI-CI 1.2.
        INVALIDATE_MUID = 0x7E,
        NAK = 0x7F,
    };

    /**
     * @class MidiCiMessage
     * @brief A MIDI-CI message: a Universal SysEx carrying the MIDI-CI header
     * plus a category body.
     * @extends MidiMessage
     *
     * MIDI-CI messages are Universal System Exclusive messages, but
     * @ref SysexMessage is final, so this derives from @ref MidiMessage and
     * assembles the full F0..F7 byte buffer itself. The accessors decode the
     * header back out. The raw bytes (via data()) are a complete SysEx ready to
     * hand to a Receiver - a UMP receiver's UmpEncoder turns them into SysEx7.
     */
    class MidiCiMessage: public MidiMessage {
    public:
        static constexpr u8 UNIVERSAL_SYSEX = 0x7E;
        static constexpr u8 SUB_ID1_MIDI_CI = 0x0D;
        static constexpr u8 DEVICE_ID_GROUP = 0x7E; ///< 1.2: to/from the whole UMP group.
        static constexpr u8 DEVICE_ID_FUNCTION_BLOCK = 0x7F; ///< 1.2: to/from the Function Block.
        static constexpr u8 DEVICE_ID_PORT = 0x7F; ///< The 1.1 name for 0x7F ("MIDI port").
        static constexpr u8 CI_VERSION_1_1 = 0x01; ///< MIDI-CI v1.1.
        static constexpr u8 CI_VERSION_1_2 = 0x02; ///< MIDI-CI v1.2.
        static constexpr usize HEADER_SIZE = 14; ///< F0 through the destination MUID.

        MidiCiMessage() = default;

        /**
         * @brief Assemble a MIDI-CI message from its header fields and body.
         * @param category The sub-ID#2 message category.
         * @param source The sender's MUID.
         * @param destination The target MUID (or Muid::broadcast()).
         * @param body The category-specific payload (may be empty).
         * @param device_id 0x00-0x0F for a channel, DEVICE_ID_GROUP, or
         * DEVICE_ID_FUNCTION_BLOCK.
         * @param ci_version The MIDI-CI version byte.
         */
        MidiCiMessage(
            CiCategory category, Muid source, Muid destination,
            Span<const u8> body,
            u8 device_id = DEVICE_ID_FUNCTION_BLOCK, u8 ci_version = CI_VERSION_1_2
        ) {
            _bytes.clear();
            _bytes.push_back(0xF0);
            _bytes.push_back(UNIVERSAL_SYSEX);
            _bytes.push_back(device_id);
            _bytes.push_back(SUB_ID1_MIDI_CI);
            _bytes.push_back(static_cast<u8>(category));
            _bytes.push_back(ci_version);
            source.to_bytes(_bytes);
            destination.to_bytes(_bytes);
            for (const u8 b: body) {
                _bytes.push_back(b);
            }
            _bytes.push_back(0xF7);
        }

        /**
         * @brief The complete F0..F7 SysEx bytes @p msg carries.
         *
         * A Sysex7Message (UMP) carries its body without the wrappers, so they
         * are restored; any other message is taken byte for byte.
         */
        [[nodiscard]]
        static Vector<u8> sysex_bytes(const MidiMessage& msg) {
            if (const Sysex7Message* sysex7 = dynamic_cast<const Sysex7Message*>(&msg)) {
                const Vector<u8> body = sysex7->payload();
                Vector<u8> bytes;
                bytes.reserve(body.size() + 2);
                bytes.push_back(0xF0);
                bytes.insert(bytes.end(), body.begin(), body.end());
                bytes.push_back(0xF7);
                return bytes;
            }
            const Span<const u8> raw = msg.data();
            return Vector<u8>(raw.begin(), raw.end());
        }

        /**
         * @brief Whether @p bytes are a well-formed MIDI-CI message: F0 7E ..
         * 0D .. F7, a full header, and 7-bit data throughout.
         */
        [[nodiscard]]
        static bool is_well_formed(Span<const u8> bytes) noexcept {
            if (
                bytes.size() < HEADER_SIZE + 1
                || bytes[0] != 0xF0
                || bytes[1] != UNIVERSAL_SYSEX
                || bytes[3] != SUB_ID1_MIDI_CI
                || bytes[bytes.size() - 1] != 0xF7
            ) {
                return false;
            }
            for (usize i = 1; i + 1 < bytes.size(); ++i) {
                if (bytes[i] > 0x7F) {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief Parse any MIDI-CI message, whatever its category.
         * @return The message, or empty if @p bytes are not well-formed MIDI-CI.
         */
        [[nodiscard]]
        static Optional<MidiCiMessage> parse(Span<const u8> bytes) {
            if (!is_well_formed(bytes)) {
                return {};
            }
            MidiCiMessage msg;
            msg.assign_bytes(bytes);
            return msg;
        }

        /// @copydoc parse(Span<const u8>)
        [[nodiscard]]
        static Optional<MidiCiMessage> parse(const MidiMessage& msg) {
            const Vector<u8> bytes = sysex_bytes(msg);
            return parse(Span<const u8>{bytes.data(), bytes.size()});
        }

        /// True if the bytes look like a MIDI-CI Universal SysEx.
        [[nodiscard]]
        bool is_midi_ci() const noexcept {
            return _bytes.size() >= HEADER_SIZE + 1
                && _bytes[0] == 0xF0
                && _bytes[1] == UNIVERSAL_SYSEX
                && _bytes[3] == SUB_ID1_MIDI_CI;
        }

        [[nodiscard]]
        u8 device_id() const noexcept {
            return _bytes.size() > 2 ? _bytes[2] : 0;
        }

        [[nodiscard]]
        CiCategory category() const noexcept {
            return static_cast<CiCategory>(_bytes.size() > 4 ? _bytes[4] : 0);
        }

        [[nodiscard]]
        u8 ci_version() const noexcept {
            return _bytes.size() > 5 ? _bytes[5] : 0;
        }

        [[nodiscard]]
        Muid source_muid() const noexcept {
            return _bytes.size() >= 10 ? Muid::from_bytes(&_bytes[6]) : Muid{};
        }

        [[nodiscard]]
        Muid destination_muid() const noexcept {
            return _bytes.size() >= 14 ? Muid::from_bytes(&_bytes[10]) : Muid{};
        }

        /// The category body: the bytes between the header and the trailing F7.
        [[nodiscard]]
        Span<const u8> body() const noexcept {
            if (_bytes.size() <= HEADER_SIZE + 1) {
                return Span<const u8>{};
            }
            return Span<const u8>{_bytes.data() + HEADER_SIZE, _bytes.size() - HEADER_SIZE - 1};
        }
    protected:
        /// Replace the stored bytes; used by the typed parse() functions.
        void assign_bytes(Span<const u8> bytes) {
            _bytes.assign(bytes.begin(), bytes.end());
        }

        /**
         * @internal
         * @brief Shared parse for the typed messages: well-formed, one of the
         * accepted categories, and a body of at least @p min_body bytes.
         */
        template <typename T>
        [[nodiscard]]
        static Optional<T> parse_as(Span<const u8> bytes, CiCategory first, CiCategory second, usize min_body) {
            if (!is_well_formed(bytes)) {
                return {};
            }
            const CiCategory category = static_cast<CiCategory>(bytes[4]);
            if (category != first && category != second) {
                return {};
            }
            if (bytes.size() - HEADER_SIZE - 1 < min_body) {
                return {};
            }
            T msg;
            static_cast<MidiCiMessage&>(msg).assign_bytes(bytes);
            return msg;
        }

        /// Append a 14-bit value as two 7-bit bytes, LSB first.
        static void push_u14(Vector<u8>& out, u16 value) {
            out.push_back(static_cast<u8>(value & 0x7F));
            out.push_back(static_cast<u8>((value >> 7) & 0x7F));
        }

        /// Read a 14-bit value from two 7-bit bytes, LSB first.
        [[nodiscard]]
        static constexpr u16 read_u14(const u8* p) noexcept {
            return static_cast<u16>((p[0] & 0x7F) | ((p[1] & 0x7F) << 7));
        }
    };

    /**
     * @class DiscoveryMessage
     * @brief A MIDI-CI Discovery Inquiry (sub-ID#2 0x70) or its reply (0x71).
     * @extends MidiCiMessage
     *
     * Discovery announces a device and the MIDI-CI categories it supports.
     * The body, in order:
     *
     * | bytes | field                                          |
     * |-------|------------------------------------------------|
     * | 3     | manufacturer SysEx id                          |
     * | 2     | device family (14-bit, LSB first)              |
     * | 2     | family model number (14-bit, LSB first)        |
     * | 4     | software revision                              |
     * | 1     | supported-category bitmap (CAP_*)              |
     * | 4     | max receivable SysEx size (28-bit, LSB first)  |
     * | 1     | output path instance id (1.2)                  |
     * | 1     | function block, replies only (1.2)             |
     */
    class DiscoveryMessage final: public MidiCiMessage {
        static constexpr usize BODY_1_1 = 16; ///< Body length without the 1.2 additions.

        static Vector<u8> build_body(
            bool reply, const u8 manufacturer[3], u16 family, u16 model, const u8 revision[4],
            u8 capabilities, u32 max_sysex_size, u8 output_path_id, u8 function_block
        ) {
            Vector<u8> body;
            body.push_back(manufacturer[0]);
            body.push_back(manufacturer[1]);
            body.push_back(manufacturer[2]);
            push_u14(body, family);
            push_u14(body, model);
            body.push_back(revision[0]);
            body.push_back(revision[1]);
            body.push_back(revision[2]);
            body.push_back(revision[3]);
            body.push_back(static_cast<u8>(capabilities & 0x7F));
            body.push_back(static_cast<u8>(max_sysex_size & 0x7F));
            body.push_back(static_cast<u8>((max_sysex_size >> 7) & 0x7F));
            body.push_back(static_cast<u8>((max_sysex_size >> 14) & 0x7F));
            body.push_back(static_cast<u8>((max_sysex_size >> 21) & 0x7F));
            body.push_back(static_cast<u8>(output_path_id & 0x7F));
            if (reply) {
                body.push_back(static_cast<u8>(function_block & 0x7F));
            }
            return body;
        }
    public:
        /// Supported-category bitmap flags.
        static constexpr u8 CAP_PROTOCOL_NEGOTIATION = 0x02; ///< 1.1 only; deprecated and reserved in 1.2.
        static constexpr u8 CAP_PROFILE_CONFIGURATION = 0x04;
        static constexpr u8 CAP_PROPERTY_EXCHANGE = 0x08;
        static constexpr u8 CAP_PROCESS_INQUIRY = 0x10; ///< 1.2.

        /// Function-block value for a reply from a device with no function blocks.
        static constexpr u8 FUNCTION_BLOCK_NONE = 0x7F;

        DiscoveryMessage() = default;

        using MidiCiMessage::MidiCiMessage;

        /**
         * @brief Build a Discovery inquiry or reply.
         * @param reply false for an inquiry (0x70), true for a reply (0x71).
         * @param source The sender's MUID.
         * @param destination Broadcast for an inquiry; the inquirer's MUID for a reply.
         * @param manufacturer Three-byte manufacturer id (0x7D for private use).
         * @param family 14-bit device family.
         * @param model 14-bit device family model.
         * @param revision Four-byte software revision.
         * @param capabilities Supported-category bitmap (CAP_* flags).
         * @param max_sysex_size Largest SysEx the sender can receive.
         * @param output_path_id Which of the sender's outputs this went out on (0 if only one).
         * @param function_block Replies only: the replying function block
         * (0x00-0x1F), or FUNCTION_BLOCK_NONE.
         */
        [[nodiscard]]
        static DiscoveryMessage create(
            bool reply, Muid source, Muid destination,
            const u8 manufacturer[3], u16 family, u16 model, const u8 revision[4],
            u8 capabilities, u32 max_sysex_size,
            u8 output_path_id = 0, u8 function_block = FUNCTION_BLOCK_NONE
        ) {
            const Vector<u8> body = build_body(
                reply, manufacturer, family, model, revision,
                capabilities, max_sysex_size, output_path_id, function_block
            );
            return DiscoveryMessage{
                reply ? CiCategory::DISCOVERY_REPLY : CiCategory::DISCOVERY,
                source, destination,
                Span<const u8>{body.data(), body.size()}
            };
        }

        /// Parse a Discovery inquiry or reply (1.1 or 1.2 layout).
        [[nodiscard]]
        static Optional<DiscoveryMessage> parse(Span<const u8> bytes) {
            return parse_as<DiscoveryMessage>(bytes, CiCategory::DISCOVERY, CiCategory::DISCOVERY_REPLY, BODY_1_1);
        }

        /// @copydoc parse(Span<const u8>)
        [[nodiscard]]
        static Optional<DiscoveryMessage> parse(const MidiMessage& msg) {
            const Vector<u8> bytes = sysex_bytes(msg);
            return parse(Span<const u8>{bytes.data(), bytes.size()});
        }

        [[nodiscard]]
        bool is_reply() const noexcept {
            return category() == CiCategory::DISCOVERY_REPLY;
        }

        [[nodiscard]]
        Array<u8, 3> manufacturer() const noexcept {
            const Span<const u8> b = body();
            return b.size() >= 3 ? Array<u8, 3>{b[0], b[1], b[2]} : Array<u8, 3>{};
        }

        [[nodiscard]]
        u16 family() const noexcept {
            const Span<const u8> b = body();
            return b.size() >= 5 ? read_u14(&b[3]) : u16{0};
        }

        [[nodiscard]]
        u16 model() const noexcept {
            const Span<const u8> b = body();
            return b.size() >= 7 ? read_u14(&b[5]) : u16{0};
        }

        [[nodiscard]]
        Array<u8, 4> revision() const noexcept {
            const Span<const u8> b = body();
            return b.size() >= 11 ? Array<u8, 4>{b[7], b[8], b[9], b[10]} : Array<u8, 4>{};
        }

        /// The supported-category bitmap.
        [[nodiscard]]
        u8 capabilities() const noexcept {
            const Span<const u8> b = body();
            return b.size() > 11 ? b[11] : 0;
        }

        /// The largest SysEx the sender can receive (28-bit, from 4 LSB-first bytes).
        [[nodiscard]]
        u32 max_sysex_size() const noexcept {
            const Span<const u8> b = body();
            if (b.size() < 16) {
                return 0;
            }
            return static_cast<u32>(b[12] & 0x7F)
                | (static_cast<u32>(b[13] & 0x7F) << 7)
                | (static_cast<u32>(b[14] & 0x7F) << 14)
                | (static_cast<u32>(b[15] & 0x7F) << 21);
        }

        /// The output path instance id; empty for a 1.1 message.
        [[nodiscard]]
        Optional<u8> output_path_id() const noexcept {
            const Span<const u8> b = body();
            return b.size() > 16 ? Optional<u8>{b[16]} : Optional<u8>{};
        }

        /// A reply's function block; empty for an inquiry or a 1.1 reply.
        [[nodiscard]]
        Optional<u8> function_block() const noexcept {
            const Span<const u8> b = body();
            return is_reply() && b.size() > 17 ? Optional<u8>{b[17]} : Optional<u8>{};
        }
    };

    /**
     * @class InvalidateMuidMessage
     * @brief A MIDI-CI Invalidate MUID message (sub-ID#2 0x7E).
     * @extends MidiCiMessage
     *
     * Broadcast to tell every device to forget a MUID - typically one found
     * to be in use twice. The body is the target MUID.
     */
    class InvalidateMuidMessage final: public MidiCiMessage {
    public:
        InvalidateMuidMessage() = default;

        using MidiCiMessage::MidiCiMessage;

        /**
         * @param source The sender's MUID.
         * @param target The MUID to invalidate.
         */
        [[nodiscard]]
        static InvalidateMuidMessage create(Muid source, Muid target) {
            Vector<u8> body;
            target.to_bytes(body);
            return InvalidateMuidMessage{
                CiCategory::INVALIDATE_MUID, source, Muid::broadcast(),
                Span<const u8>{body.data(), body.size()}
            };
        }

        [[nodiscard]]
        static Optional<InvalidateMuidMessage> parse(Span<const u8> bytes) {
            return parse_as<InvalidateMuidMessage>(bytes, CiCategory::INVALIDATE_MUID, CiCategory::INVALIDATE_MUID, 4);
        }

        [[nodiscard]]
        static Optional<InvalidateMuidMessage> parse(const MidiMessage& msg) {
            const Vector<u8> bytes = sysex_bytes(msg);
            return parse(Span<const u8>{bytes.data(), bytes.size()});
        }

        /// The MUID being invalidated.
        [[nodiscard]]
        Muid target_muid() const noexcept {
            const Span<const u8> b = body();
            return b.size() >= 4 ? Muid::from_bytes(b.data()) : Muid{};
        }
    };

    /**
     * @class AckNakMessage
     * @brief A MIDI-CI ACK (sub-ID#2 0x7D) or NAK (0x7F).
     * @extends MidiCiMessage
     *
     * 1.2 gives both the same body, in order:
     *
     * | bytes | field                                         |
     * |-------|-----------------------------------------------|
     * | 1     | sub-ID#2 of the message being answered        |
     * | 1     | status code                                   |
     * | 1     | status data                                   |
     * | 5     | status details                                |
     * | 2     | message text length (14-bit, LSB first)       |
     * | n     | message text (7-bit ASCII)                    |
     *
     * A 1.1 NAK has no body at all; it parses, with every field reading as
     * absent or zero.
     */
    class AckNakMessage final: public MidiCiMessage {
        static constexpr usize FIXED_BODY = 10; ///< Body length before the message text.
    public:
        AckNakMessage() = default;

        using MidiCiMessage::MidiCiMessage;

        /**
         * @param ack true for ACK (0x7D), false for NAK (0x7F).
         * @param source The sender's MUID.
         * @param destination The MUID of the device whose message is answered.
         * @param original The category of the message being answered.
         * @param status_code The ACK/NAK status code (0x00 = none given).
         * @param status_data Data qualifying the status code.
         * @param details Five bytes of status-specific detail.
         * @param text Optional human-readable text; 7-bit ASCII.
         * @throws InvalidMidiDataException if @p text has a non-ASCII byte or
         * is longer than 16383 bytes.
         */
        [[nodiscard]]
        THROWS(InvalidMidiDataException)
        static AckNakMessage create(
            bool ack, Muid source, Muid destination, CiCategory original,
            u8 status_code = 0, u8 status_data = 0, Array<u8, 5> details = {},
            StringView text = {}
        ) {
            if (text.size() > 0x3FFF) {
                throw InvalidMidiDataException("ACK/NAK text too long");
            }
            Vector<u8> body;
            body.push_back(static_cast<u8>(original));
            body.push_back(static_cast<u8>(status_code & 0x7F));
            body.push_back(static_cast<u8>(status_data & 0x7F));
            for (const u8 b: details) {
                body.push_back(static_cast<u8>(b & 0x7F));
            }
            push_u14(body, static_cast<u16>(text.size()));
            for (const char c: text) {
                const u8 b = static_cast<u8>(c);
                if (b > 0x7F) {
                    throw InvalidMidiDataException("ACK/NAK text is not ASCII");
                }
                body.push_back(b);
            }
            return AckNakMessage{
                ack ? CiCategory::ACK : CiCategory::NAK, source, destination,
                Span<const u8>{body.data(), body.size()}
            };
        }

        [[nodiscard]]
        static Optional<AckNakMessage> parse(Span<const u8> bytes) {
            return parse_as<AckNakMessage>(bytes, CiCategory::ACK, CiCategory::NAK, 0);
        }

        [[nodiscard]]
        static Optional<AckNakMessage> parse(const MidiMessage& msg) {
            const Vector<u8> bytes = sysex_bytes(msg);
            return parse(Span<const u8>{bytes.data(), bytes.size()});
        }

        [[nodiscard]]
        bool is_ack() const noexcept {
            return category() == CiCategory::ACK;
        }

        /// The category of the answered message; empty for a 1.1 NAK.
        [[nodiscard]]
        Optional<CiCategory> original_category() const noexcept {
            const Span<const u8> b = body();
            return b.empty() ? Optional<CiCategory>{} : Optional<CiCategory>{static_cast<CiCategory>(b[0])};
        }

        [[nodiscard]]
        u8 status_code() const noexcept {
            const Span<const u8> b = body();
            return b.size() > 1 ? b[1] : 0;
        }

        [[nodiscard]]
        u8 status_data() const noexcept {
            const Span<const u8> b = body();
            return b.size() > 2 ? b[2] : 0;
        }

        [[nodiscard]]
        Array<u8, 5> details() const noexcept {
            const Span<const u8> b = body();
            return b.size() >= 8 ? Array<u8, 5>{b[3], b[4], b[5], b[6], b[7]} : Array<u8, 5>{};
        }

        /// The message text, clipped to what is actually present.
        [[nodiscard]]
        String text() const {
            const Span<const u8> b = body();
            if (b.size() < FIXED_BODY) {
                return ""s;
            }
            const usize declared = read_u14(&b[8]);
            const usize available = b.size() - FIXED_BODY;
            const usize n = declared < available ? declared : available;
            return String(reinterpret_cast<const char*>(b.data() + FIXED_BODY), n);
        }
    };

    /**
     * @class ProfileId
     * @brief A five-byte MIDI-CI Profile identifier.
     *
     * Standard profiles start with 0x7E; manufacturer profiles start with the
     * manufacturer's SysEx id. Every byte is 7-bit.
     */
    class ProfileId final {
        Array<u8, 5> _bytes = {};
    public:
        constexpr ProfileId() = default;

        /// @throws InvalidMidiDataException if a byte is not 7-bit.
        THROWS(InvalidMidiDataException)
        explicit constexpr ProfileId(Array<u8, 5> bytes):
            _bytes{bytes} {
            for (const u8 b: bytes) {
                if (b > 0x7F) {
                    throw InvalidMidiDataException("profile id byte is not 7-bit");
                }
            }
        }

        [[nodiscard]]
        constexpr const Array<u8, 5>& bytes() const noexcept {
            return _bytes;
        }

        // Written out rather than defaulted: GCC 16 ICEs (segfault) on a
        // defaulted friend operator== here when a TU that imports the module
        // compares two ProfileIds.
        [[nodiscard]]
        friend constexpr bool operator==(const ProfileId& left, const ProfileId& right) noexcept {
            for (usize i = 0; i < 5; ++i) {
                if (left._bytes[i] != right._bytes[i]) {
                    return false;
                }
            }
            return true;
        }
    };

    /**
     * @class ProfileInquiryMessage
     * @brief A MIDI-CI Profile Configuration inquiry (sub-ID#2 0x20).
     * @extends MidiCiMessage
     *
     * Asks a device which MIDI-CI Profiles it supports on the channel, group
     * or function block named by the device id. The inquiry carries no body.
     */
    class ProfileInquiryMessage final: public MidiCiMessage {
    public:
        ProfileInquiryMessage() = default;

        using MidiCiMessage::MidiCiMessage;

        [[nodiscard]]
        static ProfileInquiryMessage create(Muid source, Muid destination, u8 device_id = DEVICE_ID_FUNCTION_BLOCK) {
            return ProfileInquiryMessage{
                CiCategory::PROFILE_INQUIRY, source, destination, Span<const u8>{}, device_id
            };
        }

        [[nodiscard]]
        static Optional<ProfileInquiryMessage> parse(Span<const u8> bytes) {
            return parse_as<ProfileInquiryMessage>(bytes, CiCategory::PROFILE_INQUIRY, CiCategory::PROFILE_INQUIRY, 0);
        }

        [[nodiscard]]
        static Optional<ProfileInquiryMessage> parse(const MidiMessage& msg) {
            const Vector<u8> bytes = sysex_bytes(msg);
            return parse(Span<const u8>{bytes.data(), bytes.size()});
        }
    };

    /**
     * @class ProfileInquiryReplyMessage
     * @brief The reply to a Profile Inquiry (sub-ID#2 0x21).
     * @extends MidiCiMessage
     *
     * Lists the profiles currently enabled, then those supported but
     * disabled. Each list is a 14-bit count (LSB first) followed by that many
     * five-byte profile ids.
     */
    class ProfileInquiryReplyMessage final: public MidiCiMessage {
        /**
         * @internal
         * @brief Read the profile list starting at @p offset in @p b.
         * @return The ids, and the offset just past the list (or b.size() if truncated).
         */
        [[nodiscard]]
        static Vector<ProfileId> read_list(Span<const u8> b, usize& offset) {
            Vector<ProfileId> out;
            if (offset + 2 > b.size()) {
                offset = b.size();
                return out;
            }
            const usize count = read_u14(&b[offset]);
            offset += 2;
            for (usize i = 0; i < count && offset + 5 <= b.size(); ++i, offset += 5) {
                out.push_back(ProfileId{Array<u8, 5>{b[offset], b[offset + 1], b[offset + 2], b[offset + 3], b[offset + 4]}});
            }
            return out;
        }

        static void write_list(Vector<u8>& body, Span<const ProfileId> list) {
            push_u14(body, static_cast<u16>(list.size()));
            for (const ProfileId& id: list) {
                body.insert(body.end(), id.bytes().begin(), id.bytes().end());
            }
        }
    public:
        ProfileInquiryReplyMessage() = default;

        using MidiCiMessage::MidiCiMessage;

        /**
         * @param source The replying device's MUID.
         * @param destination The inquirer's MUID.
         * @param enabled Profiles currently enabled.
         * @param disabled Profiles supported but currently disabled.
         * @param device_id The channel, group or function block the lists describe.
         * @throws InvalidMidiDataException if either list has more than 16383 entries.
         */
        [[nodiscard]]
        THROWS(InvalidMidiDataException)
        static ProfileInquiryReplyMessage create(
            Muid source, Muid destination,
            Span<const ProfileId> enabled, Span<const ProfileId> disabled,
            u8 device_id = DEVICE_ID_FUNCTION_BLOCK
        ) {
            if (enabled.size() > 0x3FFF || disabled.size() > 0x3FFF) {
                throw InvalidMidiDataException("too many profiles");
            }
            Vector<u8> body;
            write_list(body, enabled);
            write_list(body, disabled);
            return ProfileInquiryReplyMessage{
                CiCategory::PROFILE_INQUIRY_REPLY, source, destination,
                Span<const u8>{body.data(), body.size()}, device_id
            };
        }

        [[nodiscard]]
        static Optional<ProfileInquiryReplyMessage> parse(Span<const u8> bytes) {
            return parse_as<ProfileInquiryReplyMessage>(
                bytes, CiCategory::PROFILE_INQUIRY_REPLY, CiCategory::PROFILE_INQUIRY_REPLY, 4
            );
        }

        [[nodiscard]]
        static Optional<ProfileInquiryReplyMessage> parse(const MidiMessage& msg) {
            const Vector<u8> bytes = sysex_bytes(msg);
            return parse(Span<const u8>{bytes.data(), bytes.size()});
        }

        /// The profiles currently enabled.
        [[nodiscard]]
        Vector<ProfileId> enabled_profiles() const {
            usize offset = 0;
            return read_list(body(), offset);
        }

        /// The profiles supported but currently disabled.
        [[nodiscard]]
        Vector<ProfileId> disabled_profiles() const {
            const Span<const u8> b = body();
            usize offset = 0;
            static_cast<void>(read_list(b, offset));
            return read_list(b, offset);
        }
    };

    /**
     * @class PropertyExchangeCapabilitiesMessage
     * @brief A MIDI-CI Property Exchange capabilities inquiry (sub-ID#2 0x30)
     * or reply (0x31).
     * @extends MidiCiMessage
     *
     * Negotiates how many simultaneous Property Exchange requests the devices
     * will allow. The 1.2 body is three bytes: the number of supported
     * requests, then the Property Exchange major and minor version. A 1.1 body
     * has only the first.
     */
    class PropertyExchangeCapabilitiesMessage final: public MidiCiMessage {
    public:
        PropertyExchangeCapabilitiesMessage() = default;

        using MidiCiMessage::MidiCiMessage;

        /**
         * @param source The sender's MUID.
         * @param destination The other device's MUID.
         * @param supported_requests Simultaneous requests this device allows.
         * @param reply false for the inquiry (0x30), true for the reply (0x31).
         * @param major_version Property Exchange major version.
         * @param minor_version Property Exchange minor version.
         */
        [[nodiscard]]
        static PropertyExchangeCapabilitiesMessage create(
            Muid source, Muid destination, u8 supported_requests,
            bool reply = false, u8 major_version = 0, u8 minor_version = 0
        ) {
            const u8 body[] = {
                static_cast<u8>(supported_requests & 0x7F),
                static_cast<u8>(major_version & 0x7F),
                static_cast<u8>(minor_version & 0x7F),
            };
            return PropertyExchangeCapabilitiesMessage{
                reply ? CiCategory::PROPERTY_EXCHANGE_CAPABILITIES_REPLY : CiCategory::PROPERTY_EXCHANGE_CAPABILITIES,
                source, destination,
                Span<const u8>{body, 3}
            };
        }

        [[nodiscard]]
        static Optional<PropertyExchangeCapabilitiesMessage> parse(Span<const u8> bytes) {
            return parse_as<PropertyExchangeCapabilitiesMessage>(
                bytes,
                CiCategory::PROPERTY_EXCHANGE_CAPABILITIES,
                CiCategory::PROPERTY_EXCHANGE_CAPABILITIES_REPLY,
                1
            );
        }

        [[nodiscard]]
        static Optional<PropertyExchangeCapabilitiesMessage> parse(const MidiMessage& msg) {
            const Vector<u8> bytes = sysex_bytes(msg);
            return parse(Span<const u8>{bytes.data(), bytes.size()});
        }

        [[nodiscard]]
        bool is_reply() const noexcept {
            return category() == CiCategory::PROPERTY_EXCHANGE_CAPABILITIES_REPLY;
        }

        /// The number of simultaneous Property Exchange requests supported.
        [[nodiscard]]
        u8 supported_requests() const noexcept {
            const Span<const u8> b = body();
            return b.empty() ? 0 : b[0];
        }

        /// Property Exchange major version; empty for a 1.1 message.
        [[nodiscard]]
        Optional<u8> major_version() const noexcept {
            const Span<const u8> b = body();
            return b.size() > 1 ? Optional<u8>{b[1]} : Optional<u8>{};
        }

        /// Property Exchange minor version; empty for a 1.1 message.
        [[nodiscard]]
        Optional<u8> minor_version() const noexcept {
            const Span<const u8> b = body();
            return b.size() > 2 ? Optional<u8>{b[2]} : Optional<u8>{};
        }
    };
}
