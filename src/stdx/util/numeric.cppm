/**
 * @file numeric.cppm
 * @module stdx:util.numeric
 * @brief Module file for standard library numeric operations.
 *
 * This file contains the implementation of the numeric operations in the standard library.
 */

module;

export module stdx:util.numeric;

import core;

/**
 * @namespace stdx::util
 * @brief Wrapper namespace for standard library utility operations.
 */
export namespace stdx::util {
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
