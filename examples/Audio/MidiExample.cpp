import stdx;

using stdx::audio::midi::InvalidMidiDataException;
using stdx::audio::midi::MidiBackend;
using stdx::audio::midi::MidiDevice;
using stdx::audio::midi::MidiDeviceInfo;
using stdx::audio::midi::MidiException;
using stdx::audio::midi::MidiSystem;
using stdx::audio::midi::Receiver;
using stdx::audio::midi::Sequence;
using stdx::audio::midi::Sequencer;
using stdx::audio::midi::ShortMessage;
using stdx::audio::midi::Status;
using stdx::audio::midi::Transmitter;
using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::mem::UniquePointer;
using stdx::process::Child;
using stdx::process::Command;
using stdx::process::Stdio;
using stdx::util::ArgumentParser;
using stdx::util::DefaultArguments;

#ifdef __GNUC__
using namespace stdx::core;
using namespace stdx::literals;
#endif

[[nodiscard]]
StringView backend_name(MidiBackend b) noexcept {
    switch (b) {
        case MidiBackend::NONE:
            return "none";
        case MidiBackend::ALSA_SEQ:
            return "alsa-seq";
        case MidiBackend::ALSA_RAW_MIDI:
            return "alsa-rawmidi";
        case MidiBackend::WINMM:
            return "winmm";
        case MidiBackend::CORE_MIDI:
            return "coremidi";
        case MidiBackend::SOFT:
            return "soft";
        default:
            Ops::unreachable();
    }
    Ops::unreachable();
}

void list_devices() {
    Vector<MidiDeviceInfo> devices;
    try {
        devices = MidiSystem::devices();
    } catch (const MidiException& e) {
        System::err.println("devices() threw: {}", e.what());
        return;
    }

    System::out.println("MIDI devices: {}", devices.size());
    for (const MidiDeviceInfo& d: devices) {
        System::out.println(
            "  - {} (id={}, backend={}, in={}, out={})",
            d.name,
            d.id,
            backend_name(d.backend),
            d.is_input ? "yes" : "no",
            d.is_output ? "yes" : "no"
        );
    }
}

[[nodiscard]]
bool is_passthrough_port(const MidiDeviceInfo& d) noexcept {
    return d.name.starts_with("Midi Through") || d.name.starts_with("System");
}

[[nodiscard]]
bool has_real_output(const Vector<MidiDeviceInfo>& devices) noexcept {
    for (const MidiDeviceInfo& d: devices) {
        if (d.is_output && !is_passthrough_port(d)) {
            return true;
        }
    }
    return false;
}

// Common system soundfont locations across distros. First hit wins.
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

// On Linux, MIDI without an external synth produces no sound - the kernel and
// ALSA only move bytes, they don't synthesize audio. If the user has fluidsynth
// installed but no synth is currently serving an ALSA-seq port, spawn one for
// the duration of the test. Returns the Child handle so main() can reap it.
Optional<Child> maybe_spawn_fluidsynth() {
    Vector<MidiDeviceInfo> devices;
    try {
        devices = MidiSystem::devices();
    } catch (const MidiException& _) {
        return nullopt;
    }
    if (has_real_output(devices)) {
        return nullopt;
    }

    Optional<Path> sf2 = find_soundfont();
    if (!sf2.has_value()) {
        System::out.println(
            "No soft synth running and no system soundfont found. Install one "
            "(e.g. `pacman -S soundfont-fluid` or `apt install fluid-soundfont-gm`) "
            "to enable MIDI playback."
        );
        return nullopt;
    }

    System::out.println("Auto-launching fluidsynth with {}", sf2.value());

    Expected<Child, ErrorCode> child = Command::from("fluidsynth")
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
        .spawn();
    if (!child.has_value()) {
        System::out.println(
            "Failed to launch fluidsynth - is it installed and on PATH?"
        );
        return nullopt;
    }

    // Give the synth time to register its ALSA-seq port before we re-enumerate.
    System::Thread::sleep_for(1500ms);
    return Optional<Child>{Ops::move(*child)};
}

void play_arpeggio(const String& requested_id) {
    Vector<MidiDeviceInfo> devices;
    try {
        devices = MidiSystem::devices();
    } catch (const MidiException& e) {
        System::out.println("Cannot enumerate MIDI: {}", e.what());
        return;
    }

    const MidiDeviceInfo* target = nullptr;
    if (!requested_id.empty()) {
        for (const MidiDeviceInfo& d: devices) {
            if (d.is_output && d.id == requested_id) {
                target = &d;
                break;
            }
        }
        if (!target) {
            System::out.println(
                "Requested device id={} is not an output-capable MIDI port.",
                requested_id
            );
            return;
        }
    } else {
        for (const MidiDeviceInfo& d: devices) {
            if (d.is_output && !is_passthrough_port(d)) {
                target = &d;
                break;
            }
        }
        if (!target) {
            for (const MidiDeviceInfo& d: devices) {
                if (d.is_output) {
                    target = &d;
                    break;
                }
            }
            if (target) {
                System::out.println(
                    "Only a pass-through MIDI port is available ({}). "
                    "Start a soft synth (e.g. `fluidsynth -a pipewire -m alsa_seq "
                    "/usr/share/soundfonts/Arachno.sf2`) to hear sound, then "
                    "re-run with `--device <id>` pointing at it.",
                    target->name
                );
            }
        }
    }
    if (!target) {
        System::out.println("No output-capable MIDI device available - skipping.");
        return;
    }

    System::out.println("Targeting: {} (id={})", target->name, target->id);

    try {
        UniquePointer<MidiDevice> device = MidiSystem::open_device(*target);
        Receiver* rx = device->receiver();
        if (!rx) {
            System::out.println("Device has no receiver - nothing to do.");
            return;
        }

        // C major arpeggio: C4, E4, G4, C5
        static constexpr Array<u8, 4> C_MAJOR_ARPEGGIO{60, 64, 67, 72};
        static constexpr u8 CHANNEL = 0;
        static constexpr u8 VELOCITY = 100;

        for (u8 note: C_MAJOR_ARPEGGIO) {
            ShortMessage on{Status::NOTE_ON, CHANNEL, note, VELOCITY};
            rx->send(on, 0);
            System::out.println("  NoteOn  ch={} note={} vel={}", CHANNEL, note, VELOCITY);
            System::Thread::sleep_for(200ms);

            ShortMessage off{Status::NOTE_OFF, CHANNEL, note, 0};
            rx->send(off, 0);
            System::Thread::sleep_for(50ms);
        }

        device->close();
    } catch (const MidiException& e) {
        System::err.println("MIDI send failed: {}", e.what());
    }
}

