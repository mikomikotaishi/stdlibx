import stdx;

using stdx::collections::Vector;

using namespace stdx::test;
using namespace stdx::audio::midi;

// -- ShortMessage: channel-voice construction and accessors ------------------

void test_short_message_note_on() {
    ShortMessage note{MidiStatus::NOTE_ON, 3, 60, 100};
    expect_eq(note.length(), 3uz, "a note on is three bytes");
    expect_eq(note.status(), u8{0x93}, "status byte merges command and channel");
    expect(note.command() == MidiStatus::NOTE_ON, "command strips the channel nibble");
    expect_eq(note.channel(), u8{3}, "channel is the low nibble");
    expect_eq(note.data1(), u8{60}, "data1 is the note number");
    expect_eq(note.data2(), u8{100}, "data2 is the velocity");
}

void test_short_message_two_byte_forms() {
    // Program Change and Channel Pressure carry only one data byte.
    ShortMessage pc{MidiStatus::PROGRAM_CHANGE, 0, 42};
    expect_eq(pc.length(), 2uz, "program change is two bytes");
    expect_eq(pc.data1(), u8{42}, "the program number is data1");

    ShortMessage cp{MidiStatus::CHANNEL_PRESSURE, 5, 77};
    expect_eq(cp.length(), 2uz, "channel pressure is two bytes");
    expect_eq(cp.channel(), u8{5}, "channel still decodes");
    expect_eq(cp.data1(), u8{77}, "the pressure value is data1");
}

void test_short_message_rejects_bad_input() {
    expect_throws<InvalidMidiDataException>(
        [] -> void { static_cast<void>(ShortMessage(MidiStatus::NOTE_ON, 16, 0, 0)); },
        "a channel above 15 is rejected"
    );
    expect_throws<InvalidMidiDataException>(
        [] -> void { static_cast<void>(ShortMessage(MidiStatus::NOTE_ON, 0, 128, 0)); },
        "a data1 above 127 is rejected"
    );
    expect_throws<InvalidMidiDataException>(
        [] -> void { static_cast<void>(ShortMessage(MidiStatus::NOTE_ON, 0, 0, 128)); },
        "a data2 above 127 is rejected"
    );
}

void test_short_message_set_message_reuses_storage() {
    ShortMessage msg{MidiStatus::NOTE_ON, 0, 60, 100};
    msg.set_message(MidiStatus::CONTROL_CHANGE, 1, 7, 127);
    expect(msg.command() == MidiStatus::CONTROL_CHANGE, "set_message swaps the command");
    expect_eq(msg.channel(), u8{1}, "and the channel");
    expect_eq(msg.data1(), u8{7}, "and data1");
    expect_eq(msg.data2(), u8{127}, "and data2");
    expect_eq(msg.length(), 3uz, "a control change stays three bytes after reuse");
}

// -- MetaMessage: variable-length payload encoding ---------------------------

void test_meta_message_short_payload() {
    const u8 tempo[] = {0x07, 0xA1, 0x20}; // 500000 us/qn
    MetaMessage meta{0x51, Span<const u8>{tempo, 3}};
    expect_eq(meta.status(), u8{0xFF}, "a meta message has an 0xFF status");
    expect_eq(meta.type(), u8{0x51}, "the set-tempo type round-trips");
    // status + type + one-byte length + three payload bytes = 6.
    expect_eq(meta.length(), 6uz, "a short payload uses a single length byte");
    const Span<const u8> bytes = meta.data();
    expect_eq(bytes[2], u8{0x03}, "the length byte is the payload size");
    expect_eq(bytes[3], u8{0x07}, "payload follows the length");
}

void test_meta_message_multibyte_length() {
    // A 200-byte payload needs a two-byte VLQ length (0x81 0x48).
    Vector<u8> payload(200, u8{0xAB});
    MetaMessage meta{0x7F, Span<const u8>{payload.data(), payload.size()}};
    const Span<const u8> bytes = meta.data();
    expect_eq(bytes[0], u8{0xFF}, "status");
    expect_eq(bytes[1], u8{0x7F}, "sequencer-specific type");
    expect_eq(bytes[2], u8{0x81}, "first VLQ byte has the continuation bit set");
    expect_eq(bytes[3], u8{0x48}, "second VLQ byte holds the low 7 bits of 200");
    // status + type + two length bytes + 200 payload = 204.
    expect_eq(meta.length(), 204uz, "the full message length accounts for the VLQ and payload");
}

void test_meta_message_rejects_bad_type() {
    const u8 none[] = {0};
    expect_throws<InvalidMidiDataException>(
        [&] -> void { static_cast<void>(MetaMessage(0x80, Span<const u8>{none, 0})); },
        "a meta type above 0x7F is rejected"
    );
}

// -- SysexMessage ------------------------------------------------------------

void test_sysex_message_roundtrips() {
    const u8 raw[] = {0xF0, 0x7E, 0x00, 0x09, 0x01, 0xF7};
    SysexMessage sysex{Span<const u8>{raw, sizeof(raw)}};
    expect_eq(sysex.length(), 6uz, "the raw bytes are kept verbatim");
    expect_eq(sysex.status(), u8{0xF0}, "the leading status is 0xF0");
    const Span<const u8> bytes = sysex.data();
    expect_eq(bytes[bytes.size() - 1], u8{0xF7}, "and the trailing byte is the EOX");
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"MidiMessage.short_note_on", test_short_message_note_on},
        {"MidiMessage.short_two_byte_forms", test_short_message_two_byte_forms},
        {"MidiMessage.short_rejects_bad_input", test_short_message_rejects_bad_input},
        {"MidiMessage.short_set_message_reuses_storage", test_short_message_set_message_reuses_storage},
        {"MidiMessage.meta_short_payload", test_meta_message_short_payload},
        {"MidiMessage.meta_multibyte_length", test_meta_message_multibyte_length},
        {"MidiMessage.meta_rejects_bad_type", test_meta_message_rejects_bad_type},
        {"MidiMessage.sysex_roundtrips", test_sysex_message_roundtrips},
    });
}
