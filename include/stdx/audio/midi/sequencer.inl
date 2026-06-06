#pragma once

/**
 * @namespace stdx::audio::midi
 * @brief Sequencer - plays back a Sequence into a connected Receiver.
 */
export namespace stdx::audio::midi {
    /**
     * @interface Sequencer
     * @brief Abstract sequencer. Concrete platforms (or the cross-platform
     * software sequencer) provide a real implementation.
     * @extends MidiDevice
     *
     * A sequencer is itself a @ref MidiDevice - its @c transmitter() emits
     * events from the loaded Sequence to whatever Receiver is connected
     * (typically a Synthesizer's receiver).
     */
    class Sequencer: public MidiDevice {
    public:
        ~Sequencer() override = default;

        /**
         * @brief Replaces the current Sequence. Allowed while stopped or running.
         * @param seq The new sequence to play. Must not be null.
         * @throws InvalidMidiDataException if the sequence is malformed.
         */
        virtual void set_sequence(UniquePointer<Sequence> seq) throws (InvalidMidiDataException) = 0;

        [[nodiscard]]
        virtual const Sequence* sequence() const noexcept = 0;

        virtual void start() throws (MidiException) = 0;
        virtual void stop() noexcept = 0;
        [[nodiscard]]
        virtual bool is_running() const noexcept = 0;

        virtual void set_tempo_bpm(f64 bpm) noexcept = 0;
        [[nodiscard]]
        virtual f64 tempo_bpm() const noexcept = 0;

        /**
         * @brief Set a tempo multiplier.
         * @param factor Tempo multiplier, where 1.0 = normal speed.
         * Negative values play backwards if the backend supports it;
         * otherwise treated as 0 (stopped).
         */
        virtual void set_tempo_factor(f32 factor) noexcept = 0;
        [[nodiscard]]
        virtual f32 tempo_factor() const noexcept = 0;

        virtual void set_tick_position(i64 tick) noexcept = 0;
        [[nodiscard]]
        virtual i64 tick_position() const noexcept = 0;

        virtual void set_loop_start_point(i64 tick) noexcept = 0;

        /**
         * @brief Set the loop end point. If the sequencer is running
         * and the current tick position is past the new end point, the
         * position is moved back to the loop start point (or 0 if the start
         * point is past the new end point).
         * 
         * @param tick Loop end point in ticks, or -1 to loop to the end of the sequence.
         */
        virtual void set_loop_end_point(i64 tick) noexcept = 0;

        /**
         * @brief Set the loop count. If the sequencer is running and the current loop
         * count is past the new count, the position is moved back to the loop
         * start point (or 0 if the start point is past the current position).
         * 
         * @param count Number of times to repeat the loop, or -1 for infinite looping.
         */
        virtual void set_loop_count(i32 count) noexcept = 0;
    };
}
