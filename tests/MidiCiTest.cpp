import stdx;

using stdx::collections::Vector;

using namespace stdx::test;
using namespace stdx::audio::midi;

// -- Muid: 28-bit identity and wire encoding ---------------------------------

static_assert(Muid::broadcast().value() == 0x0FFFFFFFu, "broadcast is all 28 bits set");
static_assert(Muid::broadcast().is_broadcast(), "broadcast reports itself as broadcast");
static_assert(Muid{0x12345678u}.value() == (0x12345678u & 0x0FFFFFFFu), "a MUID masks to 28 bits");

void test_muid_wire_roundtrip() {
    Muid m{0x0A1B2C3Du};
    Vector<u8> bytes;
    m.to_bytes(bytes);
    require(bytes.size() == 4, "a MUID is four wire bytes");
    for (const u8 b: bytes) {
        expect(b <= 0x7F, "every MUID wire byte is 7-bit");
    }
    expect(Muid::from_bytes(bytes.data()) == m, "a MUID round-trips through its wire bytes");
}

void test_muid_random_is_not_broadcast() {
    for (int i = 0; i < 64; ++i) {
        expect(!Muid::random().is_broadcast(), "a random MUID is never the broadcast value");
    }
}

// -- Discovery ---------------------------------------------------------------

void test_discovery_header() {
    const u8 manufacturer[3] = {0x7D, 0x00, 0x00};
    const u8 revision[4] = {0x00, 0x00, 0x00, 0x01};
    Muid src{0x01020304u};
    DiscoveryMessage msg = DiscoveryMessage::create(
        false, src, Muid::broadcast(),
        manufacturer, 0x0001, 0x0001, revision,
        DiscoveryMessage::CAP_PROFILE_CONFIGURATION | DiscoveryMessage::CAP_PROPERTY_EXCHANGE,
        512
    );

    expect(msg.is_midi_ci(), "a discovery message is recognizable as MIDI-CI");
    expect(msg.category() == CiCategory::DISCOVERY, "an inquiry has the discovery category");
    expect_eq(msg.device_id(), u8{0x7F}, "discovery addresses the whole port");
    expect_eq(msg.ci_version(), MidiCiMessage::CI_VERSION_1_2, "the CI version is set");
    expect(msg.source_muid() == src, "the source MUID round-trips");
    expect(msg.destination_muid().is_broadcast(), "an inquiry is broadcast");

    // Raw bytes form a complete F0..F7 SysEx.
    const Span<const u8> raw = msg.data();
    expect_eq(raw[0], u8{0xF0}, "starts with F0");
    expect_eq(raw[raw.size() - 1], u8{0xF7}, "ends with F7");
    expect_eq(raw[1], u8{0x7E}, "universal non-realtime SysEx");
    expect_eq(raw[3], u8{0x0D}, "sub-id#1 marks MIDI-CI");
}

void test_discovery_body_fields() {
    const u8 manufacturer[3] = {0x7D, 0x00, 0x00};
    const u8 revision[4] = {0x00, 0x00, 0x00, 0x01};
    const u8 caps = DiscoveryMessage::CAP_PROTOCOL_NEGOTIATION
        | DiscoveryMessage::CAP_PROFILE_CONFIGURATION
        | DiscoveryMessage::CAP_PROPERTY_EXCHANGE;
    DiscoveryMessage msg = DiscoveryMessage::create(
        false, Muid::random(), Muid::broadcast(),
        manufacturer, 0x0102, 0x0304, revision, caps, 512
    );
    expect_eq(msg.capabilities(), caps, "the capability bitmap round-trips");
    expect_eq(msg.max_sysex_size(), 512u, "the max SysEx size round-trips from its 7-bit bytes");
}

void test_discovery_reply_category() {
    const u8 manufacturer[3] = {0x7D, 0x00, 0x00};
    const u8 revision[4] = {0x00, 0x00, 0x00, 0x01};
    Muid responder{0x0ABCDEFu};
    Muid inquirer{0x0123456u};
    DiscoveryMessage reply = DiscoveryMessage::create(
        true, responder, inquirer,
        manufacturer, 0x0001, 0x0001, revision, DiscoveryMessage::CAP_PROFILE_CONFIGURATION, 512
    );
    expect(reply.category() == CiCategory::DISCOVERY_REPLY, "a reply uses the reply category");
    expect(reply.source_muid() == responder, "the responder is the source");
    expect(reply.destination_muid() == inquirer, "the reply is addressed back to the inquirer");
}

// -- Profile Configuration & Property Exchange -------------------------------

