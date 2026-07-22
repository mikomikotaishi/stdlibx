#pragma once

using stdx::fs::Path;
using stdx::io::InputFileStream;
using stdx::io::OpenMode;
using stdx::io::SeekingDirection;
using stdx::io::StreamOffset;
using stdx::io::StreamPosition;
using stdx::io::StreamSize;

/**
 * @namespace stdx::audio::sampled
 * @brief AIFF / AIFC audio file decoder.
 *
 * Apple's IFF-shaped equivalent of WAV. Same chunked layout, but everything
 * multi-byte is big-endian (FORM/AIFF came from Motorola hardware) and the
 * sample rate is stored as an 80-bit IEEE extended float. Sample payloads
 * are big-endian by default; AIFC adds compression-type tags including
 * "sowt" (Apple's little-endian PCM swap) and "fl32" (32-bit IEEE float).
 *
 * Supported: PCM s16/s24/s32 BE ("NONE"), PCM LE ("sowt"), IEEE float 32 BE
 * ("fl32"). Compressed types (alaw, ulaw, ima4, etc.) throw
 * UnsupportedAudioFileException.
 */
namespace stdx::audio::sampled {
    /// AIFC compression-type FourCCs we recognize.
    /// "NONE" - uncompressed big-endian signed PCM (default for AIFF).
    /// "sowt" - uncompressed little-endian signed PCM (Apple swap; same payload as WAV PCM).
    /// "fl32" / "FL32" - 32-bit IEEE float, big-endian.
    enum class AiffCodec: u8 {
        NONE,
        SOWT,
        FL32,
    };

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
    inline i32 read_i24_be(const u8* p) noexcept {
        i32 v = (static_cast<i32>(p[0]) << 16)
            | (static_cast<i32>(p[1]) << 8)
            | static_cast<i32>(p[2]);
        if (v & 0x800000) {
            v |= static_cast<i32>(0xFF000000);
        }
        return v;
    }

    [[nodiscard]]
    inline i32 read_i32_be(const u8* p) noexcept {
        return static_cast<i32>(read_u32_be(p));
    }

    [[nodiscard]]
    inline f32 read_f32_be(const u8* p) noexcept {
        const u32 bits = read_u32_be(p);
        f32 out;
        stdx::mem::memcpy(&out, &bits, sizeof(out));
        return out;
    }

    /**
     * @brief Decode a 10-byte IEEE 754 extended-precision float (big-endian)
     * into f64. Used for AIFF/AIFC sample rates. Handles normalized values
     * in the typical sample-rate range; zero is preserved exactly.
     */
    [[nodiscard]]
    inline f64 read_extended_be(const u8* p) noexcept {
        const u16 expon = static_cast<u16>(
            (static_cast<u16>(p[0] & 0x7F) << 8) | static_cast<u16>(p[1])
        );
        const u64 himant = (static_cast<u64>(p[2]) << 56)
            | (static_cast<u64>(p[3]) << 48)
            | (static_cast<u64>(p[4]) << 40)
            | (static_cast<u64>(p[5]) << 32)
            | (static_cast<u64>(p[6]) << 24)
            | (static_cast<u64>(p[7]) << 16)
            | (static_cast<u64>(p[8]) << 8)
            | static_cast<u64>(p[9]);
        if (expon == 0 && himant == 0) {
            return 0.0;
        }
        // value = sign * himant * 2^(expon - 16383 - 63)
        f64 v = static_cast<f64>(himant)
            * Math::ldexp(1.0, static_cast<i32>(expon) - 16383 - 63);
        if (p[0] & 0x80) {
            v = -v;
        }
        return v;
    }

    [[nodiscard]]
    inline bool aiff_tag_equals(const u8* tag, const char (&lit)[5]) noexcept {
        return tag[0] == static_cast<u8>(lit[0])
            && tag[1] == static_cast<u8>(lit[1])
            && tag[2] == static_cast<u8>(lit[2])
            && tag[3] == static_cast<u8>(lit[3]);
    }

    /**
     * @class AiffAudioInputStream
     * @brief AIFF/AIFC decoder. Handles PCM (16/24/32-bit) in either byte
     * order and 32-bit IEEE float; rejects compressed codecs.
     * @extends AudioInputStream
     */
    class AiffAudioInputStream final: public AudioInputStream {
    private:
        InputFileStream _file;
        Vector<u8> _raw_buf;
        AudioFormat _fmt = {};
        u64 _frame_count = 0;
        u64 _frames_remaining = 0;
        u32 _src_bytes_per_frame = 0; ///< channels * bytes_per_sample.
        u16 _src_bits_per_sample = 0;
        u16 _src_bytes_per_sample = 0; ///< bits / 8 - stride between consecutive samples.
        AiffCodec _codec = AiffCodec::NONE;

