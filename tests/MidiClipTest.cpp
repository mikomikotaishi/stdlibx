import stdx;

using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::mem::Pointers;
using stdx::mem::UniquePointer;
using stdx::sync::LockGuard;
using stdx::sync::Mutex;
using stdx::thread::Thread;

using namespace stdx::test;
using namespace stdx::audio::midi;

namespace {
    [[nodiscard]]
    u32 word_at(const Vector<u8>& bytes, usize index) {
        return UniversalMidiPacket::word_from_be(bytes.data() + 8 + index * 4);
    }

    void put_word(Vector<u8>& out, u32 word) {
        out.push_back(static_cast<u8>(word >> 24));
        out.push_back(static_cast<u8>(word >> 16));
        out.push_back(static_cast<u8>(word >> 8));
        out.push_back(static_cast<u8>(word));
    }

    void put_packet(Vector<u8>& out, const UniversalMidiPacket& p) {
        for (u8 i = 0; i < p.word_count(); ++i) {
            put_word(out, p.word(i));
        }
    }

    [[nodiscard]]
    Vector<u8> header() {
        const char magic[] = "SMF2CLIP";
        return Vector<u8>(magic, magic + 8);
    }

    template <typename T>
    [[nodiscard]]
    const T* as(const MidiEvent& event) {
        return dynamic_cast<const T*>(event.message.get());
    }

    void add(Track& track, i64 tick, UniquePointer<MidiMessage> msg) {
        track.add(MidiEvent {
            .tick = tick,
            .message = Ops::move(msg),
        });
    }

    [[nodiscard]]
    UniquePointer<MidiMessage> tempo_meta(u32 us_per_quarter) {
        const u8 payload[] = {
            static_cast<u8>(us_per_quarter >> 16), static_cast<u8>(us_per_quarter >> 8), static_cast<u8>(us_per_quarter)
        };
        return Pointers::unique<MetaMessage>(0x51, Span<const u8>{payload, 3});
    }

    [[nodiscard]]
    UniquePointer<MidiMessage> end_of_track() {
        return Pointers::unique<MetaMessage>(0x2F, Span<const u8>{});
    }
}

void test_write_header_layout() {
    Sequence seq{TimingType::PPQ, 96};
    Track& track = seq.create_track();
    add(track, 0, Pointers::unique<ShortMessage>(MidiStatus::NOTE_ON, 0, 60, 100));

    const Vector<u8> bytes = MidiClipFile::write(seq);
    expect(MidiClipFile::is_clip(Span<const u8>{bytes.data(), bytes.size()}), "starts with SMF2CLIP");
    expect_eq(word_at(bytes, 0), 0x00400000u, "first a Delta Clockstamp of 0");
    expect_eq(word_at(bytes, 1), 0x00300060u, "then DCTPQ with the sequence's 96 ticks per quarter");
    expect_eq(word_at(bytes, 2), 0x00400000u, "a Delta Clockstamp of 0 before Start of Clip");
    expect_eq(word_at(bytes, 3) >> 16, 0xF020u, "Start of Clip");
    expect_eq(word_at(bytes, 7), 0x20903C64u, "the note, sharing Start of Clip's clockstamp, as MIDI 1.0 UMP");
    const usize words = (bytes.size() - 8) / 4;
    expect_eq(word_at(bytes, words - 4) >> 16, 0xF021u, "End of Clip is the last message");
}

