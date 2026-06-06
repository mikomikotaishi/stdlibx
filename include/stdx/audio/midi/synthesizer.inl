#pragma once

/**
 * @namespace stdx::audio::midi
 * @brief Synthesizer interface.
 *
 * Concrete soft-synth implementation lives in a backend .inl that bridges
 * to stdx::audio::sampled::OutputLine for rendering.
 */
export namespace stdx::audio::midi {
    /**
     * @interface Synthesizer
     * @brief Abstract synthesizer. Inherits MidiDevice so the rest of the
     * MIDI graph (Sequencer, Receiver routing) can target it uniformly.
     * @extends MidiDevice
     */
    class Synthesizer: public MidiDevice {
    public:
        ~Synthesizer() override = default;

        /**
         * @brief Replace the active soundbank. Voices keyed to the old bank stop.
         * @param bank The new soundbank to load.
         * @throws MidiException if the soundbank fails to load.
         */
        virtual void load_soundbank(const SoundBank& bank) throws (MidiException) = 0;

        /**
         * @brief Returns the highest concurrent voice count the synth was configured for.
         * @return The maximum polyphony.
         */
        [[nodiscard]]
        virtual u32 max_polyphony() const noexcept = 0;

        /**
         * @brief Returns the current number of voices producing sound.
         * @return The voice count.
         */
        [[nodiscard]]
        virtual u32 voice_count() const noexcept = 0;

        /// Master gain in linear units (1.0 = unity). Implementations may
        /// clamp to a reasonable range.
        virtual void set_gain(f32 gain) noexcept = 0;
        [[nodiscard]]

        virtual f32 gain() const noexcept = 0;

        /// All voices off - equivalent to sending MIDI All-Sound-Off CC120
        /// on every channel.
        virtual void all_sound_off() noexcept = 0;

        /// Reset every channel's controllers to defaults.
        virtual void reset_all_controllers() noexcept = 0;
    };
}
