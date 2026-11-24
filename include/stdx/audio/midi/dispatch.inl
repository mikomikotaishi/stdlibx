#pragma once

using stdx::fs::Path;
using stdx::mem::UniquePointer;

/**
 * @namespace stdx::audio::midi
 * @brief Cross-platform MidiSystem dispatch - methods whose body is
 * the same on every backend live here, so the platform-specific
 * files (alsa_seq.inl, winmm.inl, coremidi.inl) only need to provide
 * the parts that genuinely differ.
 */
export namespace stdx::audio::midi {
    UniquePointer<Sequence> MidiSystem::open_sequence(const Path& path) {
        // Sniff the header rather than trusting the extension: "SMF2CLIP"
        // is a MIDI 2.0 clip file, anything else goes to the SMF parser
        // (which reports a missing MThd itself).
        u8 header[8] = {};
        {
            InputFileStream f{path, OpenMode::BINARY};
            if (!f) {
                throw InvalidMidiDataException("cannot open MIDI file");
            }
            f.read(reinterpret_cast<char*>(header), sizeof(header));
            if (f.gcount() == sizeof(header) && MidiClipFile::is_clip(Span<const u8>{header, sizeof(header)})) {
                f.close();
                return MidiClipFile::read(path);
            }
        }
        return parse_smf(path);
    }

    Sequencer& MidiSystem::default_sequencer() {
        static SoftSequencer instance;
        return instance;
    }
}
