/**
 * @file numeric.cppm
 * @module std:math.numeric
 * @brief Module file for standard library numeric operations.
 *
 * This file contains the implementation of the numeric operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:math.numeric;
#else
export module stdlib:math.numeric;
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
    using core::math::iota;
    using core::math::accumulate;
    using core::math::reduce;
    using core::math::transform_reduce;
    using core::math::inner_product;
    using core::math::adjacent_difference;
    using core::math::partial_sum;
    using core::math::inclusive_scan;
    using core::math::exclusive_scan;
    using core::math::transform_inclusive_scan;
    using core::math::transform_exclusive_scan;
    using core::math::gcd;
    using core::math::lcm;
    using core::math::midpoint;
}