void test_profile_inquiry() {
    Muid src{0x0111111u};
    Muid dst{0x0222222u};
    ProfileInquiryMessage msg = ProfileInquiryMessage::create(src, dst);
    expect(msg.is_midi_ci(), "a profile inquiry is a MIDI-CI message");
    expect(msg.category() == CiCategory::PROFILE_INQUIRY, "it carries the profile-inquiry category");
    expect(msg.source_muid() == src, "source MUID round-trips");
    expect(msg.destination_muid() == dst, "a profile inquiry is addressed to a specific device");
    expect(msg.body().empty(), "a profile inquiry carries no body");
}

void test_property_exchange_capabilities() {
    Muid src{0x0333333u};
    Muid dst{0x0444444u};
    PropertyExchangeCapabilitiesMessage msg =
        PropertyExchangeCapabilitiesMessage::create(src, dst, 4);
    expect(msg.category() == CiCategory::PROPERTY_EXCHANGE_CAPABILITIES, "the PE capabilities category");
    expect_eq(msg.supported_requests(), u8{4}, "the supported-request count round-trips");
}

// -- MIDI-CI 1.2 fields ------------------------------------------------------

namespace {
    const u8 MANUFACTURER[3] = {0x7D, 0x00, 0x00};
    const u8 REVISION[4] = {0x01, 0x02, 0x03, 0x04};

    [[nodiscard]]
    Span<const u8> bytes_of(const MidiMessage& msg) {
        return msg.data();
    }
}

void test_muid_random_avoids_reserved() {
    for (int i = 0; i < 256; ++i) {
        expect(!Muid::random().is_reserved(), "a random MUID never lands in the reserved block");
    }
    expect(Muid{0x0FFFFF00u}.is_reserved(), "the reserved block starts at 0x0FFFFF00");
    expect(!Muid{0x0FFFFEFFu}.is_reserved(), "and nothing below it is reserved");
}

void test_discovery_1_2_fields() {
    DiscoveryMessage inquiry = DiscoveryMessage::create(
        false, Muid{0x111u}, Muid::broadcast(), MANUFACTURER, 0x0102, 0x0304, REVISION,
        DiscoveryMessage::CAP_PROCESS_INQUIRY, 4096, 3
    );
    expect_eq(inquiry.body().size(), 17uz, "a 1.2 inquiry body ends with the output path id");
    expect(inquiry.output_path_id() == Optional<u8>{3}, "the output path id round-trips");
    expect(!inquiry.function_block().has_value(), "an inquiry carries no function block");
    expect_eq(inquiry.family(), u16{0x0102}, "family round-trips");
    expect_eq(inquiry.model(), u16{0x0304}, "model round-trips");
    expect(inquiry.revision() == Array<u8, 4>{0x01, 0x02, 0x03, 0x04}, "revision round-trips");
    expect(inquiry.manufacturer() == Array<u8, 3>{0x7D, 0x00, 0x00}, "manufacturer round-trips");

    DiscoveryMessage reply = DiscoveryMessage::create(
        true, Muid{0x222u}, Muid{0x111u}, MANUFACTURER, 1, 1, REVISION, 0, 512, 0, 5
    );
    expect_eq(reply.body().size(), 18uz, "a 1.2 reply also carries the function block");
    expect(reply.function_block() == Optional<u8>{5}, "the function block round-trips");
}

void test_discovery_parse_roundtrip_and_1_1() {
    DiscoveryMessage original = DiscoveryMessage::create(
        true, Muid{0x0ABCDEFu}, Muid{0x0123456u}, MANUFACTURER, 7, 8, REVISION,
        DiscoveryMessage::CAP_PROPERTY_EXCHANGE, 1024, 1, 2
    );
    Optional<DiscoveryMessage> parsed = DiscoveryMessage::parse(bytes_of(original));
    require(parsed.has_value(), "a built discovery reply parses back");
    expect(parsed->is_reply(), "as a reply");
    expect(parsed->source_muid() == Muid{0x0ABCDEFu}, "with its source");
    expect_eq(parsed->max_sysex_size(), 1024u, "and its body");
    expect(parsed->function_block() == Optional<u8>{2}, "including the 1.2 fields");

    // A 1.1 inquiry: the 16-byte body with no output path id.
    Vector<u8> v11(original.data().begin(), original.data().end());
    v11[4] = static_cast<u8>(CiCategory::DISCOVERY);
    v11[5] = MidiCiMessage::CI_VERSION_1_1;
    v11.erase(v11.end() - 3, v11.end() - 1); // drop output path + function block, keep F7
    Optional<DiscoveryMessage> old = DiscoveryMessage::parse(Span<const u8>{v11.data(), v11.size()});
    require(old.has_value(), "a 1.1 discovery body still parses");
    expect(!old->output_path_id().has_value(), "with no output path id");
    expect_eq(old->max_sysex_size(), 1024u, "and its 1.1 fields intact");
}

