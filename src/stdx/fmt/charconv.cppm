/**
 * @file charconv.cppm
 * @module stdx:fmt.charconv
 * @brief Module file for standard library character conversion operations.
 *
 * This file contains the implementation of the character conversion operations in the standard library.
 */

module;

#include <charconv>

export module stdx:fmt.charconv;

/**
 * @namespace stdx::fmt
 * @brief Wrapper namespace for standard library format operations.
 */
export namespace stdx::fmt {
    using CharsFormat = std::chars_format;
    using FromCharsResult = std::from_chars_result;
    using ToCharsResult = std::to_chars_result;

    using std::from_chars;
    using std::to_chars;
}
