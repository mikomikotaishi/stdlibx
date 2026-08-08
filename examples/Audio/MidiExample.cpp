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
using stdx::sys::ExitStatus;
using stdx::sys::Stdio;
using stdx::sys::Process;
using stdx::thread::Thread;
using stdx::util::ArgumentParser;
using stdx::util::DefaultArguments;

[[nodiscard]]
StringView backendName(MidiBackend b) noexcept {
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
    }
    Ops::unreachable();
}

void listDevices() {
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
            backendName(d.backend),
            d.is_input ? "yes" : "no",
            d.is_output ? "yes" : "no"
        );
    }
}

[[nodiscard]]
bool isPassthroughPort(const MidiDeviceInfo& d) noexcept {
    return d.name.starts_with("Midi Through")
        || d.name.starts_with("System");
}

[[nodiscard]]
bool hasRealOutput(const Vector<MidiDeviceInfo>& devices) noexcept {
    for (const MidiDeviceInfo& d: devices) {
        if (d.is_output && !isPassthroughPort(d)) {
            return true;
        }
    }
    return false;
}

// Common system SoundFont locations across distros. First hit wins.
Optional<Path> findSoundFont() noexcept {
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
// ALSA only move bytes, they don't synthesize audio. If the user has FluidSynth
// installed but no synth is currently serving an ALSA-seq port, spawn one for
// the duration of the test. Returns the Process handle so main() can reap it.
Optional<Process> maybeSpawnFluidSynth() {
    Vector<MidiDeviceInfo> devices;
    try {
        devices = MidiSystem::devices();
    } catch (const MidiException& _) {
        return nullopt;
    }
    if (hasRealOutput(devices)) {
        return nullopt;
    }

    Optional<Path> sf2 = findSoundFont();
    if (!sf2.has_value()) {
        System::out.println(
            "No soft synth running and no system SoundFont found. Install one "
            "(e.g. `pacman -S soundfont-fluid` or `apt install fluid-soundfont-gm`) "
            "to enable MIDI playback."
        );
        return nullopt;
    }

    System::out.println("Auto-launching FluidSynth with {}", sf2.value());

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
        .spawn();
    if (!child.has_value()) {
        System::out.println(
            "Failed to launch FluidSynth - is it installed and on PATH?"
        );
        return nullopt;
    }

    // Give the synth time to register its ALSA-seq port before we re-enumerate.
    Thread::sleep_for(1500ms);
    return Optional<Process>{Ops::move(*child)};
}

void playArpeggio(const String& requestedId) {
    Vector<MidiDeviceInfo> devices;
    try {
        devices = MidiSystem::devices();
    } catch (const MidiException& e) {
        System::out.println("Cannot enumerate MIDI: {}", e.what());
        return;
    }

    const MidiDeviceInfo* target = nullptr;
    if (!requestedId.empty()) {
        for (const MidiDeviceInfo& d: devices) {
            if (d.is_output && d.id == requestedId) {
                target = &d;
                break;
            }
        }
        if (target == nullptr) {
            System::out.println(
                "Requested device id={} is not an output-capable MIDI port.",
                requestedId
            );
            return;
        }
    } else {
        for (const MidiDeviceInfo& d: devices) {
            if (d.is_output && !isPassthroughPort(d)) {
                target = &d;
                break;
            }
        }
        if (target == nullptr) {
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
    if (target == nullptr) {
        System::out.println("No output-capable MIDI device available - skipping.");
        return;
    }

    System::out.println("Targeting: {} (id={})", target->name, target->id);

    try {
        UniquePointer<MidiDevice> device = MidiSystem::open_device(*target);
        Receiver* rx = device->receiver();
        if (rx == nullptr) {
            System::out.println("Device has no receiver - nothing to do.");
            return;
        }

        // C major arpeggio: C4, E4, G4, C5
        static constexpr Array<u8, 4> C_MAJOR_ARPEGGIO = {60, 64, 67, 72};
        static constexpr u8 CHANNEL = 0;
        static constexpr u8 VELOCITY = 100;

        for (u8 note: C_MAJOR_ARPEGGIO) {
            ShortMessage on(Status::NOTE_ON, CHANNEL, note, VELOCITY);
            rx->send(on, 0);
            System::out.println("  NoteOn  ch={} note={} vel={}", CHANNEL, note, VELOCITY);
            Thread::sleep_for(200ms);

            ShortMessage off(Status::NOTE_OFF, CHANNEL, note, 0);
            rx->send(off, 0);
            Thread::sleep_for(50ms);
        }

        device->close();
    } catch (const MidiException& e) {
        System::err.println("MIDI send failed: {}", e.what());
    }
}

void play_smf(const Path& file, const String& requestedId) {
    Vector<MidiDeviceInfo> devices;
    try {
        devices = MidiSystem::devices();
    } catch (const MidiException& e) {
        System::err.println("Cannot enumerate MIDI: {}", e.what());
        return;
    }

    // Pick the destination Receiver - same logic as play_arpeggio.
    const MidiDeviceInfo* target = nullptr;
    if (!requestedId.empty()) {
        for (const MidiDeviceInfo& d: devices) {
            if (d.is_output && d.id == requestedId) {
                target = &d;
                break;
            }
        }
        if (target == nullptr) {
            System::out.println(
                "Requested device id={} is not an output-capable MIDI port.",
                requestedId
            );
            return;
        }
    } else {
        for (const MidiDeviceInfo& d: devices) {
            if (d.is_output && !isPassthroughPort(d)) {
                target = &d;
                break;
            }
        }
    }
    if (target == nullptr) {
        System::out.println(
            "No non-pass-through output found. Start a soft synth (e.g. "
            "`fluidsynth -a alsa -m alsa_seq /usr/share/soundfonts/Arachno.sf2`)."
        );
        return;
    }

    UniquePointer<Sequence> seq;
    try {
        seq = MidiSystem::open_sequence(file);
    } catch (const InvalidMidiDataException& e) {
        System::err.println("Cannot parse {}: {}", file, e.what());
        return;
    }
    System::out.println(
        "Parsed {}: {} track(s), last tick = {}",
        file, seq->track_count(), seq->last_tick()
    );

    try {
        UniquePointer<MidiDevice> device = MidiSystem::open_device(*target);
        Receiver* rx = device->receiver();
        if (rx == nullptr) {
            System::out.println("Device has no receiver - nothing to do.");
            return;
        }

        Sequencer& sequencer = MidiSystem::default_sequencer();
        sequencer.open();
        Transmitter* tx = sequencer.transmitter();
        if (tx == nullptr) {
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
    // skip the auto-spawn step. Otherwise spawn FluidSynth so MidiTest is
    // self-contained when no synth is running on the system.
    const String deviceArg = parser.get("--device");
    const String fileArg = parser.get("--file");
    const bool wantsToPlay = !fileArg.empty() || parser.get<bool>("--play");
    Optional<Process> autoSynth;
    if (wantsToPlay && deviceArg.empty()) {
        autoSynth = maybeSpawnFluidSynth();
    }

    listDevices();

    if (!fileArg.empty()) {
        System::out.println("\nPlaying SMF: {}", fileArg);
        play_smf(fileArg, deviceArg);
    } else if (parser.get<bool>("--play")) {
        System::out.println("\nSending a C-major arpeggio...");
        playArpeggio(deviceArg);
    } else {
        System::out.println(
            "\n(Pass --play for an arpeggio or --file <path> to play an SMF.)"
        );
    }

    // Reap the synth we spawned. kill() sends SIGTERM; wait() reaps the zombie.
    // Dropping a Process without wait() per the API contract leaves the process
    // unreaped, so this is required even if kill() reports an error.
    if (autoSynth.has_value()) {
        if (Expected<void, ErrorCode> killResult = autoSynth->kill(); killResult.error()) {
            System::err.println("Failed to kill FluidSynth: {}", killResult.error().message());
        }

        if (Expected<ExitStatus, ErrorCode> waitResult = autoSynth->wait(); waitResult) {
            System::out.println("FluidSynth exited successfully.");
        } else {
            System::err.println("Failed to wait for FluidSynth: {}", waitResult.error().message());
        }
    }
}
