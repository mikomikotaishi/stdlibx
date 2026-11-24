import stdx;

using stdx::collections::Vector;
using stdx::mem::UniquePointer;

using namespace stdx::test;
using namespace stdx::audio::midi;

// -- UniversalMidiPacket: word layout and size discrimination ----------------

static_assert(
    UniversalMidiPacket{0x10FA0000u}.type() == MessageType::SYSTEM,
    "top nibble decodes to the Message Type"
);
static_assert(
    UniversalMidiPacket{0x1AFA0000u}.group() == 0xA,
    "second nibble decodes to the group"
);
static_assert(UniversalMidiPacket{0u}.word_count() == 1, "one-word ctor is a 32-bit packet");
static_assert(UniversalMidiPacket{0u, 0u}.word_count() == 2, "two-word ctor is a 64-bit packet");
static_assert(UniversalMidiPacket{0u, 0u, 0u, 0u}.word_count() == 4, "four-word ctor is a 128-bit packet");

static_assert(UniversalMidiPacket::words_for(MessageType::UTILITY) == 1, "utility is 32-bit");
static_assert(UniversalMidiPacket::words_for(MessageType::MIDI2_CHANNEL_VOICE) == 2, "m2cv is 64-bit");
static_assert(UniversalMidiPacket::words_for(MessageType::STREAM) == 4, "stream is 128-bit");

void test_packet_word_access() {
    UniversalMidiPacket p{0x11223344u, 0x55667788u};
    expect_eq(p.word(0), 0x11223344u, "word 0 reads back");
    expect_eq(p.word(1), 0x55667788u, "word 1 reads back");
    expect_eq(p.word(2), 0u, "an out-of-range word reads 0, not garbage");
    expect_eq(p.word_count(), u8{2}, "a two-word packet reports two words");
}

void test_packet_equality() {
    expect(
        UniversalMidiPacket{0xDEADu, 0xBEEFu} == UniversalMidiPacket{0xDEADu, 0xBEEFu},
        "identical two-word packets compare equal"
    );
    expect(
        !(UniversalMidiPacket{0xDEADu} == UniversalMidiPacket{0xDEADu, 0u}),
        "packets of different word counts are never equal, even when the extra word is zero"
    );
}

// -- UmpMessage: the MidiMessage byte-order bridge ---------------------------

void test_ump_bytes_are_big_endian() {
    UmpMessage msg{UniversalMidiPacket{0x11223344u}};
    const Span<const u8> bytes = msg.data();
    require(bytes.size() == 4, "a 32-bit packet is four bytes");
    expect_eq(bytes[0], u8{0x11}, "MSB first");
    expect_eq(bytes[1], u8{0x22}, "byte 1");
    expect_eq(bytes[2], u8{0x33}, "byte 2");
    expect_eq(bytes[3], u8{0x44}, "LSB last");
    expect_eq(msg.length(), 4uz, "length tracks the byte span");
}

// -- UtilityMessage ----------------------------------------------------------

void test_utility_noop() {
    UtilityMessage noop = UtilityMessage::noop();
    expect(noop.type() == MessageType::UTILITY, "a NOOP is a utility message");
    expect(noop.utility_status() == UtilityStatus::NOOP, "status decodes to NOOP");
    expect_eq(noop.value(), u16{0}, "NOOP carries no payload");
}

void test_utility_jr_timestamp_roundtrips() {
    UtilityMessage ts = UtilityMessage::jr_timestamp(0xABCD);
    expect(ts.type() == MessageType::UTILITY, "a JR timestamp is a utility message");
    expect(ts.utility_status() == UtilityStatus::JR_TIMESTAMP, "status decodes to JR_TIMESTAMP");
    expect_eq(ts.value(), u16{0xABCD}, "the 16-bit clock value round-trips");

    UtilityMessage clk = UtilityMessage::jr_clock(0x1234);
    expect(clk.utility_status() == UtilityStatus::JR_CLOCK, "status decodes to JR_CLOCK");
    expect_eq(clk.value(), u16{0x1234}, "the JR clock value round-trips");
}

// -- SystemMessage -----------------------------------------------------------

void test_system_message_roundtrips() {
    SystemMessage clock{5, MidiStatus::TIMING_CLOCK};
    expect(clock.type() == MessageType::SYSTEM, "a system message carries the SYSTEM type");
    expect_eq(clock.group(), u8{5}, "the group round-trips");
    expect(clock.system_status() == MidiStatus::TIMING_CLOCK, "the status byte round-trips");
    expect_eq(clock.data1(), u8{0}, "Timing Clock carries no data");

    SystemMessage spp{0, MidiStatus::SONG_POSITION, 0x7F, 0x40};
    expect(spp.system_status() == MidiStatus::SONG_POSITION, "Song Position status round-trips");
    expect_eq(spp.data1(), u8{0x7F}, "data1 round-trips");
    expect_eq(spp.data2(), u8{0x40}, "data2 round-trips");
}

void test_system_message_rejects_bad_input() {
    expect_throws<InvalidMidiDataException>(
        [] -> void { static_cast<void>(SystemMessage(16, MidiStatus::TIMING_CLOCK)); },
        "a group above 15 is rejected"
    );
    expect_throws<InvalidMidiDataException>(
        [] -> void { static_cast<void>(SystemMessage(0, MidiStatus::SONG_POSITION, 128)); },
        "a data byte above 127 is rejected"
    );
}

// -- Resolution scaling ------------------------------------------------------

// Endpoints and centre are preserved exactly across widths.
static_assert(scale::up_7_to_16(0x00) == 0x0000, "7->16 keeps the minimum");
static_assert(scale::up_7_to_16(0x40) == 0x8000, "7->16 keeps the centre");
static_assert(scale::up_7_to_16(0x7F) == 0xFFFF, "7->16 keeps the maximum");
static_assert(scale::up_7_to_32(0x00) == 0x00000000u, "7->32 keeps the minimum");
static_assert(scale::up_7_to_32(0x40) == 0x80000000u, "7->32 keeps the centre");
static_assert(scale::up_7_to_32(0x7F) == 0xFFFFFFFFu, "7->32 keeps the maximum");
static_assert(scale::up_14_to_32(0x0000) == 0x00000000u, "14->32 keeps the minimum");
static_assert(scale::up_14_to_32(0x2000) == 0x80000000u, "14->32 keeps the centre");
static_assert(scale::up_14_to_32(0x3FFF) == 0xFFFFFFFFu, "14->32 keeps the maximum");

static_assert(scale::down_16_to_7(scale::up_7_to_16(0x40)) == 0x40, "7->16->7 preserves the centre");
static_assert(scale::down_32_to_7(scale::up_7_to_32(0x7F)) == 0x7F, "7->32->7 preserves the max");
static_assert(scale::down_32_to_14(scale::up_14_to_32(0x2000)) == 0x2000, "14->32->14 preserves the centre");

