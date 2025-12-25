/**
 * @file cstddef.cppm
 * @module core:math.cstddef
 * @brief Module file for standard library C-style integer conversion operations.
 *
 * This file contains the implementation of the C-style integer conversion operations in the standard library.
 */

module;

#include <cstddef>

export module core:math.cstddef;

/**
 * @namespace core::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace core::math {
    using std::to_integer;
}
