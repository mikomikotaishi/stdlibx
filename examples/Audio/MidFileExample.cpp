import stdx;

using stdx::audio::midi::InvalidMidiDataException;
using stdx::audio::midi::MetaMessage;
using stdx::audio::midi::MidiDevice;
using stdx::audio::midi::MidiDeviceInfo;
using stdx::audio::midi::MidiException;
using stdx::audio::midi::MidiMessage;
using stdx::audio::midi::MidiSystem;
using stdx::audio::midi::Receiver;
using stdx::audio::midi::Sequence;
using stdx::audio::midi::Sequencer;
using stdx::audio::midi::ShortMessage;
using stdx::audio::midi::Status;
using stdx::audio::midi::SysexMessage;
using stdx::audio::midi::TimingType;
using stdx::audio::midi::Track;
using stdx::audio::midi::Transmitter;
using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::mem::UniquePointer;
using stdx::sys::ExitStatus;
using stdx::sys::Stdio;
using stdx::sys::Process;
using stdx::thread::Thread;
using stdx::time::Duration;
using stdx::time::Instant;
using stdx::time::SteadyClock;
using stdx::util::ArgumentParser;
using stdx::util::DefaultArguments;

#ifdef __GNUC__
using namespace stdx::core;
using namespace stdx::literals;
#endif

[[nodiscard]]
Optional<Path> resolve_default_sample() noexcept {
    static constexpr Array<StringView, 4> CANDIDATES = {
        "examples/Audio/audio/midi/th06/th06_01-SD90_440Hz.mid",
        "../examples/Audio/audio/midi/th06/th06_01-SD90_440Hz.mid",
        "../../examples/Audio/audio/midi/th06/th06_01-SD90_440Hz.mid",
        "../../../examples/Audio/audio/midi/th06/th06_01-SD90_440Hz.mid",
    };
    for (StringView p: CANDIDATES) {
        Path path{p};
        if (stdx::fs::exists(path)) {
            return path;
        }
    }
    return nullopt;
}

[[nodiscard]]
bool is_passthrough_port(const MidiDeviceInfo& d) noexcept {
    return d.name.starts_with("Midi Through") || d.name.starts_with("System");
}

[[nodiscard]]
Optional<Path> find_soundfont() noexcept {
    static constexpr Array<StringView, 6> CANDIDATES = {
        "/usr/share/soundfonts/Arachno.sf2",
        "/usr/share/soundfonts/FluidR3_GM.sf2",
        "/usr/share/soundfonts/default.sf2",
        "/usr/share/sounds/sf2/FluidR3_GM.sf2",
        "/usr/share/sounds/sf2/default-GM.sf2",
        "/usr/share/sounds/sf2/TimGM6mb.sf2",
    };
    for (StringView p: CANDIDATES) {
        Path path{p};
        if (stdx::fs::exists(path)) {
            return path;
        }
    }
    return nullopt;
}

// See MidiTest.cpp for the rationale - MIDI is just messages; on Linux you
// need a synthesizer process to turn them into audio. If none is running and
// a soundfont is installed, fork fluidsynth for the duration of the test.
Optional<Process> maybe_spawn_fluidsynth() {
    Vector<MidiDeviceInfo> devices;
    try {
        devices = MidiSystem::devices();
    } catch (const MidiException& _) {
        return nullopt;
    }
    for (const MidiDeviceInfo& d: devices) {
        if (d.is_output && !is_passthrough_port(d)) {
            return nullopt;
        }
    }

    Optional<Path> sf2 = find_soundfont();
    if (!sf2.has_value()) {
        System::out.println(
            "No soft synth running and no system soundfont found. Install one "
            "(e.g. `pacman -S soundfont-fluid`) to enable MIDI playback."
        );
        return nullopt;
    }

    System::out.println("Auto-launching fluidsynth with {}", sf2.value());

    Expected<Process, ErrorCode> child = Process::Builder("fluidsynth")
        .arg("-s")
        .arg("-i")
        .arg("-q")
        .arg("-a")
        .arg("alsa")
        .arg("-m")
        .arg("alsa_seq")
        .arg(sf2.value().string())
        .stdin(Stdio::NULL_DEV)
        .stdout(Stdio::NULL_DEV)
        .stderr(Stdio::NULL_DEV)
        .terminate_on_parent_exit()
        .spawn();
    if (!child.has_value()) {
        System::out.println(
            "Failed to launch fluidsynth - is it installed and on PATH?"
        );
        return nullopt;
    }

    Thread::sleep_for(1500ms);
    return Optional<Process>{Ops::move(*child)};
}