// -- MIDI 2.0 channel voice --------------------------------------------------

void test_midi2_note_on() {
    Midi2ChannelVoiceMessage note = Midi2ChannelVoiceMessage::note_on(3, 9, 60, 0xC000);
    expect(note.type() == MessageType::MIDI2_CHANNEL_VOICE, "note on is a 64-bit channel-voice packet");
    expect_eq(note.packet().word_count(), u8{2}, "a MIDI 2.0 channel-voice message is two words");
    expect_eq(note.group(), u8{3}, "group round-trips");
    expect(note.command() == Midi2Status::NOTE_ON, "status decodes to NOTE_ON");
    expect_eq(note.channel(), u8{9}, "channel round-trips");
    expect_eq(note.index1(), u8{60}, "note number round-trips");
    expect_eq(note.velocity(), u16{0xC000}, "16-bit velocity round-trips");
}

void test_midi2_control_change() {
    Midi2ChannelVoiceMessage cc = Midi2ChannelVoiceMessage::control_change(0, 0, 74, 0xDEADBEEFu);
    expect(cc.command() == Midi2Status::CONTROL_CHANGE, "status decodes to CONTROL_CHANGE");
    expect_eq(cc.index1(), u8{74}, "controller index round-trips");
    expect_eq(cc.value(), 0xDEADBEEFu, "32-bit controller value round-trips");
}

void test_midi2_rejects_bad_input() {
    expect_throws<InvalidMidiDataException>(
        [] -> void { static_cast<void>(Midi2ChannelVoiceMessage::note_on(0, 16, 60, 0)); },
        "a channel above 15 is rejected"
    );
    expect_throws<InvalidMidiDataException>(
        [] -> void { static_cast<void>(Midi2ChannelVoiceMessage::note_on(0, 0, 128, 0)); },
        "a note above 127 is rejected"
    );
}

void test_midi1_in_ump() {
    Midi1ChannelVoiceMessage m{2, MidiStatus::NOTE_ON, 5, 64, 100};
    expect(m.type() == MessageType::MIDI1_CHANNEL_VOICE, "a MIDI 1.0 channel voice in UMP form");
    expect_eq(m.packet().word_count(), u8{1}, "it is a single 32-bit word");
    expect(m.command() == MidiStatus::NOTE_ON, "status round-trips");
    expect_eq(m.channel(), u8{5}, "channel round-trips");
    expect_eq(m.data1(), u8{64}, "data1 round-trips");
    expect_eq(m.data2(), u8{100}, "data2 round-trips");
}

// -- 1.0 <-> 2.0 translation -------------------------------------------------

void test_translate_note_on_to_midi2() {
    ShortMessage note{MidiStatus::NOTE_ON, 4, 60, 0x7F};
    Optional<Midi2ChannelVoiceMessage> up = MidiTranslator::to_midi2(note, 1);
    require(up.has_value(), "note on has a MIDI 2.0 counterpart");
    expect_eq(up->group(), u8{1}, "the requested group is applied");
    expect(up->command() == Midi2Status::NOTE_ON, "it stays a note on");
    expect_eq(up->channel(), u8{4}, "channel is preserved");
    expect_eq(up->index1(), u8{60}, "note number is preserved");
    expect_eq(up->velocity(), u16{0xFFFF}, "max 7-bit velocity widens to the 16-bit max");
}

void test_translate_note_roundtrip_preserves_center() {
    // A centred value (0x40) must survive 1.0 -> 2.0 -> 1.0 exactly.
    ShortMessage cc{MidiStatus::CONTROL_CHANGE, 0, 74, 0x40};
    Optional<Midi2ChannelVoiceMessage> up = MidiTranslator::to_midi2(cc);
    require(up.has_value(), "control change up-converts");
    expect_eq(up->value(), 0x80000000u, "a centred 7-bit value lands on the 32-bit centre");

    Optional<ShortMessage> down = MidiTranslator::to_midi1(*up);
    require(down.has_value(), "it comes back down");
    expect(down->command() == MidiStatus::CONTROL_CHANGE, "still a control change");
    expect_eq(down->data1(), u8{74}, "controller index survives the round trip");
    expect_eq(down->data2(), u8{0x40}, "the centred value survives the round trip exactly");
}

void test_translate_pitch_bend_roundtrip() {
    // 14-bit centre 0x2000 split into lsb/msb.
    ShortMessage bend{MidiStatus::PITCH_BEND, 7, 0x00, 0x40};
    Optional<Midi2ChannelVoiceMessage> up = MidiTranslator::to_midi2(bend);
    require(up.has_value(), "pitch bend up-converts");
    expect_eq(up->value(), 0x80000000u, "14-bit centre maps to the 32-bit centre");

    Optional<ShortMessage> down = MidiTranslator::to_midi1(*up);
    require(down.has_value(), "pitch bend comes back down");
    expect(down->command() == MidiStatus::PITCH_BEND, "still a pitch bend");
    expect_eq(down->data1(), u8{0x00}, "LSB survives");
    expect_eq(down->data2(), u8{0x40}, "MSB survives");
}

void test_translate_rejects_non_channel_voice() {
    // A per-note controller has no MIDI 1.0 counterpart.
    Midi2ChannelVoiceMessage pnc{
        0, Midi2Status::REGISTERED_PER_NOTE_CONTROLLER, 0, 60, 1, 0x12345678u
    };
    Optional<ShortMessage> down = MidiTranslator::to_midi1(pnc);
    expect(!down.has_value(), "a per-note controller has no MIDI 1.0 form");
}

// -- SysEx7 ------------------------------------------------------------------

// Helper: read the SysEx status nibble out of a 64-bit SysEx7 packet.
static SysexStatus sysex7_status(const UniversalMidiPacket& p) {
    return static_cast<SysexStatus>((p.word(0) >> 20) & 0x0F);
}

void test_sysex7_single_packet() {
    const u8 body[] = {0x7E, 0x00, 0x09, 0x01}; // four 7-bit bytes, fits in one packet
    Sysex7Message msg = Sysex7Message::from_bytes(2, Span<const u8>{body, sizeof(body)});
    require(msg.packets().size() == 1, "four bytes fit in a single SysEx7 packet");
    expect(msg.packets()[0].type() == MessageType::DATA_64, "a SysEx7 packet is a 64-bit data packet");
    expect(sysex7_status(msg.packets()[0]) == SysexStatus::COMPLETE, "a one-packet SysEx is COMPLETE");
    expect_eq(msg.group(), u8{2}, "group round-trips");

    Vector<u8> back = msg.payload();
    require(back.size() == sizeof(body), "the payload reassembles to its original length");
    for (usize i = 0; i < sizeof(body); ++i) {
        expect_eq(back[i], body[i], "each data byte round-trips");
    }
}

