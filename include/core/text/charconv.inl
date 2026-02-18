#pragma once

/**
 * @namespace core::text
 * @brief Wrapper namespace for standard library text operations.
 */
export namespace core::text {
    class [[nodiscard]] CharsFormat final {
    public:
        using Self = ::std::chars_format;

        static constexpr Self SCIENTIFIC = ::std::chars_format::scientific;
        static constexpr Self FIXED = ::std::chars_format::fixed;
        static constexpr Self HEX = ::std::chars_format::hex;
        static constexpr Self GENERAL = ::std::chars_format::general;
    private:
        Self value;
    public:
        CharsFormat(Self value = Self()) noexcept:
            value{value} {}

        operator Self() const noexcept {
            return value;
        }
    };

    using FromCharsResult = std::from_chars_result;
    using ToCharsResult = std::to_chars_result;

    using std::from_chars;
    using std::to_chars;
}
