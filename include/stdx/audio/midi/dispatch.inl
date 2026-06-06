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
    UniquePointer<Sequence> MidiSystem::open_sequence(
        const Path& path
    ) throws (InvalidMidiDataException) {
        return parse_smf(path);
    }

    Sequencer& MidiSystem::default_sequencer() throws (MidiException) {
        static SoftSequencer instance;
        return instance;
    }
}
