/**
 * @file cstdlib.cppm
 * @module stdx:text.cstdlib
 * @brief Module file for standard library C-style string manipulation operations.
 *
 * This file contains the imports for the C-style string manipulation operations in the standard library.
 */

module;

export module stdx:text.cstdlib;

import core;

/**
 * @namespace stdx::text
 * @brief Wrapper namespace for standard library text operations.
 */
export namespace stdx::text {
    using core::text::atof;
    using core::text::atoi;
    using core::text::atol;
    using core::text::atoll;
    using core::text::strtol;
    using core::text::strtoll;
    using core::text::strtoul;
    using core::text::strtoull;
    using core::text::strtof;
    using core::text::strtod;
    using core::text::strtold;
    using core::text::mblen;
    using core::text::mbtowc;
    using core::text::wctomb;
    using core::text::mbstowcs;
    using core::text::wcstombs;
}