void test_parse_rejects_malformed() {
    DiscoveryMessage msg = DiscoveryMessage::create(
        false, Muid{1u}, Muid::broadcast(), MANUFACTURER, 1, 1, REVISION, 0, 512
    );
    Vector<u8> bytes(msg.data().begin(), msg.data().end());

    expect(!InvalidateMuidMessage::parse(Span<const u8>{bytes.data(), bytes.size()}).has_value(),
        "a discovery does not parse as another category");

    Vector<u8> eight_bit = bytes;
    eight_bit[16] = 0x80;
    expect(!DiscoveryMessage::parse(Span<const u8>{eight_bit.data(), eight_bit.size()}).has_value(),
        "an 8-bit data byte is rejected");

    Vector<u8> truncated(bytes.begin(), bytes.begin() + 20);
    truncated.push_back(0xF7);
    expect(!DiscoveryMessage::parse(Span<const u8>{truncated.data(), truncated.size()}).has_value(),
        "a body too short for discovery is rejected");

    Vector<u8> no_eox(bytes.begin(), bytes.end() - 1);
    expect(!MidiCiMessage::parse(Span<const u8>{no_eox.data(), no_eox.size()}).has_value(),
        "a message without its F7 is rejected");
}

void test_parse_through_ump() {
    // MIDI-CI over UMP: out through an encoder as SysEx7, back through a parser.
    DiscoveryMessage original = DiscoveryMessage::create(
        false, Muid{0x0F0F0F0u}, Muid::broadcast(), MANUFACTURER, 9, 10, REVISION,
        DiscoveryMessage::CAP_PROFILE_CONFIGURATION, 2048, 4
    );
    UmpEncoder encoder;
    const Vector<UniversalMidiPacket> packets = encoder.encode(original);
    require(packets.size() > 1, "a discovery spans several SysEx7 packets");

    Vector<u32> words;
    for (const UniversalMidiPacket& p: packets) {
        for (u8 i = 0; i < p.word_count(); ++i) {
            words.push_back(p.word(i));
        }
    }
    Optional<DiscoveryMessage> parsed;
    UmpParser parser;
    parser.feed(Span<const u32>{words.data(), words.size()}, [&parsed](const MidiMessage& msg) -> void {
        parsed = DiscoveryMessage::parse(msg);
    });
    require(parsed.has_value(), "the reassembled SysEx7 parses as MIDI-CI discovery");
    expect(parsed->source_muid() == Muid{0x0F0F0F0u}, "with its source MUID");
    expect(parsed->output_path_id() == Optional<u8>{4}, "and every field");
    expect_eq(parsed->max_sysex_size(), 2048u, "intact");
}

// -- Management messages -----------------------------------------------------

void test_invalidate_muid() {
    InvalidateMuidMessage msg = InvalidateMuidMessage::create(Muid{0x10u}, Muid{0x0BADBADu});
    expect(msg.destination_muid().is_broadcast(), "invalidate is broadcast");
    Optional<InvalidateMuidMessage> parsed = InvalidateMuidMessage::parse(bytes_of(msg));
    require(parsed.has_value(), "it parses back");
    expect(parsed->target_muid() == Muid{0x0BADBADu}, "with its target");
}

void test_ack_nak() {
    AckNakMessage nak = AckNakMessage::create(
        false, Muid{1u}, Muid{2u}, CiCategory::PROFILE_INQUIRY, 0x20, 0x01,
        Array<u8, 5>{1, 2, 3, 4, 5}, "busy"
    );
    expect(nak.category() == CiCategory::NAK, "a NAK");
    Optional<AckNakMessage> parsed = AckNakMessage::parse(bytes_of(nak));
    require(parsed.has_value(), "parses back");
    expect(!parsed->is_ack(), "still a NAK");
    expect(parsed->original_category() == Optional<CiCategory>{CiCategory::PROFILE_INQUIRY}, "naming what it answers");
    expect_eq(parsed->status_code(), u8{0x20}, "status code");
    expect_eq(parsed->status_data(), u8{0x01}, "status data");
    expect(parsed->details() == Array<u8, 5>{1, 2, 3, 4, 5}, "details");
    expect_eq(parsed->text(), "busy"s, "and its text");

    AckNakMessage ack = AckNakMessage::create(true, Muid{1u}, Muid{2u}, CiCategory::DISCOVERY);
    expect(ack.category() == CiCategory::ACK, "an ACK uses 0x7D");
    expect(ack.text().empty(), "with no text by default");

    expect_throws<InvalidMidiDataException>(
        [] -> void { static_cast<void>(AckNakMessage::create(false, Muid{1u}, Muid{2u}, CiCategory::NAK, 0, 0, {}, "\xC3\xA9")); },
        "non-ASCII text is rejected"
    );

    // A 1.1 NAK: header only.
    MidiCiMessage bare{CiCategory::NAK, Muid{1u}, Muid{2u}, Span<const u8>{}, 0x7F, MidiCiMessage::CI_VERSION_1_1};
    Optional<AckNakMessage> old = AckNakMessage::parse(bytes_of(bare));
    require(old.has_value(), "a body-less 1.1 NAK parses");
    expect(!old->original_category().has_value(), "with nothing to report");
    expect(old->text().empty(), "and no text");
}

