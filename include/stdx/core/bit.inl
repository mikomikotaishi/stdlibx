#pragma once

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
    class [[nodiscard]] Endian final {
    public:
        using Self = std::endian;

        static constexpr Self LITTLE = std::endian::little;
        static constexpr Self BIG = std::endian::big;
        static constexpr Self NATIVE = std::endian::native;
    private:
        const Self value;
    public:
        constexpr Endian(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };
}