        void convert_to_f32(const u8* src, usize frames, Span<f32> out) const noexcept {
            const usize samples = frames * _fmt.channels;
            const u16 stride = _src_bytes_per_sample;
            if (_codec == AiffCodec::FL32) {
                for (usize i = 0; i < samples; ++i) {
                    out[i] = read_f32_be(src + i * stride);
                }
                return;
            }
            // codec is NONE (BE int) or SOWT (LE int). Branch once per codec
            // rather than per sample to keep the inner loop tight.
            const bool be = (_codec == AiffCodec::NONE);
            switch (_src_bits_per_sample) {
                case 16:
                    if (be) {
                        for (usize i = 0; i < samples; ++i) {
                            out[i] = static_cast<f32>(
                                read_i16_be(src + i * stride)) * INT16_NORM;
                        }
                    } else {
                        for (usize i = 0; i < samples; ++i) {
                            out[i] = static_cast<f32>(
                                read_i16_le(src + i * stride)) * INT16_NORM;
                        }
                    }
                    break;
                case 24:
                    if (be) {
                        for (usize i = 0; i < samples; ++i) {
                            out[i] = static_cast<f32>(read_i24_be(src + i * stride)) * INT24_NORM;
                        }
                    } else {
                        for (usize i = 0; i < samples; ++i) {
                            out[i] = static_cast<f32>(read_i24_le(src + i * stride)) * INT24_NORM;
                        }
                    }
                    break;
                case 32:
                    if (be) {
                        for (usize i = 0; i < samples; ++i) {
                            out[i] = static_cast<f32>(read_i32_be(src + i * stride)) * INT32_NORM;
                        }
                    } else {
                        for (usize i = 0; i < samples; ++i) {
                            out[i] = static_cast<f32>(read_i32_le(src + i * stride)) * INT32_NORM;
                        }
                    }
                    break;
                default:
                    Ops::unreachable();
            }
        }
    public:
        THROWS(UnsupportedAudioFileException)
        explicit AiffAudioInputStream(const Path& path) {
            _file.open(path, OpenMode::BINARY);
            if (!_file) {
                throw UnsupportedAudioFileException("cannot open AIFF file");
            }

            u8 header[12];
            _file.read(reinterpret_cast<char*>(header), sizeof(header));
            if (_file.gcount() != sizeof(header) || !aiff_tag_equals(header, "FORM")) {
                throw UnsupportedAudioFileException("not a FORM file");
            }
            const bool is_aifc = aiff_tag_equals(header + 8, "AIFC");
            const bool is_aiff = aiff_tag_equals(header + 8, "AIFF");
            if (!is_aifc && !is_aiff) {
                throw UnsupportedAudioFileException("FORM not AIFF or AIFC");
            }

            bool got_comm = false;
            u32 ssnd_data_offset = 0; ///< Absolute file offset of the first sample byte.
            u32 ssnd_data_size = 0;   ///< Bytes of sample data in the SSND chunk.

            // Walk chunks until we've seen both COMM and SSND. Chunks pad to
            // even byte boundary - skip the trailer byte when size is odd.
            u8 chunk_hdr[8];
            while (
                _file.read(reinterpret_cast<char*>(chunk_hdr), sizeof(chunk_hdr)),
                _file.gcount() == sizeof(chunk_hdr)
            ) {
                const u32 chunk_size = read_u32_be(chunk_hdr + 4);
                const u32 padded_size = chunk_size + (chunk_size & 1u);

                if (aiff_tag_equals(chunk_hdr, "COMM")) {
                    // COMM minimum: channels(2) + frames(4) + bits(2) + sampleRate(10) = 18.
                    // AIFC extends with compressionType(4) + Pascal-string name.
                    if (chunk_size < 18) {
                        throw UnsupportedAudioFileException("COMM chunk too small");
                    }
                    Vector<u8> comm_buf(chunk_size);
                    _file.read(reinterpret_cast<char*>(comm_buf.data()), chunk_size);
                    if (_file.gcount() != static_cast<StreamSize>(chunk_size)) {
                        throw UnsupportedAudioFileException("truncated COMM");
                    }
                    if (chunk_size & 1u) {
                        _file.seekg(1, SeekingDirection::CURRENT);
                    }

                    // COMM body layout:
                    //   [0..1]   numChannels       (i16 BE)
                    //   [2..5]   numSampleFrames   (u32 BE)
                    //   [6..7]   sampleSize (bits) (i16 BE)
                    //   [8..17]  sampleRate        (80-bit IEEE extended BE)
                    //   [18..21] (AIFC only) compressionType FourCC
                    _fmt.channels = static_cast<u16>(read_i16_be(comm_buf.data()));
                    _frame_count = read_u32_be(comm_buf.data() + 2);
                    _src_bits_per_sample = static_cast<u16>(read_i16_be(comm_buf.data() + 6));
                    const f64 rate = read_extended_be(comm_buf.data() + 8);
                    _fmt.sample_rate = static_cast<u32>(rate);

                    if (is_aifc) {
                        if (chunk_size < 22) {
                            throw UnsupportedAudioFileException("AIFC COMM missing codec");
                        }
                        const u8* ct = comm_buf.data() + 18;
                        if (aiff_tag_equals(ct, "NONE")) {
                            _codec = AiffCodec::NONE;
                        } else if (
                            aiff_tag_equals(ct, "sowt") ||
                            aiff_tag_equals(ct, "SOWT") ||
                            aiff_tag_equals(ct, "twos")
                        ) {
                            // "twos" is technically also BE PCM; treat the
                            // common case (sowt = swap-to-little-endian) as LE.
                            _codec = aiff_tag_equals(ct, "twos")
                                ? AiffCodec::NONE
                                : AiffCodec::SOWT;
                        } else if (
                            aiff_tag_equals(ct, "fl32") ||
                            aiff_tag_equals(ct, "FL32")
                        ) {
                            _codec = AiffCodec::FL32;
                        } else {
                            throw UnsupportedAudioFileException(
                                "AIFC compression not supported (PCM and fl32 only)"
                            );
                        }
                    } else {
                        _codec = AiffCodec::NONE; // plain AIFF is always BE PCM.
                    }
                    got_comm = true;
                } else if (aiff_tag_equals(chunk_hdr, "SSND")) {
                    // SSND: offset(4) + blocksize(4) + sample bytes
                    u8 ssnd_hdr[8];
                    _file.read(reinterpret_cast<char*>(ssnd_hdr), sizeof(ssnd_hdr));
                    if (_file.gcount() != sizeof(ssnd_hdr)) {
                        throw UnsupportedAudioFileException("truncated SSND header");
                    }
                    const u32 offset = read_u32_be(ssnd_hdr);
                    // sample bytes start after the 8-byte SSND header + alignment offset
                    ssnd_data_offset = static_cast<u32>(_file.tellg()) + offset;
                    ssnd_data_size = chunk_size - 8 - offset;
                    // Skip the rest of this chunk; we'll seek back to the
                    // data offset once we've validated COMM.
                    _file.seekg(
                        static_cast<StreamOffset>(chunk_size - 8),
                        SeekingDirection::CURRENT
                    );
                    if (chunk_size & 1u) {
                        _file.seekg(1, SeekingDirection::CURRENT);
                    }
                } else {
                    _file.seekg(
                        static_cast<StreamOffset>(padded_size),
                        SeekingDirection::CURRENT
                    );
                }

                if (got_comm && ssnd_data_size > 0) {
                    break;
                }
            }

            if (!got_comm) {
                throw UnsupportedAudioFileException("AIFF: no COMM chunk");
            }
            if (ssnd_data_size == 0) {
                throw UnsupportedAudioFileException("AIFF: no SSND chunk");
            }
            if (_fmt.channels == 0) {
                throw UnsupportedAudioFileException("AIFF: zero channels");
            }
            if (_codec != AiffCodec::FL32
                    && _src_bits_per_sample != 16
                    && _src_bits_per_sample != 24
                    && _src_bits_per_sample != 32) {
                throw UnsupportedAudioFileException(
                    "AIFF: PCM bit depth must be 16, 24, or 32"
                );
            }
            if (_codec == AiffCodec::FL32 && _src_bits_per_sample != 32) {
                throw UnsupportedAudioFileException(
                    "AIFF: fl32 with bit depth != 32"
                );
            }

            _src_bytes_per_sample = static_cast<u16>(_src_bits_per_sample / 8);
            _src_bytes_per_frame = static_cast<u32>(_fmt.channels) * _src_bytes_per_sample;
            _fmt.format = SampleFormat::FLOAT;
            // COMM's frame count is authoritative; SSND may be padded.
            const u64 frames_from_ssnd = ssnd_data_size / _src_bytes_per_frame;
            if (frames_from_ssnd < _frame_count) {
                _frame_count = frames_from_ssnd;
            }
            _frames_remaining = _frame_count;

            _file.seekg(ssnd_data_offset, SeekingDirection::BEGINNING);

            _raw_buf.reserve(static_cast<usize>(_fmt.sample_rate) * _src_bytes_per_frame / 4);
        }

        usize read(Span<f32> out) noexcept override {
            if (_frames_remaining == 0 || _fmt.channels == 0) {
                return 0;
            }
            const usize frames_req = out.size() / _fmt.channels;
            if (frames_req == 0) {
                return 0;
            }
            const usize frames = (frames_req < _frames_remaining)
                ? frames_req
                : static_cast<usize>(_frames_remaining);
            const usize bytes = frames * _src_bytes_per_frame;

            _raw_buf.resize(bytes);
            _file.read(
                reinterpret_cast<char*>(_raw_buf.data()),
                static_cast<StreamSize>(bytes)
            );
            const usize got_bytes = static_cast<usize>(_file.gcount());
            const usize got_frames = got_bytes / _src_bytes_per_frame;

            convert_to_f32(_raw_buf.data(), got_frames, out);
            _frames_remaining -= got_frames;
            return got_frames;
        }

        [[nodiscard]]
        const AudioFormat& format() const noexcept override {
            return _fmt;
        }

        [[nodiscard]]
        u64 total_frames() const noexcept override {
            return _frame_count;
        }

        [[nodiscard]]
        u64 position_frames() const noexcept override {
            return _frame_count - _frames_remaining;
        }
    };
}
