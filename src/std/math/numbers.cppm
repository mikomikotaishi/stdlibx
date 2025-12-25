/**
 * @file numbers.cppm
 * @module std:math.numbers
 * @brief Module file for standard library mathematical constants.
 *
 * This file contains the implementation of the mathematical constants in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:math.numbers;
#else
export module stdlib:math.numbers;
#endif

import core;

/**
 * @namespace std::math::numbers
 * @brief Wrapper namespace for standard library numerical constants (for compatibility).
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::math::inline numbers {
#else 
export namespace stdlib::math::inline numbers {
#endif
    using core::math::numbers::E; ///< e
    using core::math::numbers::LOG_2_E; ///< log_2(e)
    using core::math::numbers::LOG_10_E; ///< log_10(e)
    using core::math::numbers::PI; ///< pi
    using core::math::numbers::INV_PI; ///< 1/pi
    using core::math::numbers::INV_SQRT_PI; ///< 1/sqrt(pi)
    using core::math::numbers::LN_2; ///< log_e(2)
    using core::math::numbers::LN_10; ///< log_e(10)
    using core::math::numbers::SQRT_2; ///< sqrt(2)
    using core::math::numbers::SQRT_3; ///< sqrt(3)
    using core::math::numbers::INV_SQRT_3; ///< 1/sqrt(3)
    using core::math::numbers::E_GAMMA; ///< Euler-Mascheroni constant
    using core::math::numbers::PHI; ///< Golden ratio (1+sqrt(5))/2
}
