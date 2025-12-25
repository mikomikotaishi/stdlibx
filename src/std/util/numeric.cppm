/**
 * @file numeric.cppm
 * @module std:util.numeric
 * @brief Module file for standard library numeric operations.
 *
 * This file contains the implementation of the numeric operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:util.numeric;
#else
export module stdlib:util.numeric;
#endif

import core;

/**
 * @namespace std::util
 * @brief Wrapper namespace for standard library utility operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::util {
#else 
export namespace stdlib::util {
#endif
    using core::util::iota;
    using core::util::accumulate;
    using core::util::reduce;
    using core::util::transform_reduce;
    using core::util::inner_product;
    using core::util::adjacent_difference;
    using core::util::partial_sum;
    using core::util::inclusive_scan;
    using core::util::exclusive_scan;
    using core::util::transform_inclusive_scan;
    using core::util::transform_exclusive_scan;
    using core::util::gcd;
    using core::util::lcm;
    using core::util::midpoint;
}
