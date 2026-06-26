#pragma once

/**
 * @namespace stdx::audio::sampled
 * @brief Pull-style audio decoding interfaces.
 *
 * AudioInputStream is the contract every file-format decoder satisfies:
 * the caller supplies an f32 buffer, the decoder fills it with as many
 * interleaved frames as it can produce. Conversion from the on-disk
 * representation (s16/s24/s32/IEEE-float) into normalized f32 happens
 * inside the decoder, so consumers never branch on encoding.
 */
export namespace stdx::audio::sampled {
    /**
     * @enum AudioFileType
     * @brief Container format identifier returned by the file sniffer.
     */
    enum class AudioFileType: u8 {
        UNKNOWN, ///< Magic bytes did not match any known container.
        WAV, ///< Microsoft RIFF/WAVE.
        AIFF, ///< Apple AIFF/AIFF-C (placeholder - not yet decoded).
        FLAC, ///< Xiph FLAC (placeholder - needs libFLAC extension).
        OGG, ///< Xiph Ogg/Vorbis (placeholder - needs libvorbis extension).
        MP3, ///< MPEG-1/2 Layer III (placeholder - needs libmpg123 extension).
    };

    /**
     * @struct AudioFileFormat
     * @brief Summary of an audio file's header. Returned by the sniffer
     * before a full decoder is constructed.
     */
    struct AudioFileFormat {
        AudioFileType type = AudioFileType::UNKNOWN;
        AudioFormat format{}; ///< Channels / sample rate / sample encoding.
        u64 total_frames = 0; ///< Total frame count, 0 if not knowable from the header.
    };

    /**
     * @interface AudioInputStream
     * @brief Pull-style source of interleaved f32 frames decoded from a file.
     *
     * The render loop calls @ref read on its audio thread. Implementations
     * must be lock-free at steady state (one-time setup in the constructor
     * is fine); they MUST NOT allocate on the audio thread.
     */
    class AudioInputStream {
    public:
        AudioInputStream() = default;
        AudioInputStream(const AudioInputStream&) = delete("AudioInputStream is not copyable.");
        AudioInputStream& operator=(const AudioInputStream&) = delete("AudioInputStream is not copyable.");
        virtual ~AudioInputStream() = default;

        /**
         * @brief Decode up to @c out.size()/channels frames into @p out, return
         * the number of frames actually filled.
         *
         * The buffer is written contiguously starting at @c out[0]; on a short
         * read (end-of-stream, short physical read) the tail of @p out is left
         * untouched - the caller is responsible for zero-filling if it wants
         * silence. Returns 0 at EOF; subsequent calls keep returning 0.
         * @param out Interleaved f32 output buffer. The number of frames requested is
         * @c out.size() / channels.
         * @return The number of frames decoded into @p out, or 0 if the
         * stream is exhausted.
         */
        virtual usize read(Span<f32> out) noexcept = 0;

        /**
         * @brief The decoded audio format. Always @c SampleFormat::FLOAT -
         * the source encoding is converted by the decoder itself.
         */
        [[nodiscard]]
        virtual const AudioFormat& format() const noexcept = 0;

        /**
         * @brief Total decoded frames, if knowable from the header.
         * Streams (live captures, pipes) return 0.
         */
        [[nodiscard]]
        virtual u64 total_frames() const noexcept = 0;

        /**
         * @brief Frame index of the next frame @c read will produce.
         */
        [[nodiscard]]
        virtual u64 position_frames() const noexcept = 0;
    };
}
