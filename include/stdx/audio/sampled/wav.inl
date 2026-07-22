#pragma once

using stdx::collections::Vector;
using stdx::fs::Path;
using stdx::io::InputFileStream;
using stdx::io::OpenMode;
using stdx::io::SeekingDirection;
using stdx::io::StreamSize;

/**
 * @namespace stdx::audio::sampled
 * @brief WAV (RIFF/WAVE) audio file decoder.
 */
namespace stdx::audio::sampled {
    /// WAVE_FORMAT_PCM (integer PCM): 8/16/24/32-bit signed (8-bit is unsigned).
    constexpr u16 WAVE_FORMAT_PCM = 0x0001;
    /// WAVE_FORMAT_IEEE_FLOAT (32-bit float PCM).
    constexpr u16 WAVE_FORMAT_IEEE_FLOAT = 0x0003;
    /// WAVE_FORMAT_EXTENSIBLE: real format tag lives in a sub-GUID's first 2 bytes.
    constexpr u16 WAVE_FORMAT_EXTENSIBLE = 0xFFFE;

    /// 1 / 2^15 - s16 → f32 normalization scale.
    constexpr f32 INT16_NORM = 1.0f / 32768.0f;
    /// 1 / 2^23 - s24 → f32 normalization scale.
    constexpr f32 INT24_NORM = 1.0f / 8388608.0f;
    /// 1 / 2^31 - s32 → f32 normalization scale.
    constexpr f32 INT32_NORM = 1.0f / 2147483648.0f;

    [[nodiscard]]
    inline u16 read_u16_le(const u8* p) noexcept {
        return static_cast<u16>(p[0])
            | (static_cast<u16>(p[1]) << 8);
    }

    [[nodiscard]]
    inline u32 read_u32_le(const u8* p) noexcept {
        return static_cast<u32>(p[0])
            | (static_cast<u32>(p[1]) << 8)
            | (static_cast<u32>(p[2]) << 16)
            | (static_cast<u32>(p[3]) << 24);
    }

    [[nodiscard]]
    inline i16 read_i16_le(const u8* p) noexcept {
        return static_cast<i16>(read_u16_le(p));
    }

    [[nodiscard]]
    inline i32 read_i24_le(const u8* p) noexcept {
        // Assemble 24-bit little-endian and sign-extend the top bit.
        i32 v = static_cast<i32>(p[0])
            | (static_cast<i32>(p[1]) << 8)
            | (static_cast<i32>(p[2]) << 16);
        if (v & 0x800000) {
            v |= static_cast<i32>(0xFF000000);
        }
        return v;
    }

    [[nodiscard]]
    inline i32 read_i32_le(const u8* p) noexcept {
        return static_cast<i32>(read_u32_le(p));
    }

    [[nodiscard]]
    inline f32 read_f32_le(const u8* p) noexcept {
        const u32 bits = read_u32_le(p);
        f32 out;
        stdx::mem::memcpy(&out, &bits, sizeof(out));
        return out;
    }

    [[nodiscard]]
    inline bool tag_equals(const u8* tag, const char (&lit)[5]) noexcept {
        return tag[0] == static_cast<u8>(lit[0])
            && tag[1] == static_cast<u8>(lit[1])
            && tag[2] == static_cast<u8>(lit[2])
            && tag[3] == static_cast<u8>(lit[3]);
    }

    /**
     * @class WavAudioInputStream
     * @brief RIFF/WAVE decoder. Supports PCM (s16, s24, s32) and IEEE-float (f32),
     * including WAVE_FORMAT_EXTENSIBLE that unwraps to one of those subformats.
     * @extends AudioInputStream
     */
    class WavAudioInputStream final: public AudioInputStream {
    private:
        InputFileStream _file;
        Vector<u8> _raw_buf; ///< Reusable scratch - sized lazily on first read().
        AudioFormat _fmt = {};
        u64 _frame_count = 0; ///< Total frames in the data chunk.
        u64 _frames_remaining = 0; ///< Frames left to deliver via read().
        u32 _src_bytes_per_frame = 0; ///< channels * bytes_per_sample.
        u16 _src_format_tag = 0; ///< Effective format tag after EXTENSIBLE unwrap.
        u16 _src_bits_per_sample = 0;
        u16 _src_bytes_per_sample = 0; ///< bits / 8 - stride between consecutive samples.

