#pragma once

using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::io::InputFileStream;
using stdx::io::OpenMode;
using stdx::io::OutputFileStream;
using stdx::io::StreamSize;
using stdx::mem::Pointers;
using stdx::mem::UniquePointer;

/**
 * @namespace stdx::audio::midi
 * @brief MIDI Clip Files (M2-116-U) - the MIDI 2.0 counterpart to a type-0
 * Standard MIDI File.
 *
 * A clip file is the 8-byte header "SMF2CLIP" followed by big-endian UMP
 * words, every message timed by the Delta Clockstamp before it:
 *
 *     SMF2CLIP
 *     [Set Profile On ...]          clip configuration header
 *     DCS(0) DCTPQ(ticks/quarter)
 *     [DCS config message ...]
 *     DCS Start of Clip             clip sequence data
 *     DCS event [event ...] ...
 *     DCS End of Clip
 *
 * A clip maps onto a one-track @ref Sequence with PPQ timing:
 *
 * - tick 0 is the Start of Clip;
 * - messages from the configuration header (program changes, Set Tempo,
 *   Set Profile On, ...) are placed at tick 0, in file order, ahead of the
 *   clip's own tick-0 events, so they reach a receiver first;
 * - the End of Clip becomes an end-of-track meta event at its tick, so the
 *   clip's length survives a round trip;
 * - the file-structure messages themselves (Delta Clockstamps, DCTPQ, the
 *   Null/NOOP that restarts a long gap, Start/End of Clip) are not stored.
 *
 * Setting up profiles that Set Profile On names takes a MIDI-CI exchange with
 * the receiver, which a file cannot do; that is left to the caller.
 */
export namespace stdx::audio::midi {
    /**
     * @class MidiClipFile
     * @brief Reads and writes MIDI Clip Files.
     */
    class MidiClipFile final {
    private:
        static constexpr u8 MAGIC[8] = {'S', 'M', 'F', '2', 'C', 'L', 'I', 'P'};
        static constexpr u8 META_END_OF_TRACK = 0x2F;
        static constexpr u8 META_SET_TEMPO = 0x51;

        static void put_word(Vector<u8>& out, u32 word) {
            out.push_back(static_cast<u8>((word >> 24) & 0xFF));
            out.push_back(static_cast<u8>((word >> 16) & 0xFF));
            out.push_back(static_cast<u8>((word >> 8) & 0xFF));
            out.push_back(static_cast<u8>(word & 0xFF));
        }

        static void put_packet(Vector<u8>& out, const UniversalMidiPacket& packet) {
            for (u8 i = 0; i < packet.word_count(); ++i) {
                put_word(out, packet.word(i));
            }
        }

        /**
         * @internal
         * @brief Emit the Delta Clockstamp(s) that advance the file's clock
         * by @p ticks. Gaps beyond 20 bits are bridged with DCS + Null
         * (NOOP) pairs, which restart the count, as the spec requires.
         */
        static void advance(Vector<u8>& out, i64 ticks) {
            while (ticks > static_cast<i64>(UtilityMessage::MAX_DELTA_TICKS)) {
                put_packet(out, UtilityMessage::delta_clockstamp(UtilityMessage::MAX_DELTA_TICKS).packet());
                put_packet(out, UtilityMessage::noop().packet());
                ticks -= UtilityMessage::MAX_DELTA_TICKS;
            }
            put_packet(out, UtilityMessage::delta_clockstamp(static_cast<u32>(ticks)).packet());
        }

        /**
         * @internal
         * @brief The UMP packets for one sequence event, or none.
         *
         * Meta events have no wire form except Set Tempo, which becomes Flex
         * Data; end-of-track is expressed by End of Clip instead. Other meta
         * events (text, time and key signature, ...) are not yet converted.
         */
        [[nodiscard]]
        static Vector<UniversalMidiPacket> packets_for(const MidiMessage& msg, UmpEncoder& encoder) {
            if (const MetaMessage* meta = dynamic_cast<const MetaMessage*>(&msg)) {
                Vector<UniversalMidiPacket> out;
                const Span<const u8> b = meta->data();
                if (meta->type() == META_SET_TEMPO && b.size() >= 6) {
                    const u32 us = (static_cast<u32>(b[3]) << 16) | (static_cast<u32>(b[4]) << 8) | b[5];
                    out.push_back(FlexDataMessage::set_tempo(encoder.group(), us * 100).packet());
                }
                return out;
            }
            return encoder.encode(msg);
        }
    public:
        MidiClipFile() = DELETE_METHOD("MidiClipFile is a static utility class and cannot be instantiated.");

