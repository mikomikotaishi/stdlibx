/**
 * @file cstdlib.cppm
 * @module core:util.cstdlib
 * @brief Module file for standard library C-style algorithm operations.
 *
 * This file contains the implementation of the C-style algorithm operations in the standard library.
 */

module;

#include <cstdlib>

export module core:util.cstdlib;

/**
 * @namespace core::util
 * @brief Wrapper namespace for standard library utility operations.
 */
export namespace core::util {
    using std::qsort;
    using std::bsearch;
}

