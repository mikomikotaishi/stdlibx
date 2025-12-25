/**
 * @file bit.cppm
 * @module std:math.bit
 * @brief Module file for standard library bit manipulation operations.
 *
 * This file contains the implementation of the bit manipulation operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:math.bit;
#else
export module stdlib:math.bit;
#endif

import core;

/**
 * @namespace std::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::math {
#else 
export namespace stdlib::math {
#endif
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
 