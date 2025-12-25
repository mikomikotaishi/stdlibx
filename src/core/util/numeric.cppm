/**
 * @file numeric.cppm
 * @module core:util.numeric
 * @brief Module file for standard library numeric operations.
 *
 * This file contains the implementation of the numeric operations in the standard library.
 */

module;

#include <numeric>

export module core:util.numeric;

/**
 * @namespace core::util
 * @brief Wrapper namespace for standard library utility operations.
 */
export namespace core::util {
    using std::iota;
    using std::accumulate;
    using std::reduce;
    using std::transform_reduce;
    using std::inner_product;
    using std::adjacent_difference;
    using std::partial_sum;
    using std::inclusive_scan;
    using std::exclusive_scan;
    using std::transform_inclusive_scan;
    using std::transform_exclusive_scan;
    using std::gcd;
    using std::lcm;
    using std::midpoint;
}
