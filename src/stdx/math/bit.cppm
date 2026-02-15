/**
 * @file bit.cppm
 * @module stdx:math.bit
 * @brief Module file for standard library bit manipulation operations.
 *
 * This file contains the implementation of the bit manipulation operations in the standard library.
 */

module;

export module stdx:math.bit;

import core;

/**
 * @namespace stdx::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace stdx::math {
    using core::math::Endian;

    using core::math::bit_cast;
    using core::math::byteswap;
    using core::math::has_single_bit;
    using core::math::bit_ceil;
    using core::math::bit_floor;
    using core::math::bit_width;
    using core::math::rotl;
    using core::math::rotr;
    using core::math::countl_zero;
    using core::math::countl_one;
    using core::math::countr_zero;
    using core::math::countr_one;
    using core::math::popcount;
}