void play_smf(const String& file_path, const String& requested_id) {
    Vector<MidiDeviceInfo> devices;
    try {
        devices = MidiSystem::devices();
    } catch (const MidiException& e) {
        System::err.println("Cannot enumerate MIDI: {}", e.what());
        return;
    }

    // Pick the destination Receiver - same logic as play_arpeggio.
    const MidiDeviceInfo* target = nullptr;
    if (!requested_id.empty()) {
        for (const MidiDeviceInfo& d: devices) {
            if (d.is_output && d.id == requested_id) {
                target = &d;
                break;
            }
        }
        if (!target) {
            System::out.println(
                "Requested device id={} is not an output-capable MIDI port.",
                requested_id
            );
            return;
        }
    } else {
        for (const MidiDeviceInfo& d: devices) {
            if (d.is_output && !is_passthrough_port(d)) {
                target = &d;
                break;
            }
        }
    }
    if (!target) {
        System::out.println(
            "No non-pass-through output found. Start a soft synth (e.g. "
            "`fluidsynth -a alsa -m alsa_seq /usr/share/soundfonts/Arachno.sf2`)."
        );
        return;
    }

    UniquePointer<Sequence> seq;
    try {
        seq = MidiSystem::open_sequence(Path{file_path});
    } catch (const InvalidMidiDataException& e) {
        System::err.println("Cannot parse {}: {}", file_path, e.what());
        return;
    }
    System::out.println(
        "Parsed {}: {} track(s), last tick = {}",
        file_path, seq->track_count(), seq->last_tick()
    );

    try {
        UniquePointer<MidiDevice> device = MidiSystem::open_device(*target);
        Receiver* rx = device->receiver();
        if (!rx) {
            System::out.println("Device has no receiver - nothing to do.");
            return;
        }

        Sequencer& sequencer = MidiSystem::default_sequencer();
        sequencer.open();
        Transmitter* tx = sequencer.transmitter();
        if (!tx) {
            System::out.println("Sequencer has no transmitter - nothing to do.");
            return;
        }
        tx->set_receiver(rx);
        sequencer.set_sequence(Ops::move(seq));

        System::out.println("Playing to: {} (id={})", target->name, target->id);
        sequencer.start();

        // Poll until the worker reports it's done. The sequencer self-stops
        // when it walks past the last event.
        while (sequencer.is_running()) {
            System::Thread::sleep_for(50ms);
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
    ArgumentParser parser("MidiTest", "1.0", DefaultArguments::NONE);
    parser.add_argument("--play")
        .flag()
        .help("Send a C-major arpeggio to an output-capable MIDI device.");
    parser.add_argument("--device")
        .default_value("")
        .help(
            "ALSA-seq client:port id to target (e.g. 128:0). "
            "If omitted, the first non-pass-through output device is chosen."
        );
    parser.add_argument("--file")
        .default_value("")
        .help(
            "Path to a Standard MIDI File (.mid). When given, plays the file "
            "to completion via the built-in soft sequencer, routed to the "
            "device chosen by --device."
        );

    parser.parse_args(argc, argv);

    // If the user already specified a device or didn't ask to play anything,
    // skip the auto-spawn step. Otherwise spawn fluidsynth so MidiTest is
    // self-contained when no synth is running on the system.
    const String device_arg = parser.get("--device");
    const String file_arg = parser.get("--file");
    const bool wants_to_play = !file_arg.empty() || parser.get<bool>("--play");
    Optional<Child> auto_synth;
    if (wants_to_play && device_arg.empty()) {
        auto_synth = maybe_spawn_fluidsynth();
    }

    list_devices();

    if (!file_arg.empty()) {
        System::out.println("\nPlaying SMF: {}", file_arg);
        play_smf(file_arg, device_arg);
    } else if (parser.get<bool>("--play")) {
        System::out.println("\nSending a C-major arpeggio...");
        play_arpeggio(device_arg);
    } else {
        System::out.println(
            "\n(Pass --play for an arpeggio or --file <path> to play an SMF.)"
        );
    }

    // Reap the synth we spawned. kill() sends SIGTERM; wait() reaps the zombie.
    // Dropping a Child without wait() per the API contract leaves the process
    // unreaped, so this is required even if kill() reports an error.
    if (auto_synth.has_value()) {
        (void)auto_synth->kill();
        (void)auto_synth->wait();
    }
}
