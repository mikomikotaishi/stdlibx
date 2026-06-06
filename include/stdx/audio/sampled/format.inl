#pragma once

/**
 * @namespace stdx::audio::sampled
 * @brief PCM audio format descriptors.
 */
export namespace stdx::audio::sampled {
    /**
     * @enum SampleFormat
     * @brief Sample encoding for an interleaved (or planar) PCM frame.
     */
    enum class SampleFormat: u8 {
        FLOAT, ///< 32-bit float, interleaved (the canonical render format)
        FLOAT_PLANAR, ///< 32-bit float, one buffer per channel
        SHORT, ///< 16-bit signed integer, interleaved
        MEDIUM, ///< 24-bit signed integer packed into 3 bytes, interleaved
        INT, ///< 32-bit signed integer, interleaved
    };

    [[nodiscard]]
    constexpr usize bytes_per_sample(SampleFormat fmt) noexcept {
        switch (fmt) {
            case SampleFormat::FLOAT:
            case SampleFormat::FLOAT_PLANAR:
            case SampleFormat::INT:
                return 4;
            case SampleFormat::MEDIUM:
                return 3;
            case SampleFormat::SHORT:
                return 2;
            default:
                Ops::unreachable();
        }
        Ops::unreachable();
    }

    [[nodiscard]]
    constexpr bool is_planar(SampleFormat fmt) noexcept {
        return fmt == SampleFormat::FLOAT_PLANAR;
    }

    [[nodiscard]]
    constexpr bool is_float(SampleFormat fmt) noexcept {
        return fmt == SampleFormat::FLOAT || fmt == SampleFormat::FLOAT_PLANAR;
    }

    /**
     * @struct AudioFormat
     * @brief A PCM audio format: sample rate + channel count + sample encoding.
     */
    struct AudioFormat {
        u32 sample_rate;
        u16 channels;
        SampleFormat format;

        [[nodiscard]]
        constexpr usize bytes_per_frame() const noexcept {
            return is_planar(format)
                ? bytes_per_sample(format)
                : static_cast<usize>(channels) * bytes_per_sample(format);
        }

        [[nodiscard]]
        constexpr usize byte_rate() const noexcept {
            return static_cast<usize>(sample_rate) * bytes_per_frame();
        }

        [[nodiscard]]
        friend constexpr bool operator==(const AudioFormat& x, const AudioFormat& y) = default;
    };

    /**
     * @struct AudioTime
     * @brief Position + host-time snapshot reported alongside a render callback.
     */
    struct AudioTime {
        u64 frames_elapsed; ///< Total frames produced/consumed since line start.
        u64 host_time_ns; ///< Monotonic host time at the callback boundary.
    };

    /**
     * @enum ChannelPosition
     * @brief Symbolic identifiers for individual channels in a multi-channel stream.
     * Bit positions match the WAV/WAVE_FORMAT_EXTENSIBLE channel mask.
     */
    enum class ChannelPosition: u32 {
        FRONT_LEFT = 0x1,
        FRONT_RIGHT = 0x2,
        FRONT_CENTER = 0x4,
        LOW_FREQUENCY = 0x8,
        BACK_LEFT = 0x10,
        BACK_RIGHT = 0x20,
        FRONT_LEFT_OF_CENTER = 0x40,
        FRONT_RIGHT_OF_CENTER = 0x80,
        BACK_CENTER = 0x100,
        SIDE_LEFT = 0x200,
        SIDE_RIGHT = 0x400,
        TOP_CENTER = 0x800,
        TOP_FRONT_LEFT = 0x1000,
        TOP_FRONT_CENTER = 0x2000,
        TOP_FRONT_RIGHT = 0x4000,
        TOP_BACK_LEFT = 0x8000,
        TOP_BACK_CENTER = 0x10000,
        TOP_BACK_RIGHT = 0x20000,
    };

    /**
     * @struct ChannelLayout
     * @brief Bitmask of @ref ChannelPosition flags.
     */
    struct ChannelLayout {
        u32 mask = 0;

        [[nodiscard]]
        constexpr bool has(ChannelPosition p) const noexcept {
            return (mask & static_cast<u32>(p)) != 0;
        }

        [[nodiscard]]
        constexpr u16 count() const noexcept {
            return static_cast<u16>(Math::count_pop(mask));
        }

        [[nodiscard]]
        static constexpr ChannelLayout mono() noexcept {
            return ChannelLayout{static_cast<u32>(ChannelPosition::FRONT_CENTER)};
        }

        [[nodiscard]]
        static constexpr ChannelLayout stereo() noexcept {
            return ChannelLayout{
                static_cast<u32>(ChannelPosition::FRONT_LEFT) |
                static_cast<u32>(ChannelPosition::FRONT_RIGHT)
            };
        }

        static constexpr ChannelLayout surround_5_1() noexcept {
            return ChannelLayout{
                static_cast<u32>(ChannelPosition::FRONT_LEFT) |
                static_cast<u32>(ChannelPosition::FRONT_RIGHT) |
                static_cast<u32>(ChannelPosition::FRONT_CENTER) |
                static_cast<u32>(ChannelPosition::LOW_FREQUENCY) |
                static_cast<u32>(ChannelPosition::SIDE_LEFT) |
                static_cast<u32>(ChannelPosition::SIDE_RIGHT)
            };
        }
    };
}