void test_sysex7_multi_packet() {
    // 14 bytes -> 6 + 6 + 2 across three packets (START, CONTINUE, END).
    Vector<u8> body;
    for (u8 i = 0; i < 14; ++i) {
        body.push_back(static_cast<u8>(i + 1));
    }
    Sysex7Message msg = Sysex7Message::from_bytes(0, Span<const u8>{body.data(), body.size()});
    require(msg.packets().size() == 3, "14 bytes span three 6-byte SysEx7 packets");
    expect(sysex7_status(msg.packets()[0]) == SysexStatus::START, "the first packet is START");
    expect(sysex7_status(msg.packets()[1]) == SysexStatus::CONTINUE, "the middle packet is CONTINUE");
    expect(sysex7_status(msg.packets()[2]) == SysexStatus::END, "the last packet is END");

    Vector<u8> back = msg.payload();
    require(back.size() == body.size(), "the full payload reassembles");
    expect_eq(back[0], u8{1}, "first byte");
    expect_eq(back[13], u8{14}, "last byte");
}

void test_sysex7_rejects_non_7bit() {
    const u8 bad[] = {0x7F, 0x80}; // 0x80 has the high bit set
    expect_throws<InvalidMidiDataException>(
        [&] -> void { static_cast<void>(Sysex7Message::from_bytes(0, Span<const u8>{bad, 2})); },
        "an 8-bit byte is rejected by SysEx7"
    );
}

// -- SysEx8 ------------------------------------------------------------------

void test_sysex8_single_packet() {
    // Full 8-bit bytes, including values above 0x7F.
    const u8 body[] = {0x00, 0xFF, 0x80, 0x7F, 0x01};
    Sysex8Message msg = Sysex8Message::from_bytes(4, 0xAB, Span<const u8>{body, sizeof(body)});
    require(msg.packets().size() == 1, "five bytes fit in a single SysEx8 packet");
    expect(msg.packets()[0].type() == MessageType::DATA_128, "a SysEx8 packet is a 128-bit data packet");
    expect_eq(msg.packets()[0].word_count(), u8{4}, "it occupies four words");
    expect_eq(msg.group(), u8{4}, "group round-trips");
    expect_eq(msg.stream_id(), u8{0xAB}, "the stream id round-trips");

    Vector<u8> back = msg.payload();
    require(back.size() == sizeof(body), "the payload reassembles to its original length");
    expect_eq(back[1], u8{0xFF}, "a full 8-bit byte survives (SysEx8 is not 7-bit)");
    expect_eq(back[2], u8{0x80}, "another high byte survives");
}

void test_sysex8_multi_packet() {
    // 30 bytes -> 13 + 13 + 4 across three packets.
    Vector<u8> body;
    for (u8 i = 0; i < 30; ++i) {
        body.push_back(static_cast<u8>(0xC0 + i));
    }
    Sysex8Message msg = Sysex8Message::from_bytes(1, 0x01, Span<const u8>{body.data(), body.size()});
    require(msg.packets().size() == 3, "30 bytes span three 13-byte SysEx8 packets");

    Vector<u8> back = msg.payload();
    require(back.size() == body.size(), "the full payload reassembles");
    for (usize i = 0; i < body.size(); ++i) {
        expect_eq(back[i], body[i], "every byte round-trips across the packet boundaries");
    }
}

// -- Flex Data ---------------------------------------------------------------

void test_flex_data_set_tempo() {
    // 500000 us/qn == 50,000,000 ten-ns units.
    const u32 tempo = 50'000'000u;
    FlexDataMessage msg = FlexDataMessage::set_tempo(3, tempo);
    expect(msg.type() == MessageType::FLEX_DATA, "set tempo is a Flex Data message");
    expect_eq(msg.packet().word_count(), u8{4}, "Flex Data is a 128-bit packet");
    expect_eq(msg.group(), u8{3}, "group round-trips");
    expect(msg.address() == FlexDataAddress::GROUP, "set tempo addresses the whole group");
    expect_eq(msg.status_bank(), FlexDataMessage::BANK_SETUP_AND_PERFORMANCE, "it is a setup/performance message");
    expect_eq(msg.status(), FlexDataMessage::STATUS_SET_TEMPO, "with the set-tempo status");
    expect_eq(msg.data1(), tempo, "the tempo value round-trips in word 1");
}

void test_flex_data_channel_addressed() {
    FlexDataMessage msg = FlexDataMessage::create(
        0, FlexDataAddress::CHANNEL, 7,
        FlexDataMessage::BANK_METADATA_TEXT, 0x01, 0xAABBCCDDu, 0x11223344u, 0x55667788u
    );
    expect(msg.address() == FlexDataAddress::CHANNEL, "a channel-addressed message decodes as CHANNEL");
    expect_eq(msg.channel(), u8{7}, "the channel round-trips");
    expect_eq(msg.form(), u8{0}, "a single message is form 0");
    expect_eq(msg.data1(), 0xAABBCCDDu, "word 1 round-trips");
    expect_eq(msg.data2(), 0x11223344u, "word 2 round-trips");
    expect_eq(msg.data3(), 0x55667788u, "word 3 round-trips");
}

void test_flex_data_rejects_bad_input() {
    expect_throws<InvalidMidiDataException>(
        [] -> void { static_cast<void>(FlexDataMessage::set_tempo(16, 0)); },
        "a group above 15 is rejected"
    );
}

// -- UMP Stream --------------------------------------------------------------

void test_stream_endpoint_discovery() {
    UmpStreamMessage msg = UmpStreamMessage::endpoint_discovery(1, 1, 0x1F);
    expect(msg.type() == MessageType::STREAM, "endpoint discovery is a Stream message");
    expect_eq(msg.packet().word_count(), u8{4}, "Stream messages are 128-bit");
    expect(msg.stream_status() == StreamStatus::ENDPOINT_DISCOVERY, "the status decodes to endpoint discovery");
    expect_eq(msg.format(), UmpStreamMessage::FORMAT_COMPLETE, "a lone message is COMPLETE");
    expect_eq(msg.payload16(), u16{0x0101}, "UMP version 1.1 packs into the word-0 payload");
    expect_eq(msg.data1(), u32{0x1F}, "the request filter bitmap lands in word 1");
}

void test_stream_function_block_status() {
    UmpStreamMessage msg = UmpStreamMessage::create(StreamStatus::FUNCTION_BLOCK_INFO_NOTIFICATION, 0x0042);
    expect(
        msg.stream_status() == StreamStatus::FUNCTION_BLOCK_INFO_NOTIFICATION,
        "a 10-bit status above 0xFF still round-trips"
    );
    expect_eq(msg.payload16(), u16{0x0042}, "the word-0 payload round-trips");
}

// -- Wire byte <-> word conversion (the backend's endianness boundary) -------

