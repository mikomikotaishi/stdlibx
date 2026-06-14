#pragma once

/**
 * @namespace stdx::math
 * @brief Standard library mathematical operations.
 */
export namespace stdx::math {
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

    #ifdef __cpp_lib_saturation_arithmetic
    using std::saturating_add;
    using std::saturating_sub;
    using std::saturating_mul;
    using std::saturating_div;
    using std::saturating_cast;
    #endif
}
