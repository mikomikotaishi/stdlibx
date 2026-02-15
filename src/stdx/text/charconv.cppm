/**
 * @file charconv.cppm
 * @module stdx:text.charconv
 * @brief Module file for standard library character conversion operations.
 *
 * This file contains the imports for the character conversion operations in the standard library.
 */

module;

export module stdx:text.charconv;

import core;

/**
 * @namespace stdx::text
 * @brief Wrapper namespace for standard library text operations.
 */
export namespace stdx::text {
    using core::text::CharsFormat;
    using core::text::FromCharsResult;
    using core::text::ToCharsResult;
    using core::text::from_chars;
    using core::text::to_chars;
}