constexpr u8 BE_WORD_BYTES[4] = {0x11, 0x22, 0x33, 0x44};
static_assert(
    UniversalMidiPacket::word_from_be(BE_WORD_BYTES) == 0x11223344u,
    "four big-endian bytes assemble MSB-first into a word"
);

void test_packet_to_bytes_roundtrip() {
    // A 128-bit packet serialized to wire bytes and read back word-by-word must
    // reproduce the original words on any host - this is exactly the path the
    // ALSA UMP backend uses to move packets on and off the endpoint.
    UniversalMidiPacket p{0x40912C00u, 0xFFFF0000u, 0xDEADBEEFu, 0x0BADF00Du};
    Vector<u8> bytes;
    p.to_bytes(bytes);
    require(bytes.size() == 16, "a four-word packet serializes to sixteen bytes");

    for (u8 w = 0; w < 4; ++w) {
        const u32 reassembled = UniversalMidiPacket::word_from_be(bytes.data() + w * 4);
        expect_eq(reassembled, p.word(w), "each word survives the byte round trip");
    }
}

void test_packet_to_bytes_is_msb_first() {
    UniversalMidiPacket p{0x01020304u};
    Vector<u8> bytes;
    p.to_bytes(bytes);
    require(bytes.size() == 4, "a one-word packet is four bytes");
    expect_eq(bytes[0], u8{0x01}, "MSB first");
    expect_eq(bytes[3], u8{0x04}, "LSB last");
}

// -- Packet sizing: reserved Message Types -----------------------------------

static_assert(UniversalMidiPacket::words_for(MessageType::RESERVED_6) == 1, "0x6 is reserved 32-bit");
static_assert(UniversalMidiPacket::words_for(MessageType::RESERVED_8) == 2, "0x8 is reserved 64-bit");
static_assert(UniversalMidiPacket::words_for(MessageType::RESERVED_B) == 3, "0xB is reserved 96-bit");
static_assert(UniversalMidiPacket::words_for(MessageType::RESERVED_E) == 4, "0xE is reserved 128-bit");

void test_packet_from_words() {
    const u32 words[] = {0xB0000001u, 0x2u, 0x3u, 0x4u};
    Optional<UniversalMidiPacket> p = UniversalMidiPacket::from_words(Span<const u32>{words, 4});
    require(p.has_value(), "a reserved 96-bit packet builds from words");
    expect_eq(p->word_count(), u8{3}, "it takes three words, leaving the fourth");
    expect_eq(p->word(2), 0x3u, "the third word is the packet's last");

    expect(
        !UniversalMidiPacket::from_words(Span<const u32>{words, 2}).has_value(),
        "two words are too few for a 96-bit packet"
    );
    expect(!UniversalMidiPacket::from_words(Span<const u32>{}).has_value(), "no words, no packet");
}

// -- Translation: the remaining channel-voice messages -----------------------

void test_translate_note_on_velocity_zero_is_note_off() {
    ShortMessage off{MidiStatus::NOTE_ON, 2, 64, 0};
    Optional<Midi2ChannelVoiceMessage> up = MidiTranslator::to_midi2(off);
    require(up.has_value(), "a velocity-0 note on up-converts");
    expect(up->command() == Midi2Status::NOTE_OFF, "it becomes a MIDI 2.0 note off");
    expect_eq(up->index1(), u8{64}, "the note number is kept");
}

void test_translate_quiet_note_on_stays_on() {
    // 0x0100 narrows to 7-bit 0, which MIDI 1.0 would read as a note off.
    Midi2ChannelVoiceMessage quiet = Midi2ChannelVoiceMessage::note_on(0, 0, 60, 0x0100);
    Optional<ShortMessage> down = MidiTranslator::to_midi1(quiet);
    require(down.has_value(), "a quiet note on comes down");
    expect(down->command() == MidiStatus::NOTE_ON, "it stays a note on");
    expect_eq(down->data2(), u8{1}, "its velocity is clamped up to 1, not 0");
}

void test_translate_pressure_both_ways() {
    ShortMessage poly{MidiStatus::POLY_KEY_PRESSURE, 1, 60, 0x40};
    Optional<Midi2ChannelVoiceMessage> poly_up = MidiTranslator::to_midi2(poly);
    require(poly_up.has_value(), "poly pressure up-converts");
    expect(poly_up->command() == Midi2Status::POLY_PRESSURE, "it stays poly pressure");
    expect_eq(poly_up->index1(), u8{60}, "the note is kept");
    expect_eq(poly_up->value(), 0x80000000u, "the centred value widens to the 32-bit centre");
    Optional<ShortMessage> poly_down = MidiTranslator::to_midi1(*poly_up);
    require(poly_down.has_value(), "poly pressure comes back down");
    expect_eq(poly_down->data2(), u8{0x40}, "and round-trips exactly");

    ShortMessage channel{MidiStatus::CHANNEL_PRESSURE, 3, 0x7F};
    Optional<Midi2ChannelVoiceMessage> channel_up = MidiTranslator::to_midi2(channel);
    require(channel_up.has_value(), "channel pressure up-converts");
    expect(channel_up->command() == Midi2Status::CHANNEL_PRESSURE, "it stays channel pressure");
    expect_eq(channel_up->value(), 0xFFFFFFFFu, "the max widens to the 32-bit max");
    Optional<ShortMessage> channel_down = MidiTranslator::to_midi1(*channel_up);
    require(channel_down.has_value(), "channel pressure comes back down");
    expect_eq(channel_down->length(), 2uz, "as a two-byte message");
    expect_eq(channel_down->data1(), u8{0x7F}, "with its value");
}

void test_translate_program_change_both_ways() {
    ShortMessage pc{MidiStatus::PROGRAM_CHANGE, 9, 42};
    Optional<Midi2ChannelVoiceMessage> up = MidiTranslator::to_midi2(pc, 3);
    require(up.has_value(), "program change up-converts");
    expect(up->command() == Midi2Status::PROGRAM_CHANGE, "it stays a program change");
    expect_eq(up->program(), u8{42}, "the program is kept");
    expect(!up->bank_valid(), "a lone program change selects no bank");

    Optional<ShortMessage> down = MidiTranslator::to_midi1(*up);
    require(down.has_value(), "program change comes back down");
    expect_eq(down->data1(), u8{42}, "with its program");
    expect_eq(down->channel(), u8{9}, "on its channel");
}

