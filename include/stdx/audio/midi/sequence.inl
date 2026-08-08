#pragma once

using stdx::collections::Vector;
using stdx::mem::Pointers;
using stdx::mem::UniquePointer;

/**
 * @namespace stdx::audio::midi
 * @brief MIDI sequences
 */
export namespace stdx::audio::midi {
    /**
     * @enum TimingType
     * @brief Time-division encoding for a @ref Sequence.
     * PPQ values are positive; SMPTE division values are negative and
     * encode frame rate + ticks-per-frame (per the Standard MIDI File
     * specification).
     */
    enum class TimingType: u8 {
        PPQ, ///< Pulses per quarter note (positive division value)
        SMPTE, ///< SMPTE frames per second + ticks per frame
    };

    /**
     * @struct MidiEvent
     * @brief A timestamped MIDI event inside a @ref Track.
     */
    struct MidiEvent {
        i64 tick; ///< Position in the sequence's time grid.
        UniquePointer<MidiMessage> message; ///< Owned message.

        MidiEvent() = default;

        MidiEvent(i64 t, UniquePointer<MidiMessage> m):
            tick{t}, message{Ops::move(m)} {}
    };

    /**
     * @class Track
     * @brief Ordered (by tick) list of MidiEvent. A Track owns its events.
     */
    class Track final {
    private:
        Vector<MidiEvent> _events;
    public:
        Track() = default;

        [[nodiscard]]
        usize size() const noexcept {
            return _events.size();
        }

        [[nodiscard]]
        bool empty() const noexcept {
            return _events.empty();
        }

        [[nodiscard]]
        auto& at(this auto&& self, usize i) {
            return self._events.at(i);
        }

        [[nodiscard]]
        auto& operator[](this auto&& self, usize i) {
            return self._events[i];
        }

        /**
         * @brief Inserts the event in tick-order. Stable for equal ticks.
         * @param e Event to insert. The track takes ownership of the message.
         */
        void add(MidiEvent e) {
            auto pos = stdx::ranges::upper_bound(_events, e.tick, Less<>(), &MidiEvent::tick);
            _events.insert(pos, Ops::move(e));
        }

        void remove(usize i) {
            if (i < _events.size()) {
                _events.erase(_events.begin() + i);
            }
        }

        [[nodiscard]]
        i64 last_tick() const noexcept {
            return _events.empty() ? 0 : _events.back().tick;
        }

        [[nodiscard]]
        auto begin() const noexcept {
            return _events.begin();
        }

        [[nodiscard]]
        auto end() const noexcept {
            return _events.end();
        }
    };

    /**
     * @brief A sequence of tracks plus a timing-division descriptor.
     */
    class Sequence final {
    private:
        TimingType _type;
        i32 _division; ///< PPQ value, or encoded SMPTE division.
        Vector<UniquePointer<Track>> _tracks;
    public:
        explicit Sequence(TimingType type = TimingType::PPQ, i32 division = 480):
            _type{type}, _division{division} {}

        [[nodiscard]]
        TimingType type() const noexcept {
            return _type;
        }

        [[nodiscard]]
        i32 division() const noexcept {
            return _division;
        }

        /**
         * @brief For SMPTE-divided sequences: the negative-frame-rate format
         * byte from the SMF header, returned as a positive integer.
         *
         * Standard values: 24 (film), 25 (PAL), 29 (NTSC drop-frame, actually
         * 30000/1001 fps), 30 (NTSC non-drop). Returns 0 for PPQ sequences.
         */
        [[nodiscard]]
        i32 smpte_format() const noexcept {
            if (_type != TimingType::SMPTE) {
                return 0;
            }
            // The high byte of the i16 division is the negative frame format.
            return -static_cast<i32>(static_cast<i8>((_division >> 8) & 0xFF));
        }

        /**
         * @brief For SMPTE-divided sequences: ticks per video frame, encoded
         * in the low byte of the SMF division word. Returns 0 for PPQ.
         */
        [[nodiscard]]
        u8 smpte_ticks_per_frame() const noexcept {
            if (_type != TimingType::SMPTE) {
                return 0;
            }
            return static_cast<u8>(_division & 0xFF);
        }

        /**
         * @brief For SMPTE-divided sequences: the effective frame rate in
         * frames per second. Returns the nominal integer rate except for
         * format 29, which expands to 30000/1001 (NTSC drop-frame). Returns
         * 0 for PPQ.
         */
        [[nodiscard]]
        f64 smpte_frame_rate() const noexcept {
            const i32 f = smpte_format();
            if (f == 0) {
                return 0.0;
            }
            if (f == 29) {
                // NTSC drop-frame: 30000/1001 ≈ 29.97 frames/second.
                return 30000.0 / 1001.0;
            }
            return static_cast<f64>(f);
        }

        Track& create_track() {
            _tracks.push_back(Pointers::unique<Track>());
            return *_tracks.back();
        }

        bool delete_track(Track& t) {
            for (auto it = _tracks.begin(); it != _tracks.end(); ++it) {
                if (it->get() == &t) {
                    _tracks.erase(it);
                    return true;
                }
            }
            return false;
        }

        [[nodiscard]]
        usize track_count() const noexcept {
            return _tracks.size();
        }

        [[nodiscard]]
        auto& track(this auto&& self, usize i) {
            return *self._tracks.at(i);
        }

        [[nodiscard]]
        auto& operator[](this auto&& self, usize i) {
            return *self._tracks[i];
        }

        [[nodiscard]]
        i64 last_tick() const noexcept {
            i64 last = 0;
            for (const UniquePointer<Track>& t: _tracks) {
                last = Math::max(last, t->last_tick());
            }
            return last;
        }
    };
}
