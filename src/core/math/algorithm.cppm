/**
 * @file algorithm.cppm
 * @module core:math.algorithm
 * @brief Module file for standard library mathematical operations from <algorithm>.
 *
 * This file contains the implementation of the mathematical operations from <algorithm> in the standard library.
 */

module;

#include <algorithm>

export module core:math.algorithm;

/**
 * @namespace core::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace core::math {
    using std::includes;
    using std::set_difference;
    using std::set_intersection;
    using std::set_symmetric_difference;
    using std::set_union;

    using std::max;
    using std::max_element;
    using std::min;
    using std::min_element;
    using std::minmax;
    using std::minmax_element;
    using std::clamp;
}