void test_to_midi1_messages_expands_bank_select() {
    Midi2ChannelVoiceMessage pc = Midi2ChannelVoiceMessage::program_change(0, 3, 10, u8{1}, 2);
    expect(pc.bank_valid(), "a banked program change sets Bank Valid");
    Vector<ShortMessage> out = MidiTranslator::to_midi1_messages(pc);
    require(out.size() == 3, "a banked program change becomes three MIDI 1.0 messages");
    expect(out[0].command() == MidiStatus::CONTROL_CHANGE && out[0].data1() == 0 && out[0].data2() == 1,
        "first Bank Select MSB");
    expect(out[1].command() == MidiStatus::CONTROL_CHANGE && out[1].data1() == 32 && out[1].data2() == 2,
        "then Bank Select LSB");
    expect(out[2].command() == MidiStatus::PROGRAM_CHANGE && out[2].data1() == 10, "then the program");
    expect_eq(out[2].channel(), u8{3}, "all on the original channel");
}

void test_to_midi1_messages_expands_rpn() {
    // RPN 0/0 is pitch bend sensitivity: 12 semitones = Data Entry MSB 12, LSB 0.
    const u32 value = scale::up_14_to_32(12 << 7);
    Midi2ChannelVoiceMessage rpn = Midi2ChannelVoiceMessage::registered_controller(0, 0, 0, 0, value);
    Vector<ShortMessage> out = MidiTranslator::to_midi1_messages(rpn);
    require(out.size() == 4, "a registered controller becomes four CCs");
    expect(out[0].data1() == 101 && out[0].data2() == 0, "RPN MSB");
    expect(out[1].data1() == 100 && out[1].data2() == 0, "RPN LSB");
    expect(out[2].data1() == 6 && out[2].data2() == 12, "Data Entry MSB carries the semitones");
    expect(out[3].data1() == 38 && out[3].data2() == 0, "Data Entry LSB carries the cents");

    Midi2ChannelVoiceMessage nrpn = Midi2ChannelVoiceMessage::assignable_controller(0, 0, 5, 6, value);
    Vector<ShortMessage> nout = MidiTranslator::to_midi1_messages(nrpn);
    require(nout.size() == 4, "an assignable controller also becomes four CCs");
    expect(nout[0].data1() == 99 && nout[0].data2() == 5, "NRPN MSB");
    expect(nout[1].data1() == 98 && nout[1].data2() == 6, "NRPN LSB");
}

// -- Stateful MIDI 1.0 -> 2.0 translation ------------------------------------

void test_stateful_bank_select_folds_into_program_change() {
    Midi1ToMidi2Translator translator;
    expect(!translator.translate(ShortMessage{MidiStatus::CONTROL_CHANGE, 4, 0, 1}).has_value(),
        "Bank Select MSB is consumed");
    expect(!translator.translate(ShortMessage{MidiStatus::CONTROL_CHANGE, 4, 32, 2}).has_value(),
        "Bank Select LSB is consumed");

    Optional<Midi2ChannelVoiceMessage> pc = translator.translate(ShortMessage{MidiStatus::PROGRAM_CHANGE, 4, 7});
    require(pc.has_value(), "the program change comes out");
    expect(pc->bank_valid(), "carrying the selected bank");
    expect_eq(pc->bank_msb(), u8{1}, "bank MSB");
    expect_eq(pc->bank_lsb(), u8{2}, "bank LSB");
    expect_eq(pc->program(), u8{7}, "program");

    Optional<Midi2ChannelVoiceMessage> again = translator.translate(ShortMessage{MidiStatus::PROGRAM_CHANGE, 4, 8});
    require(again.has_value(), "a second program change comes out");
    expect(again->bank_valid() && again->bank_msb() == 1, "the bank persists, as in MIDI 1.0");

    Optional<Midi2ChannelVoiceMessage> other = translator.translate(ShortMessage{MidiStatus::PROGRAM_CHANGE, 5, 8});
    require(other.has_value(), "a program change on another channel comes out");
    expect(!other->bank_valid(), "with no bank: state is per channel");
}

void test_stateful_rpn_becomes_registered_controller() {
    Midi1ToMidi2Translator translator;
    expect(!translator.translate(ShortMessage{MidiStatus::CONTROL_CHANGE, 0, 101, 0}).has_value(), "RPN MSB consumed");
    expect(!translator.translate(ShortMessage{MidiStatus::CONTROL_CHANGE, 0, 100, 0}).has_value(), "RPN LSB consumed");

    Optional<Midi2ChannelVoiceMessage> msb = translator.translate(ShortMessage{MidiStatus::CONTROL_CHANGE, 0, 6, 12});
    require(msb.has_value(), "Data Entry MSB emits right away");
    expect(msb->command() == Midi2Status::REGISTERED_CONTROLLER, "as a registered controller");
    expect(msb->index1() == 0 && msb->index2() == 0, "addressing RPN 0/0");
    expect_eq(msb->value(), scale::up_14_to_32(12 << 7), "with the MSB and a zero LSB");

    Optional<Midi2ChannelVoiceMessage> lsb = translator.translate(ShortMessage{MidiStatus::CONTROL_CHANGE, 0, 38, 64});
    require(lsb.has_value(), "Data Entry LSB emits again");
    expect_eq(lsb->value(), scale::up_14_to_32((12 << 7) | 64), "with the full 14-bit value");

    // The null RPN ends the selection; Data Entry is then an ordinary CC.
    static_cast<void>(translator.translate(ShortMessage{MidiStatus::CONTROL_CHANGE, 0, 101, 127}));
    static_cast<void>(translator.translate(ShortMessage{MidiStatus::CONTROL_CHANGE, 0, 100, 127}));
    Optional<Midi2ChannelVoiceMessage> plain = translator.translate(ShortMessage{MidiStatus::CONTROL_CHANGE, 0, 6, 5});
    require(plain.has_value(), "after the null RPN, CC 6 still comes out");
    expect(plain->command() == Midi2Status::CONTROL_CHANGE, "as a plain control change");
}

// -- UmpEncoder --------------------------------------------------------------

void test_encoder_midi1_protocol() {
    UmpEncoder encoder{UmpProtocol::MIDI1, 2};
    Vector<UniversalMidiPacket> note = encoder.encode(ShortMessage{MidiStatus::NOTE_ON, 0, 0x3C, 0x7F});
    require(note.size() == 1, "a note on is one packet");
    expect_eq(note[0].word(0), 0x22903C7Fu, "a MIDI 1.0 channel-voice packet on group 2");

    Vector<UniversalMidiPacket> pc = encoder.encode(ShortMessage{MidiStatus::PROGRAM_CHANGE, 5, 5});
    require(pc.size() == 1, "a program change is one packet");
    expect_eq(pc[0].word(0), 0x22C50500u, "with its unused data byte zeroed");
}