// -- Profiles and Property Exchange ------------------------------------------

void test_profile_inquiry_reply() {
    const ProfileId gm{Array<u8, 5>{0x7E, 0x00, 0x01, 0x01, 0x00}};
    const ProfileId vendor{Array<u8, 5>{0x00, 0x21, 0x09, 0x05, 0x01}};
    const ProfileId enabled[] = {gm};
    const ProfileId disabled[] = {vendor, gm};
    ProfileInquiryReplyMessage reply = ProfileInquiryReplyMessage::create(
        Muid{3u}, Muid{4u}, Span<const ProfileId>{enabled, 1}, Span<const ProfileId>{disabled, 2}, 0x05
    );
    expect_eq(reply.device_id(), u8{0x05}, "the reply describes channel 5");

    Optional<ProfileInquiryReplyMessage> parsed = ProfileInquiryReplyMessage::parse(bytes_of(reply));
    require(parsed.has_value(), "parses back");
    const Vector<ProfileId> on = parsed->enabled_profiles();
    const Vector<ProfileId> off = parsed->disabled_profiles();
    require(on.size() == 1 && off.size() == 2, "both lists come back at their lengths");
    expect(on[0] == gm, "the enabled profile");
    expect(off[0] == vendor && off[1] == gm, "the disabled profiles, in order");

    expect_throws<InvalidMidiDataException>(
        [] -> void { static_cast<void>(ProfileId{Array<u8, 5>{0x80, 0, 0, 0, 0}}); },
        "a profile id byte must be 7-bit"
    );
}

void test_property_exchange_capabilities_1_2() {
    PropertyExchangeCapabilitiesMessage reply = PropertyExchangeCapabilitiesMessage::create(
        Muid{5u}, Muid{6u}, 2, true, 1, 0
    );
    expect(reply.is_reply(), "a reply uses 0x31");
    Optional<PropertyExchangeCapabilitiesMessage> parsed = PropertyExchangeCapabilitiesMessage::parse(bytes_of(reply));
    require(parsed.has_value(), "parses back");
    expect_eq(parsed->supported_requests(), u8{2}, "request count");
    expect(parsed->major_version() == Optional<u8>{1}, "major version");
    expect(parsed->minor_version() == Optional<u8>{0}, "minor version");

    const u8 v11_body[] = {4};
    MidiCiMessage old{CiCategory::PROPERTY_EXCHANGE_CAPABILITIES, Muid{5u}, Muid{6u}, Span<const u8>{v11_body, 1}};
    Optional<PropertyExchangeCapabilitiesMessage> parsed_old = PropertyExchangeCapabilitiesMessage::parse(bytes_of(old));
    require(parsed_old.has_value(), "a 1.1 one-byte body parses");
    expect(!parsed_old->major_version().has_value(), "with no version");
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"MidiCi.muid_wire_roundtrip", test_muid_wire_roundtrip},
        {"MidiCi.muid_random_is_not_broadcast", test_muid_random_is_not_broadcast},
        {"MidiCi.discovery_header", test_discovery_header},
        {"MidiCi.discovery_body_fields", test_discovery_body_fields},
        {"MidiCi.discovery_reply_category", test_discovery_reply_category},
        {"MidiCi.profile_inquiry", test_profile_inquiry},
        {"MidiCi.property_exchange_capabilities", test_property_exchange_capabilities},
        {"MidiCi.muid_random_avoids_reserved", test_muid_random_avoids_reserved},
        {"MidiCi.discovery_1_2_fields", test_discovery_1_2_fields},
        {"MidiCi.discovery_parse_roundtrip_and_1_1", test_discovery_parse_roundtrip_and_1_1},
        {"MidiCi.parse_rejects_malformed", test_parse_rejects_malformed},
        {"MidiCi.parse_through_ump", test_parse_through_ump},
        {"MidiCi.invalidate_muid", test_invalidate_muid},
        {"MidiCi.ack_nak", test_ack_nak},
        {"MidiCi.profile_inquiry_reply", test_profile_inquiry_reply},
        {"MidiCi.property_exchange_capabilities_1_2", test_property_exchange_capabilities_1_2},
    });
}
