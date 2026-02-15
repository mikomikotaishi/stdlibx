/**
 * @file cstdlib.cppm
 * @module stdx:util.cstdlib
 * @brief Module file for standard library C-style algorithm operations.
 *
 * This file contains the implementation of the C-style algorithm operations in the standard library.
 */

module;

export module stdx:util.cstdlib;

import core;

/**
 * @namespace stdx::util
 * @brief Wrapper namespace for standard library utility operations.
 */
export namespace stdx::util {
    using core::util::qsort;
    using core::util::bsearch;
}

