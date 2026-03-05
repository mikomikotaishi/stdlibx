#pragma once

/**
 * @namespace core::math::numbers
 * @brief Wrapper namespace for standard library numerical constants (for compatibility).
 */
export namespace core::math::inline numbers {
    inline constexpr double E = std::numbers::e; ///< e
    inline constexpr double LOG_2_E = std::numbers::log2e; ///< log_2 e
    inline constexpr double LOG_10_E = std::numbers::log10e; ///< log_10 e
    inline constexpr double PI = std::numbers::pi; ///< pi
    inline constexpr double INV_PI = std::numbers::inv_pi; ///< 1/pi
    inline constexpr double INV_SQRT_PI = std::numbers::inv_sqrtpi; ///< 1/sqrt(pi)
    inline constexpr double LN_2 = std::numbers::ln2; ///< log_e 2
    inline constexpr double LN_10 = std::numbers::ln10; ///< log_e 10
    inline constexpr double SQRT_2 = std::numbers::sqrt2; ///< sqrt(2)
    inline constexpr double SQRT_3 = std::numbers::sqrt3; ///< sqrt(3)
    inline constexpr double INV_SQRT_3 = std::numbers::inv_sqrt3; ///< 1/sqrt(3)
    inline constexpr double E_GAMMA = std::numbers::egamma; ///< Euler-Mascheroni constant
    inline constexpr double PHI = std::numbers::phi; ///< Golden ratio (1+sqrt(5))/2
}