struct EventCounts {
    u64 note_on = 0;
    u64 note_off = 0;
    u64 control_change = 0;
    u64 program_change = 0;
    u64 pitch_bend = 0;
    u64 other_short = 0;
    u64 sysex = 0;
    u64 meta = 0;
    u64 tempo_meta = 0;
};

void count_events(const Sequence& seq, EventCounts& out) noexcept {
    for (usize ti = 0; ti < seq.track_count(); ++ti) {
        const Track& t = seq.track(ti);
        for (usize ei = 0; ei < t.size(); ++ei) {
            const MidiMessage* msg = t[ei].message.get();
            if (!msg) {
                continue;
            }
            if (const SysexMessage* _ = dynamic_cast<const SysexMessage*>(msg)) {
                ++out.sysex;
            } else if (const MetaMessage* m = dynamic_cast<const MetaMessage*>(msg)) {
                ++out.meta;
                if (m->type() == 0x51) {
                    ++out.tempo_meta;
                }
            } else if (const ShortMessage* s = dynamic_cast<const ShortMessage*>(msg)) {
                switch (s->command()) {
                    case Status::NOTE_ON:
                        ++out.note_on;
                        break;
                    case Status::NOTE_OFF:
                        ++out.note_off;
                        break;
                    case Status::CONTROL_CHANGE:
                        ++out.control_change;
                        break;
                    case Status::PROGRAM_CHANGE:
                        ++out.program_change;
                        break;
                    case Status::PITCH_BEND:
                        ++out.pitch_bend;
                        break;
                    default:
                        ++out.other_short;
                        break;
                }
            }
        }
    }
}

void describe(const Path& path) {
    UniquePointer<Sequence> seq;
    try {
        seq = MidiSystem::open_sequence(path);
    } catch (const InvalidMidiDataException& e) {
        System::out.println("parses {}: failed", path.filename());
        System::err.println("       reason: {}", e.what());
        return;
    }
    System::out.println("parses {}: ok", path.filename());

    const TimingType timing = seq->timing_type();
    const i32 division = seq->division();
    System::out.println(
        "       timing: {}, division={}, tracks={}, last_tick={}",
        timing == TimingType::PPQ ? "PPQ" : "SMPTE",
        division, seq->track_count(), seq->last_tick()
    );
    if (timing == TimingType::SMPTE) {
        System::out.println(
            "       smpte:  {} fps, {} ticks/frame",
            seq->smpte_format(),
            static_cast<u32>(seq->smpte_ticks_per_frame())
        );
    }

    System::out.println("{}: {}", "has at least one track", seq->track_count() > 0);
    System::out.println("{}: {}", "non-zero last tick", seq->last_tick() > 0);

    EventCounts counts;
    count_events(*seq, counts);
    System::out.println(
        "       events: NoteOn={}, NoteOff={}, CC={}, PC={}, PB={}, "
        "OtherShort={}, Sysex={}, Meta={} (Tempo={})",
        counts.note_on, counts.note_off, counts.control_change,
        counts.program_change, counts.pitch_bend, counts.other_short,
        counts.sysex, counts.meta, counts.tempo_meta
    );

    System::out.println("{}: {}", "has Note On events", counts.note_on > 0);
    // SMF spec requires every track to end with End-of-Track meta (type 0x2F),
    // so meta count must be >= track count.
    System::out.println("{}: {}", "meta count >= track count", counts.meta >= seq->track_count());
}