void test_round_trip() {
    Sequence seq{TimingType::PPQ, 96};
    Track& track = seq.create_track();
    add(track, 0, tempo_meta(500'000));
    add(track, 0, Pointers::unique<ShortMessage>(MidiStatus::NOTE_ON, 1, 60, 100));
    add(track, 96, Pointers::unique<ShortMessage>(MidiStatus::NOTE_OFF, 1, 60, 0));
    add(track, 96, Pointers::unique<Midi2ChannelVoiceMessage>(Midi2ChannelVoiceMessage::note_on(0, 2, 64, 0x8000)));
    Vector<u8> sysex = {0xF0};
    for (u8 i = 0; i < 20; ++i) {
        sysex.push_back(i);
    }
    sysex.push_back(0xF7);
    add(track, 192, Pointers::unique<SysexMessage>(Span<const u8>{sysex.data(), sysex.size()}));
    add(track, 384, end_of_track());

    const Vector<u8> bytes = MidiClipFile::write(seq);
    UniquePointer<Sequence> back = MidiClipFile::parse(Span<const u8>{bytes.data(), bytes.size()});
    require(back != nullptr, "the clip parses back");
    expect(back->type() == TimingType::PPQ, "PPQ timing");
    expect_eq(back->division(), 96, "division from DCTPQ");
    require(back->track_count() == 1, "a clip is one track");
    const Track& t = back->track(0);
    require(t.size() == 6, "every event survives");

    const FlexDataMessage* tempo = as<FlexDataMessage>(t[0]);
    require(tempo != nullptr && t[0].tick == 0, "tempo meta became Flex Set Tempo at tick 0");
    expect(tempo->tempo_us_per_quarter() == Optional<u32>{500'000}, "with the same tempo");

    const Midi1ChannelVoiceMessage* on = as<Midi1ChannelVoiceMessage>(t[1]);
    require(on != nullptr && t[1].tick == 0, "the MIDI 1.0 note on is a MIDI 1.0 UMP at tick 0");
    expect(on->channel() == 1 && on->data1() == 60 && on->data2() == 100, "bit-exact");

    expect(as<Midi1ChannelVoiceMessage>(t[2]) != nullptr && t[2].tick == 96, "note off at tick 96");
    const Midi2ChannelVoiceMessage* m2 = as<Midi2ChannelVoiceMessage>(t[3]);
    require(m2 != nullptr && t[3].tick == 96, "the MIDI 2.0 note keeps its class and tick");
    expect_eq(m2->velocity(), u16{0x8000}, "and its 16-bit velocity");

    const Sysex7Message* s7 = as<Sysex7Message>(t[4]);
    require(s7 != nullptr && t[4].tick == 192, "the SysEx came back as one reassembled SysEx7");
    expect_eq(s7->payload().size(), 20uz, "with its whole body");

    const MetaMessage* eot = as<MetaMessage>(t[5]);
    require(eot != nullptr, "End of Clip becomes an end-of-track meta");
    expect(eot->type() == 0x2F && t[5].tick == 384, "at the clip's end, keeping its length");
}

void test_long_gap_uses_null_restart() {
    Sequence seq{TimingType::PPQ, 480};
    Track& track = seq.create_track();
    add(track, 0, Pointers::unique<ShortMessage>(MidiStatus::NOTE_ON, 0, 60, 1));
    add(track, 3'000'000, Pointers::unique<ShortMessage>(MidiStatus::NOTE_OFF, 0, 60, 0));

    const Vector<u8> bytes = MidiClipFile::write(seq);
    bool saw_noop = false;
    for (usize i = 0; i < (bytes.size() - 8) / 4; ++i) {
        saw_noop = saw_noop || word_at(bytes, i) == 0x00000000u;
    }
    expect(saw_noop, "a gap past 20 bits is bridged with a Null (NOOP) restart");

    UniquePointer<Sequence> back = MidiClipFile::parse(Span<const u8>{bytes.data(), bytes.size()});
    require(back->track(0).size() >= 2, "both notes come back");
    expect_eq(back->track(0)[1].tick, i64{3'000'000}, "at exactly the original tick");
}

void test_write_midi2_protocol() {
    Sequence seq{TimingType::PPQ, 96};
    add(seq.create_track(), 0, Pointers::unique<ShortMessage>(MidiStatus::NOTE_ON, 0, 60, 127));
    const Vector<u8> bytes = MidiClipFile::write(seq, UmpProtocol::MIDI2);
    UniquePointer<Sequence> back = MidiClipFile::parse(Span<const u8>{bytes.data(), bytes.size()});
    const Midi2ChannelVoiceMessage* note = as<Midi2ChannelVoiceMessage>(back->track(0)[0]);
    require(note != nullptr, "under MIDI2 a MIDI 1.0 note is written as MIDI 2.0");
    expect_eq(note->velocity(), u16{0xFFFF}, "with widened velocity");
}

void test_write_rejects_smpte() {
    Sequence seq{TimingType::SMPTE, -(25 << 8) | 40};
    expect_throws<InvalidMidiDataException>(
        [&] -> void { static_cast<void>(MidiClipFile::write(seq)); },
        "a clip file can only express PPQ timing"
    );
}

void test_configuration_header_messages_lead_tick_zero() {
    Vector<u8> bytes = header();
    put_packet(bytes, UtilityMessage::delta_clockstamp(0).packet());
    put_packet(bytes, UtilityMessage::delta_clockstamp_tpq(480).packet());
    put_packet(bytes, UtilityMessage::delta_clockstamp(0).packet());
    put_packet(bytes, Midi1ChannelVoiceMessage{0, MidiStatus::PROGRAM_CHANGE, 0, 5}.packet()); // config
    put_packet(bytes, UtilityMessage::delta_clockstamp(100).packet()); // config header time
    put_packet(bytes, UmpStreamMessage::start_of_clip().packet());
    put_packet(bytes, Midi1ChannelVoiceMessage{0, MidiStatus::NOTE_ON, 0, 60, 90}.packet());
    put_packet(bytes, UtilityMessage::delta_clockstamp(480).packet());
    put_packet(bytes, UmpStreamMessage::end_of_clip().packet());

    UniquePointer<Sequence> seq = MidiClipFile::parse(Span<const u8>{bytes.data(), bytes.size()});
    const Track& t = seq->track(0);
    require(t.size() == 3, "program change, note, end of track");
    const Midi1ChannelVoiceMessage* pc = as<Midi1ChannelVoiceMessage>(t[0]);
    require(pc != nullptr, "the configuration message is kept");
    expect(pc->command() == MidiStatus::PROGRAM_CHANGE && t[0].tick == 0, "at tick 0, ahead of the clip");
    expect_eq(t[1].tick, i64{0}, "clip time restarts at Start of Clip, not after the header's 100 ticks");
    expect_eq(t[2].tick, i64{480}, "End of Clip at 480");
}

void test_parse_rejects_malformed() {
    const auto rejects = [](const Vector<u8>& bytes, const char* what) -> void {
        expect_throws<InvalidMidiDataException>(
            [&] -> void { static_cast<void>(MidiClipFile::parse(Span<const u8>{bytes.data(), bytes.size()})); },
            what
        );
    };

    Vector<u8> no_magic = {'M', 'T', 'h', 'd', 0, 0, 0, 6};
    rejects(no_magic, "a file without SMF2CLIP is rejected");

    Vector<u8> no_tpq = header();
    put_packet(no_tpq, UtilityMessage::delta_clockstamp(0).packet());
    put_packet(no_tpq, UmpStreamMessage::start_of_clip().packet());
    rejects(no_tpq, "a clip without DCTPQ is rejected");

    Vector<u8> ragged = header();
    put_packet(ragged, UtilityMessage::delta_clockstamp(0).packet());
    ragged.push_back(0x00);
    rejects(ragged, "data that is not whole words is rejected");

    Vector<u8> cut = header();
    put_packet(cut, UtilityMessage::delta_clockstamp(0).packet());
    put_packet(cut, UtilityMessage::delta_clockstamp_tpq(96).packet());
    put_word(cut, Midi2ChannelVoiceMessage::note_on(0, 0, 60, 1).packet().word(0)); // half a 64-bit packet
    rejects(cut, "a clip that ends mid-packet is rejected");
}

void test_open_sequence_detects_clip() {
    const Path dir = stdx::fs::temp_directory_path() / "stdx_clip_test";
    stdx::fs::remove_all(dir);
    stdx::fs::create_directories(dir);
    const Path path = dir / "clip.midi2";

    Sequence seq{TimingType::PPQ, 240};
    add(seq.create_track(), 10, Pointers::unique<ShortMessage>(MidiStatus::NOTE_ON, 0, 60, 1));
    MidiClipFile::save(seq, path);

    UniquePointer<Sequence> back = MidiSystem::open_sequence(path);
    require(back != nullptr, "open_sequence reads a clip file");
    expect_eq(back->division(), 240, "recognised by its header, not its extension");
    expect_eq(back->track(0)[0].tick, i64{10}, "with its events");
    stdx::fs::remove_all(dir);
}

namespace {
    /**
     * @brief Receiver that records what reaches it.
     */
    class Recorder final: public Receiver {
    public:
        Mutex mutex;
        Vector<String> kinds;

        void send(const MidiMessage& msg, [[maybe_unused]] u64 time_micros) override {
            LockGuard<Mutex> guard{mutex};
            kinds.push_back(
                dynamic_cast<const FlexDataMessage*>(&msg) ? "flex"
                : dynamic_cast<const Midi2ChannelVoiceMessage*>(&msg) ? "midi2"
                : dynamic_cast<const ShortMessage*>(&msg) ? "short"
                : "other"
            );
        }

        void close() noexcept override {}
    };
}

void test_sequencer_plays_ump_sequence() {
    UniquePointer<Sequence> seq = Pointers::unique<Sequence>(TimingType::PPQ, 96);
    Track& track = seq->create_track();
    // 10 ms per quarter: the whole sequence plays in a few milliseconds.
    add(track, 0, Pointers::unique<FlexDataMessage>(FlexDataMessage::set_tempo(0, 10'000 * 100)));
    add(track, 0, Pointers::unique<UmpStreamMessage>(UmpStreamMessage::start_of_clip()));
    add(track, 0, Pointers::unique<UtilityMessage>(UtilityMessage::delta_clockstamp(0)));
    add(track, 24, Pointers::unique<Midi2ChannelVoiceMessage>(Midi2ChannelVoiceMessage::note_on(0, 0, 60, 0x8000)));
    add(track, 48, Pointers::unique<ShortMessage>(MidiStatus::NOTE_OFF, 0, 60, 0));
    add(track, 48, end_of_track());

    Recorder recorder;
    Sequencer& sequencer = MidiSystem::default_sequencer();
    sequencer.transmitter()->set_receiver(&recorder);
    sequencer.set_sequence(Ops::move(seq));
    sequencer.start();
    for (int i = 0; i < 500 && sequencer.is_running(); ++i) {
        Thread::sleep_for(2ms);
    }
    sequencer.stop();
    sequencer.transmitter()->set_receiver(nullptr);

    LockGuard<Mutex> guard{recorder.mutex};
    require(recorder.kinds.size() == 3, "tempo, note on and note off reach the receiver; utility and stream do not");
    expect_eq(recorder.kinds[0], "flex"s, "Flex Set Tempo is forwarded");
    expect_eq(recorder.kinds[1], "midi2"s, "the UMP note");
    expect_eq(recorder.kinds[2], "short"s, "the MIDI 1.0 note");
    expect(sequencer.tempo_bpm() > 5999.0 && sequencer.tempo_bpm() < 6001.0, "Flex Set Tempo set the playback tempo");
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"MidiClip.write_header_layout", test_write_header_layout},
        {"MidiClip.round_trip", test_round_trip},
        {"MidiClip.long_gap_uses_null_restart", test_long_gap_uses_null_restart},
        {"MidiClip.write_midi2_protocol", test_write_midi2_protocol},
        {"MidiClip.write_rejects_smpte", test_write_rejects_smpte},
        {"MidiClip.configuration_header_messages_lead_tick_zero", test_configuration_header_messages_lead_tick_zero},
        {"MidiClip.parse_rejects_malformed", test_parse_rejects_malformed},
        {"MidiClip.open_sequence_detects_clip", test_open_sequence_detects_clip},
        {"MidiClip.sequencer_plays_ump_sequence", test_sequencer_plays_ump_sequence},
    });
}
