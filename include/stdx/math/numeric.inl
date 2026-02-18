#pragma once

/**
 * @namespace stdx::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace stdx::math {
    using ::core::math::iota;
    using ::core::math::accumulate;
    using ::core::math::reduce;
    using ::core::math::transform_reduce;
    using ::core::math::inner_product;
    using ::core::math::adjacent_difference;
    using ::core::math::partial_sum;
    using ::core::math::inclusive_scan;
    using ::core::math::exclusive_scan;
    using ::core::math::transform_inclusive_scan;
    using ::core::math::transform_exclusive_scan;
    using ::core::math::gcd;
    using ::core::math::lcm;
    using ::core::math::midpoint;
}
