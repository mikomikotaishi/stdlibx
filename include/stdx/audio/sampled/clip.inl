#pragma once

using stdx::mem::UniquePointer;
using stdx::sync::Atomic;

/**
 * @namespace stdx::audio::sampled
 * @brief Clip - pull from an AudioInputStream into an OutputLine, end-of-file
 * detection included.
 */
export namespace stdx::audio::sampled {
    /**
     * @class Clip
     * @brief Plays an AudioInputStream through an OutputLine until the stream
     * exhausts.
     *
     * Clip owns the decoder, the output line, and the end-of-stream flag. The
     * render callback captures @c this; member-destruction order (line before
     * stream) guarantees the callback can't fire against a freed stream.
     */
    class Clip final {
    private:
        UniquePointer<AudioInputStream> _stream;
        UniquePointer<OutputLine> _line;
        Atomic<bool> _finished{false};
        Atomic<bool> _playing{false};
    public:
        explicit Clip(UniquePointer<AudioInputStream> stream) noexcept:
            _stream{Ops::move(stream)} {}

        ~Clip() noexcept {
            close();
        }

        Clip(const Clip&) = DELETE_METHOD("Clip is not copyable.");
        Clip& operator=(const Clip&) = DELETE_METHOD("Clip is not copyable.");

        /**
         * @brief Open the default output at the file's native format and start
         * playback. Returns once the line's render thread is running.
         * @throws UnsupportedAudioFormatException if the file's format isn't
         * supported by the system's default output.
         * @throws LineUnavailableException if the line can't be opened
         * (e.g. no free device, or the device doesn't support the file's format).
         */
        THROWS(UnsupportedAudioFormatException, LineUnavailableException)
        void play() {
            if (_playing.load() || !_stream) {
                return;
            }

            const AudioFormat fmt = _stream->format();
            // The lambda captures @c this; safe because line is destroyed
            // before stream/finished by reverse-declaration-order teardown,
            // and the OutputLine destructor joins its worker.
            RenderCallback cb = [this](Span<f32> out, AudioTime) -> void {
                if (_finished.load()) {
                    for (f32& s: out) {
                        s = 0.0f;
                    }
                    return;
                }
                const usize channels = _stream->format().channels;
                const usize frames_filled = _stream->read(out);
                const usize samples_filled = frames_filled * channels;
                // Pad the trailing portion with silence on a short read (EOF
                // mid-buffer) so the device doesn't replay stale frames.
                for (usize i = samples_filled; i < out.size(); ++i) {
                    out[i] = 0.0f;
                }
                if (frames_filled == 0) {
                    _finished.store(true);
                }
            };

            _line = AudioSystem::open_default_output(fmt, Ops::move(cb));
            _line->start();
            _playing.store(true);
        }

        /**
         * @brief Stop the underlying line. Idempotent. Does not destroy the
         *        line - call close() to release device resources.
         */
        void stop() noexcept {
            if (!_playing.exchange(false)) {
                return;
            }
            if (_line) {
                try {
                    _line->stop();
                } catch (...) {
                    // stop() throws LineUnavailableException; on close-path
                    // there's nothing useful we can do, so swallow it.
                }
            }
            _finished.store(true);
        }

        /**
         * @brief Stop and release the line. Idempotent.
         */
        void close() noexcept {
            stop();
            if (_line) {
                _line->close();
                _line.reset();
            }
        }

        /**
         * @brief Block (poll-spin at 10 ms) until the stream signals EOF. Use
         * for short ad-hoc playbacks; for long files prefer driving the
         * check from your own event loop.
         */
        void wait_for_completion() noexcept {
            while (_playing.load() && !_finished.load()) {
                Thread::sleep_for(10ms);
            }
        }

        [[nodiscard]]
        bool is_finished() const noexcept {
            return _finished.load();
        }

        [[nodiscard]]
        bool is_playing() const noexcept {
            return _playing.load() && !_finished.load();
        }

        [[nodiscard]]
        const AudioFormat& format() const noexcept {
            return _stream->format();
        }

        [[nodiscard]]
        u64 total_frames() const noexcept {
            return _stream ? _stream->total_frames() : 0;
        }
    };
}
