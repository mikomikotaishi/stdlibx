import stdx;

using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::io::OpenMode;
using stdx::io::OutputFileStream;
using stdx::io::StreamSize;
using stdx::mem::UniquePointer;

using namespace stdx::test;
using namespace stdx::audio::midi;

// Append a big-endian 16-bit value.
static void put_u16(Vector<u8>& out, u16 v) {
    out.push_back(static_cast<u8>((v >> 8) & 0xFF));
    out.push_back(static_cast<u8>(v & 0xFF));
}

// Append a big-endian 32-bit value.
static void put_u32(Vector<u8>& out, u32 v) {
    out.push_back(static_cast<u8>((v >> 24) & 0xFF));
    out.push_back(static_cast<u8>((v >> 16) & 0xFF));
    out.push_back(static_cast<u8>((v >> 8) & 0xFF));
    out.push_back(static_cast<u8>(v & 0xFF));
}

static void put_tag(Vector<u8>& out, const char* tag) {
    for (usize i = 0; i < 4; ++i) {
        out.push_back(static_cast<u8>(tag[i]));
    }
}

// Write bytes to a temp file and parse them back as a Sequence.
static UniquePointer<Sequence> parse_bytes(const Vector<u8>& bytes, const Path& path) {
    stdx::fs::remove_all(path.parent_path());
    stdx::fs::create_directories(path.parent_path());
    {
        OutputFileStream out(path, OpenMode::OUT | OpenMode::BINARY);
        out.write(reinterpret_cast<const char*>(bytes.data()), static_cast<StreamSize>(bytes.size()));
    }
    return MidiSystem::open_sequence(path);
}

static Path temp_mid(const char* name) {
    return stdx::fs::temp_directory_path() / "stdx_smf_test" / name;
}

// A minimal format-0 file: PPQ 480, one track with note on @0, note off @480,
// and an end-of-track meta. Running status, deltas, and the meta VLQ all get
// exercised in one parse.
static Vector<u8> minimal_format0() {
    Vector<u8> track;
    // delta 0, Note On ch0 note 60 vel 100
    track.push_back(0x00);
    track.push_back(0x90);
    track.push_back(60);
    track.push_back(100);
    // delta 480 (VLQ 0x83 0x60), running status: note 60 vel 0 (note off form)
    track.push_back(0x83);
    track.push_back(0x60);
    track.push_back(60);
    track.push_back(0);
    // delta 0, end-of-track meta (FF 2F 00)
    track.push_back(0x00);
    track.push_back(0xFF);
    track.push_back(0x2F);
    track.push_back(0x00);

    Vector<u8> file;
    put_tag(file, "MThd");
    put_u32(file, 6);       // header length
    put_u16(file, 0);       // format 0
    put_u16(file, 1);       // one track
    put_u16(file, 480);     // PPQ division
    put_tag(file, "MTrk");
    put_u32(file, static_cast<u32>(track.size()));
    for (u8 b: track) {
        file.push_back(b);
    }
    return file;
}

void test_parses_header_and_division() {
    UniquePointer<Sequence> seq = parse_bytes(minimal_format0(), temp_mid("basic.mid"));
    require(seq != nullptr, "a well-formed file parses to a sequence");
    expect(seq->type() == TimingType::PPQ, "a positive division is PPQ timing");
    expect_eq(seq->division(), 480, "the division round-trips");
    expect_eq(seq->track_count(), 1uz, "format 0 yields one track");
}

void test_parses_events_and_running_status() {
    UniquePointer<Sequence> seq = parse_bytes(minimal_format0(), temp_mid("events.mid"));
    const Track& track = seq->track(0);
    // note on, note off (via running status), end-of-track = 3 events.
    expect_eq(track.size(), 3uz, "all three events are decoded");

    const MidiEvent& on = track[0];
    expect_eq(on.tick, i64{0}, "the note on sits at tick 0");
    expect_eq(on.message->status(), u8{0x90}, "it is a note on on channel 0");

    const MidiEvent& off = track[1];
    expect_eq(off.tick, i64{480}, "the delta of 480 accumulates to an absolute tick");
    // Running status reuses 0x90; a zero velocity is the note-off idiom.
    expect_eq(off.message->status(), u8{0x90}, "running status carries the note-on status forward");
}

void test_smpte_division() {
    // Division 0xE728: high byte 0xE7 = -25 (format 25), low byte 0x28 = 40 ticks/frame.
    Vector<u8> file;
    put_tag(file, "MThd");
    put_u32(file, 6);
    put_u16(file, 0);
    put_u16(file, 0);           // zero tracks is legal
    put_u16(file, 0xE728);
    UniquePointer<Sequence> seq = parse_bytes(file, temp_mid("smpte.mid"));
    expect(seq->type() == TimingType::SMPTE, "a negative division is SMPTE timing");
    expect_eq(seq->smpte_format(), 25, "the frame format decodes from the high byte");
    expect_eq(seq->smpte_ticks_per_frame(), u8{40}, "ticks-per-frame decodes from the low byte");
}

void test_rejects_bad_header() {
    Vector<u8> file;
    put_tag(file, "MWTF");      // wrong magic
    put_u32(file, 6);
    put_u16(file, 0);
    put_u16(file, 0);
    put_u16(file, 480);
    expect_throws<InvalidMidiDataException>(
        [&] -> void { static_cast<void>(parse_bytes(file, temp_mid("badmagic.mid"))); },
        "a file without the MThd magic is rejected"
    );
}

void test_rejects_unknown_format() {
    Vector<u8> file;
    put_tag(file, "MThd");
    put_u32(file, 6);
    put_u16(file, 3);           // only 0/1/2 exist
    put_u16(file, 0);
    put_u16(file, 480);
    expect_throws<InvalidMidiDataException>(
        [&] -> void { static_cast<void>(parse_bytes(file, temp_mid("badformat.mid"))); },
        "an unknown format number is rejected"
    );
}

int main(int argc, char* argv[]) {
    const int rc = run(argc, argv, {
        {"Smf.parses_header_and_division", test_parses_header_and_division},
        {"Smf.parses_events_and_running_status", test_parses_events_and_running_status},
        {"Smf.smpte_division", test_smpte_division},
        {"Smf.rejects_bad_header", test_rejects_bad_header},
        {"Smf.rejects_unknown_format", test_rejects_unknown_format},
    });
    stdx::fs::remove_all(stdx::fs::temp_directory_path() / "stdx_smf_test");
    return rc;
}
