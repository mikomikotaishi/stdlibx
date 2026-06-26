#pragma once

using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::io::InputFileStream;
using stdx::io::OpenMode;
using stdx::io::SeekingDirection;
using stdx::io::StreamOffset;
using stdx::io::StreamPosition;
using stdx::io::StreamSize;
using stdx::mem::Pointers;
using stdx::mem::UniquePointer;

/**
 * @namespace stdx::audio::midi
 * @brief Standard MIDI File (SMF) decoder.
 *
 * Parses format-0 and format-1 SMF files (the only ones any tool actually
 * emits) into a stdx::audio::midi::Sequence. Format-2 files (independent
 * sequential patterns) are read as a multi-track Sequence and the caller
 * gets to interpret them - no automatic concatenation.
 */
export namespace stdx::audio::midi {
    [[nodiscard]]
    inline u16 read_u16_be(const u8* p) noexcept {
        return static_cast<u16>(
            (static_cast<u16>(p[0]) << 8) | static_cast<u16>(p[1])
        );
    }

    [[nodiscard]]
    inline u32 read_u32_be(const u8* p) noexcept {
        return (static_cast<u32>(p[0]) << 24)
            | (static_cast<u32>(p[1]) << 16)
            | (static_cast<u32>(p[2]) << 8)
            | static_cast<u32>(p[3]);
    }

    [[nodiscard]]
    inline i16 read_i16_be(const u8* p) noexcept {
        return static_cast<i16>(read_u16_be(p));
    }

    [[nodiscard]]
    inline bool smf_tag_equals(const u8* tag, const char (&lit)[5]) noexcept {
        return tag[0] == static_cast<u8>(lit[0])
            && tag[1] == static_cast<u8>(lit[1])
            && tag[2] == static_cast<u8>(lit[2])
            && tag[3] == static_cast<u8>(lit[3]);
    }

    /**
     * @brief Read a Variable-Length Quantity from @p f. 7 bits per byte,
     * MSB set on every byte except the last. SMF caps VLQs at 4 bytes.
     */
    [[nodiscard]]
    inline u32 read_vlq(InputFileStream& f) noexcept {
        u32 v = 0;
        for (i32 i = 0; i < 4; ++i) {
            const i32 c = f.get();
            if (c < 0) {
                return v;
            }
            const u8 b = static_cast<u8>(c);
            v = (v << 7) | static_cast<u32>(b & 0x7F);
            if ((b & 0x80) == 0) {
                return v;
            }
        }
        return v;
    }

    /**
     * @brief Parse one MTrk chunk into @p track. Caller has already consumed
     * the "MTrk" + chunk-size header; @p chunk_size is the body length.
     * @param f Input stream positioned at the start of the MTrk body.
     * @param chunk_size Size of the MTrk body in bytes.
     * @param track Track to populate with events.
     * @throws InvalidMidiDataException on truncated events or invalid running
     * status.
     */
    THROWS(InvalidMidiDataException)
    inline void parse_track(InputFileStream& f, u32 chunk_size, Track& track) {
        const StreamPosition start = f.tellg();
        const StreamPosition end = start + static_cast<StreamOffset>(chunk_size);

        i64 abs_tick = 0;
        u8 running_status = 0;

        while (f.tellg() < end && f.good()) {
            const u32 delta = read_vlq(f);
            abs_tick += static_cast<i64>(delta);

            const i32 peek_c = f.peek();
            if (peek_c < 0) {
                throw InvalidMidiDataException("SMF: unexpected EOF in track");
            }
            u8 status = static_cast<u8>(peek_c);
            if (status < 0x80) {
                // Data byte where status expected - running status applies.
                if (running_status == 0) {
                    throw InvalidMidiDataException(
                        "SMF: data byte with no running status"
                    );
                }
                status = running_status;
            } else {
                f.get(); // consume the status byte
                if (status < 0xF0) {
                    // Only channel-voice messages set running status.
                    running_status = status;
                }
            }

            if (status == 0xFF) {
                // Meta event: type byte + VLQ length + payload.
                const i32 type_c = f.get();
                if (type_c < 0) {
                    throw InvalidMidiDataException("SMF: truncated meta event");
                }
                const u8 type = static_cast<u8>(type_c);
                const u32 len = read_vlq(f);
                Vector<u8> payload(len);
                if (len > 0) {
                    f.read(
                        reinterpret_cast<char*>(payload.data()),
                        static_cast<StreamSize>(len)
                    );
                    if (static_cast<u32>(f.gcount()) != len) {
                        throw InvalidMidiDataException(
                            "SMF: truncated meta payload"
                        );
                    }
                }
                UniquePointer<MidiMessage> msg = Pointers::unique<MetaMessage>(
                    type, Span<const u8>{payload.data(), payload.size()}
                );
                track.add(MidiEvent{abs_tick, Ops::move(msg)});
                if (type == 0x2F) {
                    // End-of-track meta - done with this MTrk regardless of
                    // any padding bytes the writer may have left.
                    break;
                }
            } else if (status == 0xF0 || status == 0xF7) {
                // SysEx (0xF0) or escape (0xF7): VLQ length + raw bytes.
                const u32 len = read_vlq(f);
                Vector<u8> raw;
                raw.reserve(static_cast<usize>(len) + 1);
                if (status == 0xF0) {
                    raw.push_back(0xF0);
                }
                const usize before = raw.size();
                raw.resize(before + len);
                if (len > 0) {
                    f.read(
                        reinterpret_cast<char*>(raw.data() + before),
                        static_cast<StreamSize>(len)
                    );
                    if (static_cast<u32>(f.gcount()) != len) {
                        throw InvalidMidiDataException(
                            "SMF: truncated sysex payload"
                        );
                    }
                }
                UniquePointer<MidiMessage> msg = Pointers::unique<SysexMessage>(
                    Span<const u8>{raw.data(), raw.size()}
                );
                track.add(MidiEvent{abs_tick, Ops::move(msg)});
                running_status = 0;
            } else {
                // Channel-voice message. C0/D0 have 1 data byte; the rest take 2.
                const u8 cmd = static_cast<u8>(status & 0xF0);
                const u8 channel = static_cast<u8>(status & 0x0F);
                const bool two_byte_only = (cmd == 0xC0) || (cmd == 0xD0);
                const i32 d1c = f.get();
                if (d1c < 0) {
                    throw InvalidMidiDataException("SMF: truncated channel msg");
                }
                u8 d1 = static_cast<u8>(d1c);
                u8 d2 = 0;
                if (!two_byte_only) {
                    const i32 d2c = f.get();
                    if (d2c < 0) {
                        throw InvalidMidiDataException(
                            "SMF: truncated channel msg"
                        );
                    }
                    d2 = static_cast<u8>(d2c);
                }
                UniquePointer<MidiMessage> msg = Pointers::unique<ShortMessage>(
                    static_cast<Status>(cmd), channel, d1, d2
                );
                track.add(MidiEvent{abs_tick, Ops::move(msg)});
            }
        }

        // Skip to the end of the chunk if the parser stopped early on EOT.
        if (f.tellg() < end) {
            f.seekg(end);
        }
    }

