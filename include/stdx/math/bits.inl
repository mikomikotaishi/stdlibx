#pragma once

/**
 * @namespace stdx::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace stdx::math {
    class [[nodiscard]] Endian final {
    public:
        using Self = std::endian;

        static constexpr Self LITTLE = std::endian::little;
        static constexpr Self BIG = std::endian::big;
        static constexpr Self NATIVE = std::endian::native;
    private:
        const Self value;
    public:
        constexpr Endian() noexcept = delete;

        constexpr Endian(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };

    using std::bit_cast;
    using std::byteswap;
    using std::has_single_bit;
    using std::bit_ceil;
    using std::bit_floor;
    using std::bit_width;
    using std::rotl;
    using std::rotr;
    using std::countl_zero;
    using std::countl_one;
    using std::countr_zero;
    using std::countr_one;
    using std::popcount;
}
