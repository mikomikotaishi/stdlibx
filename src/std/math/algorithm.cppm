/**
 * @file algorithm.cppm
 * @module std:math.algorithm
 * @brief Module file for standard library mathematical operations from <algorithm>.
 *
 * This file contains the implementation of the mathematical operations from <algorithm> in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:math.algorithm;
#else
export module stdlib:math.algorithm;
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