    /**
     * @brief Parse a Standard MIDI File at @p path into a Sequence.
     * @throws InvalidMidiDataException on malformed header, unknown format,
     * truncated tracks, or invalid running status.
     */
    [[nodiscard]]
    THROWS(InvalidMidiDataException)
    inline UniquePointer<Sequence> parse_smf(const Path& path) {
        InputFileStream f{path, OpenMode::BINARY};
        if (!f) {
            throw InvalidMidiDataException("SMF: cannot open file");
        }

        u8 mthd[14];
        f.read(reinterpret_cast<char*>(mthd), sizeof(mthd));
        if (f.gcount() != sizeof(mthd) || !smf_tag_equals(mthd, "MThd")) {
            throw InvalidMidiDataException("SMF: missing MThd header");
        }
        const u32 hdr_size = read_u32_be(mthd + 4);
        if (hdr_size < 6) {
            throw InvalidMidiDataException("SMF: MThd chunk too small");
        }
        const u16 smf_format = read_u16_be(mthd + 8);
        const u16 ntrks = read_u16_be(mthd + 10);
        const i16 division_raw = read_i16_be(mthd + 12);

        if (smf_format > 2) {
            throw InvalidMidiDataException("SMF: unknown format");
        }
        // Skip any extra MThd bytes past the 6 we consumed.
        if (hdr_size > 6) {
            f.seekg(static_cast<StreamOffset>(hdr_size - 6), SeekingDirection::CURRENT);
        }

        // Division decode: top bit clear = PPQ (positive ticks-per-quarter),
        // top bit set = SMPTE (negative SMPTE format byte + ticks-per-frame).
        TimingType timing = TimingType::PPQ;
        i32 division_val = static_cast<i32>(division_raw);
        if (division_raw < 0) {
            timing = TimingType::SMPTE;
        }

        UniquePointer<Sequence> seq = Pointers::unique<Sequence>(timing, division_val);

        for (u16 ti = 0; ti < ntrks; ++ti) {
            u8 mtrk[8];
            f.read(reinterpret_cast<char*>(mtrk), sizeof(mtrk));
            if (f.gcount() != sizeof(mtrk)) {
                throw InvalidMidiDataException("SMF: truncated before MTrk");
            }
            if (!smf_tag_equals(mtrk, "MTrk")) {
                // Unknown chunk - skip per spec.
                const u32 size = read_u32_be(mtrk + 4);
                f.seekg(static_cast<StreamOffset>(size), SeekingDirection::CURRENT);
                --ti; // don't count this skipped chunk against ntrks
                continue;
            }
            const u32 size = read_u32_be(mtrk + 4);
            Track& track = seq->create_track();
            parse_track(f, size, track);
        }

        return seq;
    }
}