// @p max_seconds caps playback at the first N seconds; 0 plays in full.
void play(const Path& path, i64 max_seconds) {
    UniquePointer<Sequence> seq;
    try {
        seq = MidiSystem::open_sequence(path);
    } catch (const InvalidMidiDataException& e) {
        System::err.println("Cannot parse {}: {}", path, e.what());
        return;
    }

    Vector<MidiDeviceInfo> devices;
    try {
        devices = MidiSystem::devices();
    } catch (const MidiException& e) {
        System::err.println("Cannot enumerate MIDI: {}", e.what());
        return;
    }
    const MidiDeviceInfo* target = nullptr;
    for (const MidiDeviceInfo& d: devices) {
        if (d.is_output && !is_passthrough_port(d)) {
            target = &d;
            break;
        }
    }
    if (!target) {
        System::out.println(
            "No real synth available - skipping playback. "
            "(The auto-launcher might have failed; check fluidsynth is installed.)"
        );
        return;
    }

    if (max_seconds > 0) {
        System::out.println(
            "Playing {} -> {} (first {}s)",
            path.filename(),
            target->name, max_seconds
        );
    } else {
        System::out.println(
            "Playing {} -> {}",
            path.filename(),
            target->name
        );
    }

    try {
        UniquePointer<MidiDevice> device = MidiSystem::open_device(*target);
        Receiver* rx = device->receiver();
        if (!rx) {
            System::err.println("Device has no receiver.");
            return;
        }
        Sequencer& sequencer = MidiSystem::default_sequencer();
        sequencer.open();
        Transmitter* tx = sequencer.transmitter();
        if (!tx) {
            System::err.println("Sequencer has no transmitter.");
            return;
        }
        tx->set_receiver(rx);
        sequencer.set_sequence(Ops::move(seq));
        // Measure wall-clock playback time so an abridged run can stop the
        // sequencer before the sequence ends. chrono's non-member operators
        // aren't reachable across the `import stdx;` boundary, so diff raw tick
        // counts (member fns only) using the clock's compile-time tick period.
        constexpr i64 TICKS_PER_SECOND =
            SteadyClock::period::den / SteadyClock::period::num;
        const i64 start_ticks = SteadyClock::now().time_since_epoch().count();
        sequencer.start();

        while (sequencer.is_running()) {
            const i64 elapsed_ticks =
                SteadyClock::now().time_since_epoch().count() - start_ticks;
            if (max_seconds > 0 && elapsed_ticks >= max_seconds * TICKS_PER_SECOND) {
                break;
            }
            Thread::sleep_for(50ms);
        }
        sequencer.stop();
        tx->set_receiver(nullptr);
        device->close();
        System::out.println("Playback finished.");
    } catch (const MidiException& e) {
        System::err.println("Playback failed: {}", e.what());
    }
}

int main(int argc, char* argv[]) {
    ArgumentParser parser("MidFileTest", "1.0", DefaultArguments::NONE);
    parser.add_argument("--file")
        .default_value("")
        .help("Standard MIDI File to test. If omitted, the bundled th06 sample is used.");
    parser.add_argument("--play")
        .flag()
        .help(
            "After parsing, also play the file via the soft sequencer. "
            "Auto-launches fluidsynth if no synth is on the bus."
        );
    parser.add_argument("--abridge")
        .default_value(static_cast<i64>(0))
        .scan<'d', i64>()
        .help(
            "Limit playback to the first N seconds (e.g. --abridge 10). "
            "0 (the default) plays the file in full. Only affects --play."
        );

    parser.parse_args(argc, argv);

    const String file_arg = parser.get("--file");
    Path path;
    if (!file_arg.empty()) {
        path = Path{file_arg};
        if (!stdx::fs::exists(path)) {
            System::err.println("File not found: {}", file_arg);
            return System::EXIT_FAILURE;
        }
    } else {
        Optional<Path> resolved = resolve_default_sample();
        if (!resolved.has_value()) {
            System::err.println(
                "Could not locate the bundled sample. Pass --file <path> or "
                "run the test from the project root."
            );
            return System::EXIT_FAILURE;
        }
        path = *resolved;
    }

    // Only spawn a synth if the user actually wants playback - pure parsing
    // doesn't need any audio path.
    const bool wants_play = parser.get<bool>("--play");
    Optional<Process> auto_synth;
    if (wants_play) {
        auto_synth = maybe_spawn_fluidsynth();
    }

    describe(path);

    if (wants_play) {
        System::out.println("");
        play(path, parser.get<i64>("--abridge"));
    }

    if (auto_synth.has_value()) {
        if (Expected<void, ErrorCode> kill_result = auto_synth->kill(); kill_result.error()) {
            System::err.println("Failed to kill fluidsynth: {}", kill_result.error().message());
        }

        if (Expected<ExitStatus, ErrorCode> wait_result = auto_synth->wait(); wait_result) {
            System::out.println("fluidsynth exited successfully.");
        } else {
            System::err.println("Failed to wait for fluidsynth: {}", wait_result.error().message());
        }
    }

    return System::EXIT_SUCCESS;
}
