#pragma once

/**
 * @namespace stdx::audio::sampled
 * @brief Callback-driven audio line abstractions.
 */
export namespace stdx::audio::sampled {
    /**
     * @brief Render callback invoked by the backend each time more output
     *        frames are needed. Fill @p out with interleaved float samples.
     *
     * The callback runs on the backend's audio thread (CoreAudio HAL thread /
     * WASAPI event thread / ALSA poll thread). Do not block, allocate, or
     * take long locks here.
     */
    using RenderCallback = Function<void(Span<f32> out, AudioTime time)>;

    /**
     * @brief Capture callback. @p in contains the most recently captured
     *        interleaved float frames.
     */
    using CaptureCallback = Function<void(Span<const f32> in, AudioTime time)>;

    /**
     * @interface OutputLine
     * @brief Abstract handle to an opened output line.
     */
    class OutputLine {
    public:
        OutputLine() = default;
        OutputLine(const OutputLine&) = delete;
        OutputLine& operator=(const OutputLine&) = delete;
        virtual ~OutputLine() = default;

        /**
         * @brief Begin invoking the render callback. Has no effect if already started.
         */
        virtual void start() throws (LineUnavailableException) = 0;

        /**
         * @brief Stop invoking the render callback. Buffer is drained.
         */
        virtual void stop() throws (LineUnavailableException) = 0;

        /**
         * @brief Stop and release device resources. Idempotent.
         */
        virtual void close() noexcept = 0;

        /**
         * @brief The negotiated format actually in use (may differ from requested).
         */
        [[nodiscard]]
        virtual const AudioFormat& format() const noexcept = 0;

        /**
         * @brief Approximate one-way latency in frames (input-callback to speaker).
         */
        [[nodiscard]]
        virtual u32 latency_frames() const noexcept = 0;

        /**
         * @brief Number of buffer underruns observed since open.
         */
        [[nodiscard]]
        virtual u64 underrun_count() const noexcept = 0;

        /**
         * @brief True if start() has been called and stop()/close() has not.
         */
        [[nodiscard]]
        virtual bool is_running() const noexcept = 0;
    };

    /**
     * @interface InputLine
     * @brief Mirror of @ref OutputLine for the capture side.
     */
    class InputLine {
    public:
        InputLine() = default;
        InputLine(const InputLine&) = delete;
        InputLine& operator=(const InputLine&) = delete;
        virtual ~InputLine() = default;

        virtual void start() throws (LineUnavailableException) = 0;
        virtual void stop() throws (LineUnavailableException) = 0;
        virtual void close() noexcept = 0;

        [[nodiscard]]
        virtual const AudioFormat& format() const noexcept = 0;

        [[nodiscard]]
        virtual u32 latency_frames() const noexcept = 0;

        [[nodiscard]]
        virtual u64 overrun_count() const noexcept = 0;

        [[nodiscard]]
        virtual bool is_running() const noexcept = 0;
    };
}
