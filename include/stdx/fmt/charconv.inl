#pragma once

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
