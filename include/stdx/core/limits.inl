#pragma once

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
    template <typename T>
    using NumericLimits = std::numeric_limits<T>;

    class [[nodiscard]] FloatRoundStyle final {
    public:
        using Self = std::float_round_style;

        static constexpr Self INDETERMINATE = std::float_round_style::round_indeterminate;
        static constexpr Self TOWARD_ZERO = std::float_round_style::round_toward_zero;
        static constexpr Self TO_NEAREST = std::float_round_style::round_to_nearest;
        static constexpr Self TOWARD_POSITIVE_INFINITY = std::float_round_style::round_toward_infinity;
        static constexpr Self TOWARD_NEGATIVE_INFINITY = std::float_round_style::round_toward_neg_infinity;
    private:
        const Self value = TOWARD_ZERO;
    public:
        constexpr FloatRoundStyle() noexcept = default;

        constexpr FloatRoundStyle(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    class [[nodiscard]] FloatDenormStyle final {
    public:
        using Self = std::float_denorm_style;

        static constexpr Self INDETERMINATE = std::float_denorm_style::denorm_indeterminate;
        static constexpr Self ABSENT = std::float_denorm_style::denorm_absent;
        static constexpr Self PRESENT = std::float_denorm_style::denorm_present;
    private:
        const Self value = ABSENT;
    public:
        constexpr FloatDenormStyle() noexcept = default;

        constexpr FloatDenormStyle(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };
}
