/**
 * @file charconv.cppm
 * @module core:text.charconv
 * @brief Module file for standard library character conversion operations.
 *
 * This file contains the imports for the character conversion operations in the standard library.
 */

module;

#include <charconv>

export module core:text.charconv;

/**
 * @namespace core::text
 * @brief Wrapper namespace for standard library text operations.
 */
export namespace core::text {
    class CharsFormat {
    public:
        using Self = ::std::chars_format;

        CharsFormat() = delete;

        static constexpr Self SCIENTIFIC = ::std::chars_format::scientific;
        static constexpr Self FIXED = ::std::chars_format::fixed;
        static constexpr Self HEX = ::std::chars_format::hex;
        static constexpr Self GENERAL = ::std::chars_format::general;
    };

    using FromCharsResult = std::from_chars_result;
    using ToCharsResult = std::to_chars_result;

    using std::from_chars;
    using std::to_chars;
}