        /**
         * @brief Whether @p bytes start with the "SMF2CLIP" file header.
         */
        [[nodiscard]]
        static bool is_clip(Span<const u8> bytes) noexcept {
            if (bytes.size() < sizeof(MAGIC)) {
                return false;
            }
            for (usize i = 0; i < sizeof(MAGIC); ++i) {
                if (bytes[i] != MAGIC[i]) {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief Parse an in-memory MIDI Clip File into a one-track Sequence.
         * @throws InvalidMidiDataException if the header is missing, the data
         * is not whole UMP words or ends mid-packet, or there is no DCTPQ.
         */
        [[nodiscard]]
        THROWS(InvalidMidiDataException)
        static UniquePointer<Sequence> parse(Span<const u8> bytes) {
            if (!is_clip(bytes)) {
                throw InvalidMidiDataException("clip: missing SMF2CLIP header");
            }
            if ((bytes.size() - sizeof(MAGIC)) % 4 != 0) {
                throw InvalidMidiDataException("clip: data is not whole UMP words");
            }
            Vector<u32> words;
            words.reserve((bytes.size() - sizeof(MAGIC)) / 4);
            for (usize i = sizeof(MAGIC); i < bytes.size(); i += 4) {
                words.push_back(UniversalMidiPacket::word_from_be(bytes.data() + i));
            }

            Optional<u16> ticks_per_quarter;
            Vector<MidiEvent> events;
            i64 tick = 0;
            bool in_clip = false;
            bool ended = false;

            UmpParser parser;
            parser.feed_owned(
                Span<const u32>{words.data(), words.size()},
                [&](UniquePointer<MidiMessage> msg) -> void {
                    if (ended) {
                        return; // nothing may follow End of Clip; ignore it if it does
                    }
                    if (const UtilityMessage* utility = dynamic_cast<const UtilityMessage*>(msg.get())) {
                        switch (utility->utility_status()) {
                            case UtilityStatus::DELTA_CLOCKSTAMP:
                                tick += utility->delta_ticks();
                                break;
                            case UtilityStatus::DELTA_CLOCKSTAMP_TPQ:
                                if (!ticks_per_quarter.has_value() && utility->value() != 0) {
                                    ticks_per_quarter = utility->value();
                                }
                                break;
                            default:
                                break; // Null/NOOP, jitter reduction: timing only
                        }
                        return;
                    }
                    if (const UmpStreamMessage* stream = dynamic_cast<const UmpStreamMessage*>(msg.get())) {
                        if (stream->stream_status() == StreamStatus::START_OF_CLIP) {
                            in_clip = true;
                            tick = 0; // clip time starts here
                            return;
                        }
                        if (stream->stream_status() == StreamStatus::END_OF_CLIP) {
                            ended = true;
                            events.push_back(MidiEvent{
                                .tick = tick,
                                .message = Pointers::unique<MetaMessage>(META_END_OF_TRACK, Span<const u8>{}),
                            });
                            return;
                        }
                    }
                    events.push_back(MidiEvent{.tick = in_clip ? tick : 0, .message = Ops::move(msg)});
                }
            );

            if (parser.has_partial_packet()) {
                throw InvalidMidiDataException("clip: data ends mid-packet");
            }
            if (!ticks_per_quarter.has_value()) {
                throw InvalidMidiDataException("clip: missing Delta Clockstamp Ticks Per Quarter Note");
            }

            UniquePointer<Sequence> seq = Pointers::unique<Sequence>(TimingType::PPQ, static_cast<i32>(*ticks_per_quarter));
            Track& track = seq->create_track();
            for (MidiEvent& event: events) {
                track.add(Ops::move(event));
            }
            return seq;
        }

        /**
         * @brief Read a MIDI Clip File (.midi2) into a one-track Sequence.
         * @throws InvalidMidiDataException if the file cannot be read or is malformed.
         */
        [[nodiscard]]
        THROWS(InvalidMidiDataException)
        static UniquePointer<Sequence> read(const Path& path) {
            InputFileStream f{path, OpenMode::BINARY};
            if (!f) {
                throw InvalidMidiDataException("clip: cannot open file");
            }
            Vector<u8> bytes;
            char buffer[4096];
            while (f.read(buffer, sizeof(buffer)) || f.gcount() > 0) {
                const usize got = static_cast<usize>(f.gcount());
                bytes.insert(bytes.end(), reinterpret_cast<const u8*>(buffer), reinterpret_cast<const u8*>(buffer) + got);
                if (got < sizeof(buffer)) {
                    break;
                }
            }
            return parse(Span<const u8>{bytes.data(), bytes.size()});
        }

        /**
         * @brief Serialize @p seq as a MIDI Clip File.
         *
         * All tracks are merged into the clip's single timeline, stably by
         * tick, so same-tick events keep their track order. UMP events are
         * written as they are; MIDI 1.0 events are converted by a
         * @ref UmpEncoder for @p protocol on @p group - MIDI1 (the default)
         * keeps them bit-exact, MIDI2 widens them. Set Tempo meta events
         * become Flex Data Set Tempo; the End of Clip goes at the sequence's
         * last tick, so a trailing end-of-track meta sets the clip length.
         *
         * @throws InvalidMidiDataException if @p seq is not PPQ-timed with a
         * division of 1-65535, or holds a malformed MIDI 1.0 message.
         */
        [[nodiscard]]
        THROWS(InvalidMidiDataException)
        static Vector<u8> write(const Sequence& seq, UmpProtocol protocol = UmpProtocol::MIDI1, u8 group = 0) {
            if (seq.type() != TimingType::PPQ) {
                throw InvalidMidiDataException("clip: only PPQ-timed sequences can be written");
            }
            if (seq.division() <= 0 || seq.division() > 0xFFFF) {
                throw InvalidMidiDataException("clip: ticks per quarter note out of range");
            }

            struct EventRef {
                i64 tick;
                const MidiMessage* msg;
            };
            Vector<EventRef> merged;
            for (usize t = 0; t < seq.track_count(); ++t) {
                for (const MidiEvent& event: seq.track(t)) {
                    merged.push_back(EventRef{event.tick, event.message.get()});
                }
            }
            stdx::ranges::stable_sort(merged, [](const EventRef& a, const EventRef& b) noexcept -> bool {
                return a.tick < b.tick;
            });

            UmpEncoder encoder{protocol, group};
            Vector<u8> out(MAGIC, MAGIC + sizeof(MAGIC));

            // Clip configuration header: just the timing resolution.
            advance(out, 0);
            put_packet(out, UtilityMessage::delta_clockstamp_tpq(static_cast<u16>(seq.division())).packet());

            // Clip sequence data.
            advance(out, 0);
            put_packet(out, UmpStreamMessage::start_of_clip().packet());
            i64 now = 0;
            for (const EventRef& event: merged) {
                const Vector<UniversalMidiPacket> packets = packets_for(*event.msg, encoder);
                if (packets.empty()) {
                    continue;
                }
                const i64 at = event.tick < 0 ? 0 : event.tick;
                if (at != now) {
                    advance(out, at - now); // same-tick events share one clockstamp
                    now = at;
                }
                for (const UniversalMidiPacket& packet: packets) {
                    put_packet(out, packet);
                }
            }
            const i64 end = seq.last_tick() > now ? seq.last_tick() : now;
            advance(out, end - now);
            put_packet(out, UmpStreamMessage::end_of_clip().packet());
            return out;
        }

        /**
         * @brief Write @p seq to @p path as a MIDI Clip File. @see write
         * @throws InvalidMidiDataException as for write, or if the file
         * cannot be written.
         */
        THROWS(InvalidMidiDataException)
        static void save(const Sequence& seq, const Path& path, UmpProtocol protocol = UmpProtocol::MIDI1, u8 group = 0) {
            const Vector<u8> bytes = write(seq, protocol, group);
            OutputFileStream f{path, OpenMode::OUT | OpenMode::BINARY};
            if (!f) {
                throw InvalidMidiDataException("clip: cannot open file for writing");
            }
            f.write(reinterpret_cast<const char*>(bytes.data()), static_cast<StreamSize>(bytes.size()));
            if (!f) {
                throw InvalidMidiDataException("clip: write failed");
            }
        }
    };
}
