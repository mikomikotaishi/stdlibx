#pragma once

using stdx::collections::Vector;
using stdx::fs::Path;

/**
 * @namespace stdx::audio::midi
 * @brief MidiSystem - entry point for MIDI device enumeration.
 */
export namespace stdx::audio::midi {
    /**
     * @class MidiSystem
     * @brief Entry point for MIDI device enumeration.
     * Provides static methods to list devices and open them by info, as well as
     * access to built-in software synth and sequencer implementations.
     */
    class MidiSystem final {
    public:
        MidiSystem() = delete;

        [[nodiscard]]
        static Vector<MidiDeviceInfo> devices() throws (MidiException);

        [[nodiscard]]
        static UniquePointer<MidiDevice> open_device(const MidiDeviceInfo& info) throws (MidiException);

        /**
         * @brief Returns the built-in software synthesizer, bound (lazily on
         * first call) to the default sampled output. The returned
         * reference is owned by the system and lives for the duration
         * of the process.
         * @return Reference to the default synthesizer.
         * @throws AudioException if no audio output is available.
         * @throws MidiException if the synthesizer fails to initialize.
         */
        [[nodiscard]]
        static Synthesizer& default_synthesizer() throws (AudioException, MidiException);

        /**
         * @brief Returns the built-in software sequencer. Its transmitter
         * is left disconnected - wire it to a Synthesizer's
         * receiver, or to any MidiDevice receiver, before start().
         * @return Reference to the default sequencer.
         * @throws MidiException if the sequencer fails to initialize.
         */
        [[nodiscard]]
        static Sequencer& default_sequencer() throws (MidiException);

        /**
         * @brief Parse a Standard MIDI File (.mid) into a Sequence.
         * @param path Path to a format-0/1/2 SMF file.
         * @throws InvalidMidiDataException on malformed header or tracks.
         */
        [[nodiscard]]
        static UniquePointer<Sequence> open_sequence(
            const Path& path
        ) throws (InvalidMidiDataException);
    };
}

#if defined(__linux__) && __has_include(<alsa/asoundlib.h>)
#include "stdx/audio/midi/linux/alsa_seq.inl"
#elif defined(_WIN32) && __has_include(<mmsystem.h>)
// WinMM backend - not yet implemented; falls back to throwing stubs so the
// public API still links. See stdx/audio/midi/win32/winmm.inl when added.
export namespace stdx::audio::midi {
    Vector<MidiDeviceInfo> MidiSystem::devices() throws (MidiException) {
        return {};
    }
    UniquePointer<MidiDevice> MidiSystem::open_device(
        const MidiDeviceInfo&
    ) throws (MidiException) {
        throw MidiException("WinMM MIDI backend not yet wired");
    }
    Synthesizer& MidiSystem::default_synthesizer() throws (AudioException, MidiException) {
        throw MidiException("SoftSynthesizer not yet wired");
    }
}
#elif defined(__APPLE__)
// CoreMIDI backend - not yet implemented; same stub fallback.
export namespace stdx::audio::midi {
    Vector<MidiDeviceInfo> MidiSystem::devices() throws (MidiException) {
        return {};
    }
    UniquePointer<MidiDevice> MidiSystem::open_device(
        const MidiDeviceInfo&
    ) throws (MidiException) {
        throw MidiException("CoreMIDI backend not yet wired");
    }
    Synthesizer& MidiSystem::default_synthesizer() throws (AudioException, MidiException) {
        throw MidiException("SoftSynthesizer not yet wired");
    }
}
#else
export namespace stdx::audio::midi {
    Vector<MidiDeviceInfo> MidiSystem::devices() throws (MidiException) {
        return {};
    }

    UniquePointer<MidiDevice> MidiSystem::open_device(
        const MidiDeviceInfo&
    ) throws (MidiException) {
        throw MidiException("no MIDI backend wired in this build");
    }

    Synthesizer& MidiSystem::default_synthesizer() throws (AudioException, MidiException) {
        throw MidiException("default synthesizer not wired; ship the SoftSynthesizer backend");
    }
}
#endif
