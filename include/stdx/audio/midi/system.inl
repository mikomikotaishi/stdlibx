#pragma once

using stdx::collections::Vector;
using stdx::fs::Path;

/**
 * @namespace stdx::audio::midi
 * @brief MidiSystem - entry point for MIDI device enumeration.
 */
export namespace stdx::audio::midi {
    /**
     * @struct VirtualEndpointOptions
     * @brief How to set up a virtual MIDI 2.0 endpoint.
     * @see MidiSystem::create_virtual_endpoint
     */
    struct VirtualEndpointOptions {
        String name = "stdx virtual endpoint"; ///< Shown to other applications; must not be empty.
        u8 groups = 1; ///< UMP groups the endpoint (and its one function block) spans, 1-16.
        UmpProtocol protocol = UmpProtocol::MIDI2; ///< The protocol the endpoint announces and MIDI 1.0 messages are sent in.
    };

    /**
     * @class MidiSystem
     * @brief Entry point for MIDI device enumeration.
     * Provides static methods to list devices and open them by info, as well as
     * access to built-in software synth and sequencer implementations.
     */
    class MidiSystem final {
    public:
        MidiSystem() = DELETE_METHOD("MidiSystem is a static utility class and cannot be instantiated.");

        [[nodiscard]]
        THROWS(MidiException)
        static Vector<MidiDeviceInfo> devices();

        [[nodiscard]]
        THROWS(MidiException)
        static UniquePointer<MidiDevice> open_device(const MidiDeviceInfo& info);

        /**
         * @brief Create a MIDI 2.0 endpoint of this application's own, which
         * other applications can connect to.
         * @param options Its name, group count and protocol.
         * @return The endpoint, already open: its Receiver sends to everything
         * connected to it, its Transmitter delivers what they send to it. Its
         * info() can be passed to open_device by another part of the program.
         * @throws MidiException if the platform has no virtual endpoints or the
         * endpoint cannot be created.
         *
         * On Linux this is an ALSA sequencer client in UMP mode, announced as a
         * UMP endpoint with one bidirectional function block. Legacy MIDI 1.0
         * clients can connect to it too; the sequencer converts.
         */
        [[nodiscard]]
        THROWS(MidiException)
        static UniquePointer<MidiDevice> create_virtual_endpoint(const VirtualEndpointOptions& options = {});

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
        THROWS(AudioException, MidiException)
        static Synthesizer& default_synthesizer();

        /**
         * @brief Returns the built-in software sequencer. Its transmitter
         * is left disconnected - wire it to a Synthesizer's
         * receiver, or to any MidiDevice receiver, before start().
         * @return Reference to the default sequencer.
         * @throws MidiException if the sequencer fails to initialize.
         */
        [[nodiscard]]
        THROWS(MidiException)
        static Sequencer& default_sequencer();

        /**
         * @brief Parse a MIDI file into a Sequence: a Standard MIDI File
         * (.mid, format 0/1/2) or a MIDI 2.0 Clip File (.midi2), told apart
         * by the file header.
         * @param path Path to the file.
         * @throws InvalidMidiDataException on malformed header or tracks.
         */
        [[nodiscard]]
        THROWS(InvalidMidiDataException)
        static UniquePointer<Sequence> open_sequence(const Path& path);
    };
}

#if defined(__linux__) && __has_include(<alsa/asoundlib.h>)
#include "stdx/audio/midi/linux/alsa_ump.inl"
#include "stdx/audio/midi/linux/alsa_seq_ump.inl" // before alsa_seq.inl, whose open_device uses it
#include "stdx/audio/midi/linux/alsa_seq.inl"
#elif defined(_WIN32) && __has_include(<mmsystem.h>)
// WinMM backend - not yet implemented; falls back to throwing stubs so the
// public API still links. See stdx/audio/midi/win32/winmm.inl when added.
export namespace stdx::audio::midi {
    Vector<MidiDeviceInfo> MidiSystem::devices() {
        return {};
    }

    UniquePointer<MidiDevice> MidiSystem::open_device([[maybe_unused]] const MidiDeviceInfo& info) {
        throw MidiException("WinMM MIDI backend not yet wired");
    }

    UniquePointer<MidiDevice> MidiSystem::create_virtual_endpoint([[maybe_unused]] const VirtualEndpointOptions& options) {
        throw MidiException("virtual MIDI endpoints are not available on this platform yet");
    }

    Synthesizer& MidiSystem::default_synthesizer() {
        throw MidiException("SoftSynthesizer not yet wired");
    }
}
#elif defined(__APPLE__)
// CoreMIDI backend - not yet implemented; same stub fallback.
export namespace stdx::audio::midi {
    Vector<MidiDeviceInfo> MidiSystem::devices() {
        return {};
    }

    UniquePointer<MidiDevice> MidiSystem::open_device([[maybe_unused]] const MidiDeviceInfo& info) {
        throw MidiException("CoreMIDI backend not yet wired");
    }

    UniquePointer<MidiDevice> MidiSystem::create_virtual_endpoint([[maybe_unused]] const VirtualEndpointOptions& options) {
        throw MidiException("virtual MIDI endpoints are not available on this platform yet");
    }

    Synthesizer& MidiSystem::default_synthesizer() {
        throw MidiException("SoftSynthesizer not yet wired");
    }
}
#else
export namespace stdx::audio::midi {
    Vector<MidiDeviceInfo> MidiSystem::devices() {
        return {};
    }

    UniquePointer<MidiDevice> MidiSystem::open_device([[maybe_unused]] const MidiDeviceInfo& info) {
        throw MidiException("no MIDI backend wired in this build");
    }

    UniquePointer<MidiDevice> MidiSystem::create_virtual_endpoint([[maybe_unused]] const VirtualEndpointOptions& options) {
        throw MidiException("virtual MIDI endpoints are not available on this platform yet");
    }

    Synthesizer& MidiSystem::default_synthesizer() {
        throw MidiException("default synthesizer not wired; ship the SoftSynthesizer backend");
    }
}
#endif
