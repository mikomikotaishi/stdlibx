/**
 * @file algorithm.cppm
 * @module stdx:math.algorithm
 * @brief Module file for standard library mathematical operations from <algorithm>.
 *
 * This file contains the implementation of the mathematical operations from <algorithm> in the standard library.
 */

module;

export module stdx:math.algorithm;

import core;

/**
 * @namespace stdx::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace stdx::math {
    using core::math::includes;
    using core::math::set_difference;
    using core::math::set_intersection;
    using core::math::set_symmetric_difference;
    using core::math::set_union;

    using core::math::max;
    using core::math::max_element;
    using core::math::min;
    using core::math::min_element;
    using core::math::minmax;
    using core::math::minmax_element;
    using core::math::clamp;
}