void test_encoder_midi2_protocol() {
    UmpEncoder encoder{UmpProtocol::MIDI2};
    Vector<UniversalMidiPacket> note = encoder.encode(ShortMessage{MidiStatus::NOTE_ON, 1, 60, 0x7F});
    require(note.size() == 1, "a note on is one packet");
    expect(note[0].type() == MessageType::MIDI2_CHANNEL_VOICE, "a MIDI 2.0 channel-voice packet");
    expect_eq(note[0].word(1) >> 16, 0xFFFFu, "with 16-bit velocity");

    expect(encoder.encode(ShortMessage{MidiStatus::CONTROL_CHANGE, 1, 0, 3}).empty(),
        "Bank Select is held for the next program change");
    Vector<UniversalMidiPacket> pc = encoder.encode(ShortMessage{MidiStatus::PROGRAM_CHANGE, 1, 9});
    require(pc.size() == 1, "which then comes out");
    expect(Midi2ChannelVoiceMessage{pc[0]}.bank_valid(), "carrying the bank");
}

void test_encoder_system_sysex_meta_and_ump() {
    UmpEncoder encoder;

    Vector<u8> clock_bytes;
    clock_bytes.push_back(0xF8);
    Vector<UniversalMidiPacket> clock = encoder.encode(MidiMessage{Ops::move(clock_bytes)});
    require(clock.size() == 1, "timing clock is one packet");
    expect_eq(clock[0].word(0), 0x10F80000u, "a System packet");

    const u8 spp_raw[] = {0xF2, 0x10, 0x20};
    Vector<UniversalMidiPacket> spp = encoder.encode(SysexMessage{Span<const u8>{spp_raw, 3}});
    require(spp.size() == 1, "song position is one packet");
    expect_eq(spp[0].word(0), 0x10F21020u, "with both data bytes");

    const u8 sysex_raw[] = {0xF0, 0x7E, 0x01, 0x02, 0xF7};
    Vector<UniversalMidiPacket> sysex = encoder.encode(SysexMessage{Span<const u8>{sysex_raw, 5}});
    require(sysex.size() == 1, "a short SysEx is one SysEx7 packet");
    Vector<u8> body = Sysex7Message::from_packets(sysex).payload();
    require(body.size() == 3, "the F0/F7 wrappers are stripped");
    expect(body[0] == 0x7E && body[2] == 0x02, "the body is kept");

    const u8 tempo[] = {0x07, 0xA1, 0x20};
    expect(encoder.encode(MetaMessage{0x51, Span<const u8>{tempo, 3}}).empty(), "meta events never reach a wire");

    Vector<UniversalMidiPacket> passthrough = encoder.encode(UmpStreamMessage::endpoint_discovery());
    require(passthrough.size() == 1, "a UMP message passes through");
    expect(passthrough[0].type() == MessageType::STREAM, "as itself");
}

// -- UmpParser ---------------------------------------------------------------

namespace {
    /**
     * @brief What the parser handed back, captured outside the callback (the
     * message itself is a temporary).
     */
    struct Seen {
        String kind;
        Vector<u8> payload;
        u32 word0 = 0;
        u8 words = 0;
    };

    [[nodiscard]]
    Seen describe(const MidiMessage& msg) {
        Seen seen;
        if (const Sysex7Message* s7 = dynamic_cast<const Sysex7Message*>(&msg)) {
            seen.kind = "sysex7";
            seen.payload = s7->payload();
        } else if (const Sysex8Message* s8 = dynamic_cast<const Sysex8Message*>(&msg)) {
            seen.kind = "sysex8";
            seen.payload = s8->payload();
            seen.word0 = s8->stream_id();
        } else if (const UmpMessage* ump = dynamic_cast<const UmpMessage*>(&msg)) {
            seen.kind = dynamic_cast<const Midi2ChannelVoiceMessage*>(&msg) ? "midi2"
                : dynamic_cast<const SystemMessage*>(&msg) ? "system"
                : dynamic_cast<const UmpStreamMessage*>(&msg) ? "stream"
                : "ump";
            seen.word0 = ump->packet().word(0);
            seen.words = ump->packet().word_count();
        }
        return seen;
    }

    void append_words(Vector<u32>& out, const Vector<UniversalMidiPacket>& packets) {
        for (const UniversalMidiPacket& p: packets) {
            for (u8 i = 0; i < p.word_count(); ++i) {
                out.push_back(p.word(i));
            }
        }
    }
}

void test_parser_decodes_across_split_feeds() {
    UmpParser parser;
    Vector<Seen> seen;
    const auto sink = [&seen](const MidiMessage& msg) -> void { seen.push_back(describe(msg)); };

    const UniversalMidiPacket note = Midi2ChannelVoiceMessage::note_on(0, 0, 60, 0x8000).packet();
    const u32 first[] = {note.word(0)};
    const u32 rest[] = {note.word(1), 0x10F80000u};

    parser.feed(Span<const u32>{first, 1}, sink);
    expect(seen.empty(), "half a 64-bit packet produces nothing yet");
    expect(parser.has_partial_packet(), "and is held");

    parser.feed(Span<const u32>{rest, 2}, sink);
    require(seen.size() == 2, "the rest completes it, and the next packet follows");
    expect_eq(seen[0].kind, "midi2"s, "the first decodes as MIDI 2.0 channel voice");
    expect_eq(seen[0].word0, note.word(0), "intact");
    expect_eq(seen[1].kind, "system"s, "the second as a System message");
    expect(!parser.has_partial_packet(), "nothing is left over");
}

void test_parser_reassembles_sysex7() {
    Vector<u8> body;
    for (u8 i = 0; i < 20; ++i) {
        body.push_back(i);
    }
    Vector<u32> words;
    append_words(words, Sysex7Message::from_bytes(3, Span<const u8>{body.data(), body.size()}).packets());

    UmpParser parser;
    Vector<Seen> seen;
    parser.feed(Span<const u32>{words.data(), words.size()}, [&seen](const MidiMessage& msg) -> void {
        seen.push_back(describe(msg));
    });
    require(seen.size() == 1, "a four-packet SysEx7 arrives as one message");
    expect_eq(seen[0].kind, "sysex7"s, "a Sysex7Message");
    require(seen[0].payload.size() == 20, "with the whole body");
    expect_eq(seen[0].payload[19], u8{19}, "in order");
}

void test_parser_reassembles_interleaved_sysex8_streams() {
    Vector<u8> a(30, u8{0xAA});
    Vector<u8> b(20, u8{0xBB});
    const Vector<UniversalMidiPacket> pa = Sysex8Message::from_bytes(0, 1, Span<const u8>{a.data(), a.size()}).packets();
    const Vector<UniversalMidiPacket> pb = Sysex8Message::from_bytes(0, 2, Span<const u8>{b.data(), b.size()}).packets();
    require(pa.size() == 3 && pb.size() == 2, "the two runs span three and two packets");

    // a0 b0 a1 b1 a2: the runs overlap on the same group.
    Vector<UniversalMidiPacket> mixed;
    mixed.push_back(pa[0]);
    mixed.push_back(pb[0]);
    mixed.push_back(pa[1]);
    mixed.push_back(pb[1]);
    mixed.push_back(pa[2]);
    Vector<u32> words;
    append_words(words, mixed);

    UmpParser parser;
    Vector<Seen> seen;
    parser.feed(Span<const u32>{words.data(), words.size()}, [&seen](const MidiMessage& msg) -> void {
        seen.push_back(describe(msg));
    });
    require(seen.size() == 2, "both interleaved runs reassemble");
    expect(seen[0].kind == "sysex8" && seen[0].word0 == 2, "stream 2 finishes first");
    expect_eq(seen[0].payload.size(), 20uz, "with its whole body");
    expect(seen[1].kind == "sysex8" && seen[1].word0 == 1, "then stream 1");
    expect_eq(seen[1].payload.size(), 30uz, "with its whole body");
}