        void convert_to_f32(const u8* src, usize frames, Span<f32> out) const noexcept {
            const usize samples = frames * _fmt.channels;
            const u16 stride = _src_bytes_per_sample;
            if (_src_format_tag == WAVE_FORMAT_IEEE_FLOAT) {
                for (usize i = 0; i < samples; ++i) {
                    out[i] = read_f32_le(src + i * stride);
                }
                return;
            }
            switch (_src_bits_per_sample) {
                case 16:
                    for (usize i = 0; i < samples; ++i) {
                        out[i] = static_cast<f32>(read_i16_le(src + i * stride)) * INT16_NORM;
                    }
                    break;
                case 24:
                    for (usize i = 0; i < samples; ++i) {
                        out[i] = static_cast<f32>(read_i24_le(src + i * stride)) * INT24_NORM;
                    }
                    break;
                case 32:
                    for (usize i = 0; i < samples; ++i) {
                        out[i] = static_cast<f32>(read_i32_le(src + i * stride)) * INT32_NORM;
                    }
                    break;
                default:
                    Ops::unreachable();
            }
        }
    public:
        THROWS(UnsupportedAudioFileException)
        explicit WavAudioInputStream(const Path& path) {
            _file.open(path, OpenMode::BINARY);
            if (!_file) {
                throw UnsupportedAudioFileException("cannot open WAV file");
            }

            u8 header[12];
            _file.read(reinterpret_cast<char*>(header), sizeof(header));
            if (_file.gcount() != sizeof(header) ||
                !tag_equals(header, "RIFF") ||
                !tag_equals(header + 8, "WAVE")
            ) {
                throw UnsupportedAudioFileException("not a RIFF/WAVE file");
            }

            bool got_fmt = false;
            u32 data_chunk_offset = 0;
            u32 data_chunk_size = 0;

            // Walk chunks until we've seen both fmt and data. Chunks are
            // padded to even byte boundary - skip the trailer byte when the
            // declared size is odd.
            u8 chunk_hdr[8];
            while (
                _file.read(reinterpret_cast<char*>(chunk_hdr), sizeof(chunk_hdr)),
                _file.gcount() == sizeof(chunk_hdr)
            ) {
                const u32 chunk_size = read_u32_le(chunk_hdr + 4);
                const u32 padded_size = chunk_size + (chunk_size & 1u);

                if (tag_equals(chunk_hdr, "fmt ")) {
                    if (chunk_size < 16) {
                        throw UnsupportedAudioFileException("fmt chunk too small");
                    }
                    Vector<u8> fmt_buf(chunk_size);
                    _file.read(reinterpret_cast<char*>(fmt_buf.data()), chunk_size);
                    if (_file.gcount() != static_cast<StreamSize>(chunk_size)) {
                        throw UnsupportedAudioFileException("truncated fmt chunk");
                    }
                    if (chunk_size & 1u) {
                        _file.seekg(1, SeekingDirection::CURRENT);
                    }

                    _src_format_tag = read_u16_le(fmt_buf.data());
                    _fmt.channels = read_u16_le(fmt_buf.data() + 2);
                    _fmt.sample_rate = read_u32_le(fmt_buf.data() + 4);
                    _src_bits_per_sample = read_u16_le(fmt_buf.data() + 14);

                    if (_src_format_tag == WAVE_FORMAT_EXTENSIBLE) {
                        if (chunk_size < 40) {
                            throw UnsupportedAudioFileException(
                                "EXTENSIBLE fmt chunk truncated"
                            );
                        }
                        // SubFormat GUID at offset 24; its first 2 bytes are
                        // the effective format tag.
                        _src_format_tag = read_u16_le(fmt_buf.data() + 24);
                    }
                    got_fmt = true;
                } else if (tag_equals(chunk_hdr, "data")) {
                    data_chunk_offset = static_cast<u32>(_file.tellg());
                    data_chunk_size = chunk_size;
                    _file.seekg(padded_size, SeekingDirection::CURRENT);
                } else {
                    _file.seekg(padded_size, SeekingDirection::CURRENT);
                }

                if (got_fmt && data_chunk_size > 0) {
                    break;
                }
            }

            if (!got_fmt) {
                throw UnsupportedAudioFileException("WAV: no fmt chunk");
            }
            if (data_chunk_size == 0) {
                throw UnsupportedAudioFileException("WAV: no data chunk");
            }

            const bool tag_ok =
                (_src_format_tag == WAVE_FORMAT_PCM)
                || (_src_format_tag == WAVE_FORMAT_IEEE_FLOAT);
            if (!tag_ok) {
                throw UnsupportedAudioFileException(
                    "WAV codec not supported (only PCM and IEEE float)"
                );
            }
            if (_src_format_tag == WAVE_FORMAT_IEEE_FLOAT && _src_bits_per_sample != 32) {
                throw UnsupportedAudioFileException(
                    "WAV: IEEE float at bit depth != 32"
                );
            }
            if (_src_format_tag == WAVE_FORMAT_PCM &&
                _src_bits_per_sample != 16 &&
                _src_bits_per_sample != 24 &&
                _src_bits_per_sample != 32) {
                throw UnsupportedAudioFileException(
                    "WAV: PCM bit depth must be 16, 24, or 32"
                );
            }
            if (_fmt.channels == 0) {
                throw UnsupportedAudioFileException("WAV: zero channels");
            }

            _src_bytes_per_sample = static_cast<u16>(_src_bits_per_sample / 8);
            _src_bytes_per_frame = static_cast<u32>(_fmt.channels) * _src_bytes_per_sample;
            _fmt.format = SampleFormat::FLOAT;
            _frame_count = data_chunk_size / _src_bytes_per_frame;
            _frames_remaining = _frame_count;

            _file.seekg(data_chunk_offset, SeekingDirection::BEGINNING);

            // ~250ms of scratch - typical render periods are 5-20ms, so the
            // first read() resizes within reserve() and subsequent calls hit
            // the existing capacity.
            _raw_buf.reserve(static_cast<usize>(_fmt.sample_rate) * _src_bytes_per_frame / 4);
        }

        usize read(Span<f32> out) noexcept override {
            if (_frames_remaining == 0 || _fmt.channels == 0) {
                return 0;
            }
            usize frames_req = out.size() / _fmt.channels;
            if (frames_req == 0) {
                return 0;
            }
            const usize frames = (frames_req < _frames_remaining)
                ? frames_req
                : static_cast<usize>(_frames_remaining);
            const usize bytes = frames * _src_bytes_per_frame;

            _raw_buf.resize(bytes);
            _file.read(
                reinterpret_cast<char*>(_raw_buf.data()), static_cast<StreamSize>(bytes)
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
