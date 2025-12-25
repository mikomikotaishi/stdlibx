/**
 * @file bit.cppm
 * @module core:math.bit
 * @brief Module file for standard library bit manipulation operations.
 *
 * This file contains the implementation of the bit manipulation operations in the standard library.
 */

module;

#if __has_include(<bit>)
#include <bit>
#endif

export module core:math.bit;

/**
 * @namespace core::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace core::math {
    #if __has_include(<bit>)
    using Endian = std::endian;

    using std::bit_cast;
    using std::byteswap;
    using std::has_single_bit;
    using std::bit_ceil;
    using std::bit_floor;
    using std::bit_width;
    using std::rotl;
    using std::rotr;
    using std::countl_zero;
    using std::countl_one;
    using std::countr_zero;
    using std::countr_one;
    using std::popcount;
    #endif
}