void test_parser_skips_reserved_packets_whole() {
    const u32 words[] = {0xB0000000u, 0x11111111u, 0x22222222u, 0x10F80000u};
    UmpParser parser;
    Vector<Seen> seen;
    parser.feed(Span<const u32>{words, 4}, [&seen](const MidiMessage& msg) -> void {
        seen.push_back(describe(msg));
    });
    require(seen.size() == 2, "a reserved 96-bit packet does not swallow the next one");
    expect(seen[0].kind == "ump" && seen[0].words == 3, "it comes out as a plain three-word UmpMessage");
    expect_eq(seen[1].kind, "system"s, "and the timing clock after it decodes normally");
}

void test_parser_drops_orphan_sysex_end() {
    Vector<u8> body(14, u8{1});
    const Vector<UniversalMidiPacket> run = Sysex7Message::from_bytes(0, Span<const u8>{body.data(), body.size()}).packets();
    Vector<u32> words;
    for (usize i = 1; i < run.size(); ++i) { // skip START
        words.push_back(run[i].word(0));
        words.push_back(run[i].word(1));
    }
    UmpParser parser;
    usize count = 0;
    parser.feed(Span<const u32>{words.data(), words.size()}, [&count](const MidiMessage&) -> void { ++count; });
    expect_eq(count, 0uz, "CONTINUE and END with no START are dropped");
}

// -- UMP -> MIDI 1.0 bytes ---------------------------------------------------

void test_to_midi1_bytes() {
    expect(MidiTranslator::is_ump(UtilityMessage::noop()), "a UtilityMessage is UMP");
    expect(!MidiTranslator::is_ump(ShortMessage{MidiStatus::NOTE_ON, 0, 60, 1}), "a ShortMessage is not");

    Vector<Vector<u8>> m1 = MidiTranslator::to_midi1_bytes(
        Midi1ChannelVoiceMessage{0, MidiStatus::NOTE_ON, 3, 60, 100}
    );
    require(m1.size() == 1 && m1[0].size() == 3, "a MIDI 1.0 channel-voice packet becomes three bytes");
    expect(m1[0][0] == 0x93 && m1[0][1] == 60 && m1[0][2] == 100, "status, note, velocity");

    Vector<Vector<u8>> clock = MidiTranslator::to_midi1_bytes(SystemMessage{0, MidiStatus::TIMING_CLOCK});
    require(clock.size() == 1 && clock[0].size() == 1, "timing clock is one byte");

    Vector<Vector<u8>> pc = MidiTranslator::to_midi1_bytes(
        Midi2ChannelVoiceMessage::program_change(0, 0, 1, u8{0}, 5)
    );
    expect_eq(pc.size(), 3uz, "a banked program change becomes three messages");

    expect(MidiTranslator::to_midi1_bytes(FlexDataMessage::set_tempo(0, 50'000'000)).empty(),
        "flex data has no MIDI 1.0 wire form");

    const u8 body[] = {0x7E, 0x7F, 0x06, 0x01};
    Vector<Vector<u8>> sysex = MidiTranslator::to_midi1_bytes(
        Sysex7Message::from_bytes(0, Span<const u8>{body, 4})
    );
    require(sysex.size() == 1 && sysex[0].size() == 6, "SysEx7 becomes one F0 ... F7 message");
    expect(sysex[0].front() == 0xF0 && sysex[0].back() == 0xF7, "with its wrappers restored");
}

// -- Delta Clockstamps, clip markers, Mixed Data Set -------------------------

void test_delta_clockstamps() {
    UtilityMessage tpq = UtilityMessage::delta_clockstamp_tpq(960);
    expect_eq(tpq.packet().word(0), 0x003003C0u, "DCTPQ: status 3, 16-bit ticks per quarter");
    expect(tpq.utility_status() == UtilityStatus::DELTA_CLOCKSTAMP_TPQ, "decodes its status");
    expect_eq(tpq.value(), u16{960}, "and its resolution");

    UtilityMessage dc = UtilityMessage::delta_clockstamp(0xABCDE);
    expect_eq(dc.packet().word(0), 0x004ABCDEu, "DCS: status 4, 20-bit tick count");
    expect_eq(dc.delta_ticks(), 0xABCDEu, "decodes all 20 bits");
    expect_eq(tpq.delta_ticks(), 0u, "other utility statuses report no delta");

    expect_throws<InvalidMidiDataException>(
        [] -> void { static_cast<void>(UtilityMessage::delta_clockstamp(0x100000)); },
        "a delta past 20 bits is rejected"
    );
    expect_throws<InvalidMidiDataException>(
        [] -> void { static_cast<void>(UtilityMessage::delta_clockstamp_tpq(0)); },
        "zero ticks per quarter is rejected"
    );
}

