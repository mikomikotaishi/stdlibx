#pragma once

using stdx::collections::Vector;
using stdx::mem::Pointers;
using stdx::mem::UniquePointer;
using stdx::sync::Atomic;
using stdx::thread::Thread;
using stdx::time::Instant;
using stdx::time::Microseconds;
using stdx::time::SteadyClock;

/**
 * @namespace stdx::audio::midi
 * @brief Cross-platform soft sequencer - walks a Sequence's merged event
 * stream on a worker thread, sleeping between events according to the
 * tempo currently in effect, and dispatches non-meta events to whatever
 * Receiver is wired to its Transmitter.
 */
namespace stdx::audio::midi {
    /**
     * @class SoftSequencerTransmitter
     * @brief Trivial transmitter - holds one Receiver* pointer. The
     * sequencer worker uses it to look up the connected sink.
     * @extends Transmitter
     */
    class SoftSequencerTransmitter final: public Transmitter {
    private:
        Atomic<Receiver*> rx{nullptr};
    public:
        void set_receiver(Receiver* r) noexcept override {
            rx.store(r);
        }

        [[nodiscard]]
        Receiver* receiver() const noexcept override {
            return rx.load();
        }

        void close() noexcept override {
            rx.store(nullptr);
        }
    };

    /**
     * @class SoftSequencer
     * @brief Cross-platform Sequencer implementation. PPQ timing only;
     * SMPTE-divided sequences will play but the division-to-microseconds
     * math treats them as PPQ (TODO: real SMPTE support).
     * @extends Sequencer
     */
    class SoftSequencer final: public Sequencer {
    private:
        MidiDeviceInfo dev_info{};
        UniquePointer<Sequence> seq;
        SoftSequencerTransmitter tx;
        Thread worker;
        Atomic<bool> running{false};
        Atomic<bool> opened{false};
        Atomic<i64> tick_pos{0};
        /// Tempo in microseconds per quarter-note. 500'000 == 120 BPM.
        Atomic<u32> tempo_us_per_qn{500'000};
        Atomic<f32> factor{1.0f};
        Atomic<i64> loop_start{0};
        Atomic<i64> loop_end{-1};
        Atomic<i32> loop_count{0};

        void play_loop() noexcept {
            if (!seq) {
                running.store(false);
                return;
            }

            // Two timing modes:
            //   PPQ:    us_per_tick = tempo_us_per_qn / ppq.  Mutable -
            //           Meta 0x51 (Set Tempo) events change tempo mid-sequence.
            //   SMPTE:  us_per_tick = 1e6 / (frame_rate * ticks_per_frame).
            //           Fixed by the SMF header; tempo meta events are ignored
            //           because the clock is frame-locked, not music-locked.
            const bool is_smpte = (seq->timing_type() == TimingType::SMPTE);
            i32 ppq = 480;
            f64 smpte_us_per_tick = 0.0;
            if (is_smpte) {
                const f64 rate = seq->smpte_frame_rate();
                const u8 tpf = seq->smpte_ticks_per_frame();
                if (rate > 0.0 && tpf > 0) {
                    smpte_us_per_tick = 1.0e6 / (rate * static_cast<f64>(tpf));
                } else {
                    // Malformed SMPTE division - fall back to something
                    // plausible so we still play rather than spin.
                    smpte_us_per_tick = 1000.0;
                }
            } else {
                const i32 d = seq->division();
                ppq = (d > 0) ? d : 480;
            }

            // Snapshot a tick-sorted view of every track's events. Stable
            // sort: events on the same tick preserve their track order, so
            // program-change-then-note-on relations stay intact.
            struct EventRef {
                i64 tick;
                const MidiMessage* msg;
            };
            Vector<EventRef> merged;
            for (usize i = 0; i < seq->track_count(); ++i) {
                const Track& t = seq->track(i);
                for (usize j = 0; j < t.size(); ++j) {
                    merged.emplace_back(t[j].tick, t[j].message.get());
                }
            }
            stdx::ranges::stable_sort(
                merged,
                [](const EventRef& a, const EventRef& b) noexcept -> bool {
                    return a.tick < b.tick;
                }
            );

            // Wall-clock baseline: every event is dispatched at
            //     start_time + cumulative_us / factor
            // where cumulative_us accumulates the delta-us per event.
            // Re-reading factor each iteration lets set_tempo_factor() take
            // effect for subsequent events without rewriting the past.
            const Instant<SteadyClock> start_time = SteadyClock::now();
            i64 cumulative_us = 0;
            i64 last_tick = 0;
            u32 tempo = tempo_us_per_qn.load();

            for (usize i = 0; i < merged.size(); ++i) {
                if (!running.load()) {
                    break;
                }
                const EventRef& ev = merged[i];
                const i64 ticks = ev.tick - last_tick;
                if (ticks > 0) {
                    i64 us;
                    if (is_smpte) {
                        us = static_cast<i64>(
                            static_cast<f64>(ticks) * smpte_us_per_tick
                        );
                    } else {
                        us = static_cast<i64>(ticks) * tempo / ppq;
                    }
                    f32 f = factor.load();
                    if (f <= 0.0f) {
                        f = 1.0f;
                    }
                    cumulative_us += static_cast<i64>(static_cast<f64>(us) / f);
                    const Instant<SteadyClock> target = start_time + Microseconds{cumulative_us};
                    Thread::sleep_until(target);
                }
                last_tick = ev.tick;
                tick_pos.store(ev.tick);

                const MidiMessage* msg = ev.msg;
                if (const MetaMessage* meta = dynamic_cast<const MetaMessage*>(msg)) {
                    // Meta 0x51 = Set Tempo, payload is 3 bytes BE microseconds-per-qn.
                    // Raw bytes layout: [0xFF, type, len(VLQ)..., payload...]
                    // SMPTE-divided sequences have a frame-locked clock, so
                    // tempo events have no effect on event timing - we just
                    // record the new value for external observers (tempo_bpm()).
                    if (meta->type() == 0x51) {
                        const Span<const u8> b = meta->data();
                        if (b.size() >= 6) {
                            const u32 new_tempo = (static_cast<u32>(b[3]) << 16)
                                | (static_cast<u32>(b[4]) << 8)
                                | static_cast<u32>(b[5]);
                            tempo_us_per_qn.store(new_tempo);
                            if (!is_smpte) {
                                tempo = new_tempo;
                            }
                        }
                    }
                    // Other meta types are SMF-only; never forwarded to a wire Receiver.
                } else {
                    // ShortMessage or SysexMessage - forward to the wired Receiver.
                    Receiver* r = tx.receiver();
                    if (r) {
                        try {
                            r->send(*msg, 0);
                        } catch (...) {
                            // The receiver decides what to do with errors;
                            // the sequencer just keeps walking the timeline.
                        }
                    }
                }
            }
            running.store(false);
        }
    public:
        SoftSequencer() noexcept:
            dev_info{
                .id = "soft-sequencer",
                .name = "stdx soft sequencer",
                .backend = MidiBackend::SOFT,
                .is_input = false,
                .is_output = true, // emits via transmitter()
            } {}

        ~SoftSequencer() override {
            stop();
            close();
        }

        // MidiDevice
        [[nodiscard]]
        const MidiDeviceInfo& info() const noexcept override {
            return dev_info;
        }

        THROWS(MidiException)
        void open() override {
            opened.store(true);
        }

        void close() noexcept override {
            stop();
            opened.store(false);
        }

        [[nodiscard]]
        bool is_open() const noexcept override {
            return opened.load();
        }

        /**
         * @brief Sequencer has no Receiver - it produces events, not consumes.
         */
        [[nodiscard]]
        Receiver* receiver() override {
            return nullptr;
        }

        [[nodiscard]]
        Transmitter* transmitter() override {
            return &tx;
        }

        // Sequencer
        THROWS(InvalidMidiDataException)
        void set_sequence(UniquePointer<Sequence> s) override {
            if (!s) {
                throw InvalidMidiDataException("null sequence");
            }
            // Hot-swap mid-playback isn't supported here - the worker thread
            // is iterating a sequence-derived view.
            const bool was_running = running.exchange(false);
            if (was_running && worker.joinable()) {
                worker.join();
            }
            seq = Ops::move(s);
            tick_pos.store(0);
        }

        [[nodiscard]]
        const Sequence* sequence() const noexcept override {
            return seq.get();
        }

        THROWS(MidiException)
        void start() override {
            if (!seq) {
                throw MidiException("no sequence loaded");
            }
            if (running.exchange(true)) {
                return;
            }
            if (worker.joinable()) {
                worker.join();
            }
            worker = Thread([this] -> void {
                play_loop();
            });
        }

        void stop() noexcept override {
            if (!running.exchange(false)) {
                if (worker.joinable()) {
                    worker.join();
                }
                return;
            }
            if (worker.joinable()) {
                worker.join();
            }
        }

        [[nodiscard]]
        bool is_running() const noexcept override {
            return running.load();
        }

        void set_tempo_bpm(f64 bpm) noexcept override {
            if (bpm <= 0.0) {
                return;
            }
            tempo_us_per_qn.store(static_cast<u32>(60'000'000.0 / bpm));
        }

        [[nodiscard]]
        f64 tempo_bpm() const noexcept override {
            return 60'000'000.0 / static_cast<f64>(tempo_us_per_qn.load());
        }

        void set_tempo_factor(f32 f) noexcept override {
            factor.store(f);
        }

        [[nodiscard]]
        f32 tempo_factor() const noexcept override {
            return factor.load();
        }

        void set_tick_position(i64 tick) noexcept override {
            // Honored only when stopped; mid-playback seek would require
            // interrupting sleep_until, which is a larger change.
            if (!running.load()) {
                tick_pos.store(tick);
            }
        }

        [[nodiscard]]
        i64 tick_position() const noexcept override {
            return tick_pos.load();
        }

        void set_loop_start_point(i64 tick) noexcept override {
            loop_start.store(tick);
        }

        void set_loop_end_point(i64 tick) noexcept override {
            loop_end.store(tick);
        }

        void set_loop_count(i32 count) noexcept override {
            loop_count.store(count);
        }
    };
}
