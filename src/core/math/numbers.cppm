/**
 * @file numbers.cppm
 * @module core:math.numbers
 * @brief Module file for standard library mathematical constants.
 *
 * This file contains the implementation of the mathematical constants in the standard library.
 */

module;

#if __has_include(<numbers>)
#include <numbers>
#endif

export module core:math.numbers;

/**
 * @namespace core::math
 * @brief Wrapper namespace for standard library numerical constants (for compatibility).
 */
export namespace core::math {
    #if __has_include(<numbers>)
    class Numbers {
    public:
        Numbers() = delete;

        static constexpr double E = std::numbers::e; ///< e
        static constexpr double LOG_2_E = std::numbers::log2e; ///< log_2 e
        static constexpr double LOG_10_E = std::numbers::log10e; ///< log_10 e
        static constexpr double PI = std::numbers::pi; ///< pi
        static constexpr double INV_PI = std::numbers::inv_pi; ///< 1/pi
        static constexpr double INV_SQRT_PI = std::numbers::inv_sqrtpi; ///< 1/sqrt(pi)
        static constexpr double LN_2 = std::numbers::ln2; ///< log_e 2
        static constexpr double LN_10 = std::numbers::ln10; ///< log_e 10
        static constexpr double SQRT_2 = std::numbers::sqrt2; ///< sqrt(2)
        static constexpr double SQRT_3 = std::numbers::sqrt3; ///< sqrt(3)
        static constexpr double INV_SQRT_3 = std::numbers::inv_sqrt3; ///< 1/sqrt(3)
        static constexpr double E_GAMMA = std::numbers::egamma; ///< Euler-Mascheroni constant
        static constexpr double PHI = std::numbers::phi; ///< Golden ratio (1+sqrt(5))/2
    };
    #endif
}