void test_clip_markers_and_flex_tempo() {
    expect(UmpStreamMessage::start_of_clip().stream_status() == StreamStatus::START_OF_CLIP, "Start of Clip");
    expect_eq(UmpStreamMessage::end_of_clip().packet().word(0), 0xF0210000u, "End of Clip is status 0x21");

    FlexDataMessage tempo = FlexDataMessage::set_tempo(0, 50'000'000); // 0.5 s per quarter
    expect(tempo.is_set_tempo(), "Set Tempo recognises itself");
    expect(tempo.tempo_us_per_quarter() == Optional<u32>{500'000}, "10 ns units convert to microseconds");
    expect(!FlexDataMessage::create(0, FlexDataAddress::GROUP, 0, 0, FlexDataMessage::STATUS_SET_TIME_SIGNATURE)
        .tempo_us_per_quarter().has_value(), "other flex messages carry no tempo");
}

void test_mixed_data_set_packets() {
    MixedDataSetHeader header = MixedDataSetHeader::create(3, 7, 400, 2, 1, 0x0021, 0x0004, 0x0011, 0x0022);
    expect_eq(header.packet().word(0), 0x53870190u, "header: type 5, group 3, status 8, MDS id 7, 400 bytes");
    expect(header.chunk_count() == 2 && header.chunk_number() == 1, "chunk count and number");
    expect(header.manufacturer_id() == 0x0021 && header.device_id() == 0x0004, "manufacturer and device");
    expect(header.sub_id1() == 0x0011 && header.sub_id2() == 0x0022, "sub ids");

    Vector<u8> data;
    for (u8 i = 1; i <= 14; ++i) {
        data.push_back(i);
    }
    MixedDataSetPayload payload = MixedDataSetPayload::create(3, 7, Span<const u8>{data.data(), data.size()});
    expect_eq(payload.packet().word(0), 0x53970102u, "payload: status 9, first two bytes in word 0");
    expect_eq(payload.mds_id(), u8{7}, "MDS id");
    const Array<u8, 14> back = payload.bytes();
    expect(back[0] == 1 && back[13] == 14, "all fourteen bytes come back in order");

    Vector<u32> words;
    for (const UniversalMidiPacket& p: {header.packet(), payload.packet()}) {
        for (u8 i = 0; i < p.word_count(); ++i) {
            words.push_back(p.word(i));
        }
    }
    UmpParser parser;
    Vector<String> kinds;
    parser.feed(Span<const u32>{words.data(), words.size()}, [&kinds](const MidiMessage& msg) -> void {
        kinds.push_back(dynamic_cast<const MixedDataSetHeader*>(&msg) ? "header"
            : dynamic_cast<const MixedDataSetPayload*>(&msg) ? "payload" : "other");
    });
    require(kinds.size() == 2, "the parser yields both packets");
    expect(kinds[0] == "header" && kinds[1] == "payload", "as their own classes");
}

void test_parser_feed_owned_keeps_class() {
    const UniversalMidiPacket note = Midi2ChannelVoiceMessage::note_on(0, 0, 60, 1).packet();
    const u32 words[] = {note.word(0), note.word(1), 0x10F80000u};
    Vector<UniquePointer<MidiMessage>> kept;
    UmpParser parser;
    parser.feed_owned(Span<const u32>{words, 3}, [&kept](UniquePointer<MidiMessage> msg) -> void {
        kept.push_back(Ops::move(msg));
    });
    require(kept.size() == 2, "both messages are handed over");
    expect(dynamic_cast<const Midi2ChannelVoiceMessage*>(kept[0].get()) != nullptr, "the first keeps its class");
    expect(dynamic_cast<const SystemMessage*>(kept[1].get()) != nullptr, "so does the second");
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Ump.packet_word_access", test_packet_word_access},
        {"Ump.packet_equality", test_packet_equality},
        {"Ump.bytes_are_big_endian", test_ump_bytes_are_big_endian},
        {"Ump.utility_noop", test_utility_noop},
        {"Ump.utility_jr_timestamp_roundtrips", test_utility_jr_timestamp_roundtrips},
        {"Ump.system_message_roundtrips", test_system_message_roundtrips},
        {"Ump.system_message_rejects_bad_input", test_system_message_rejects_bad_input},
        {"Ump.midi2_note_on", test_midi2_note_on},
        {"Ump.midi2_control_change", test_midi2_control_change},
        {"Ump.midi2_rejects_bad_input", test_midi2_rejects_bad_input},
        {"Ump.midi1_in_ump", test_midi1_in_ump},
        {"Ump.translate_note_on_to_midi2", test_translate_note_on_to_midi2},
        {"Ump.translate_roundtrip_preserves_center", test_translate_note_roundtrip_preserves_center},
        {"Ump.translate_pitch_bend_roundtrip", test_translate_pitch_bend_roundtrip},
        {"Ump.translate_rejects_non_channel_voice", test_translate_rejects_non_channel_voice},
        {"Ump.sysex7_single_packet", test_sysex7_single_packet},
        {"Ump.sysex7_multi_packet", test_sysex7_multi_packet},
        {"Ump.sysex7_rejects_non_7bit", test_sysex7_rejects_non_7bit},
        {"Ump.sysex8_single_packet", test_sysex8_single_packet},
        {"Ump.sysex8_multi_packet", test_sysex8_multi_packet},
        {"Ump.flex_data_set_tempo", test_flex_data_set_tempo},
        {"Ump.flex_data_channel_addressed", test_flex_data_channel_addressed},
        {"Ump.flex_data_rejects_bad_input", test_flex_data_rejects_bad_input},
        {"Ump.stream_endpoint_discovery", test_stream_endpoint_discovery},
        {"Ump.stream_function_block_status", test_stream_function_block_status},
        {"Ump.packet_to_bytes_roundtrip", test_packet_to_bytes_roundtrip},
        {"Ump.packet_to_bytes_is_msb_first", test_packet_to_bytes_is_msb_first},
        {"Ump.packet_from_words", test_packet_from_words},
        {"Ump.translate_note_on_velocity_zero_is_note_off", test_translate_note_on_velocity_zero_is_note_off},
        {"Ump.translate_quiet_note_on_stays_on", test_translate_quiet_note_on_stays_on},
        {"Ump.translate_pressure_both_ways", test_translate_pressure_both_ways},
        {"Ump.translate_program_change_both_ways", test_translate_program_change_both_ways},
        {"Ump.to_midi1_messages_expands_bank_select", test_to_midi1_messages_expands_bank_select},
        {"Ump.to_midi1_messages_expands_rpn", test_to_midi1_messages_expands_rpn},
        {"Ump.stateful_bank_select_folds_into_program_change", test_stateful_bank_select_folds_into_program_change},
        {"Ump.stateful_rpn_becomes_registered_controller", test_stateful_rpn_becomes_registered_controller},
        {"Ump.encoder_midi1_protocol", test_encoder_midi1_protocol},
        {"Ump.encoder_midi2_protocol", test_encoder_midi2_protocol},
        {"Ump.encoder_system_sysex_meta_and_ump", test_encoder_system_sysex_meta_and_ump},
        {"Ump.parser_decodes_across_split_feeds", test_parser_decodes_across_split_feeds},
        {"Ump.parser_reassembles_sysex7", test_parser_reassembles_sysex7},
        {"Ump.parser_reassembles_interleaved_sysex8_streams", test_parser_reassembles_interleaved_sysex8_streams},
        {"Ump.parser_skips_reserved_packets_whole", test_parser_skips_reserved_packets_whole},
        {"Ump.parser_drops_orphan_sysex_end", test_parser_drops_orphan_sysex_end},
        {"Ump.to_midi1_bytes", test_to_midi1_bytes},
        {"Ump.delta_clockstamps", test_delta_clockstamps},
        {"Ump.clip_markers_and_flex_tempo", test_clip_markers_and_flex_tempo},
        {"Ump.mixed_data_set_packets", test_mixed_data_set_packets},
        {"Ump.parser_feed_owned_keeps_class", test_parser_feed_owned_keeps_class},
    });
}
