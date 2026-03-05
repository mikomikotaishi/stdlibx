#pragma once

/**
 * @namespace stdx::math::numbers
 * @brief Wrapper namespace for standard library numerical constants (for compatibility).
 */
export namespace stdx::math::inline numbers {
    inline constexpr double E = ::core::math::numbers::E; ///< e
    inline constexpr double LOG_2_E = ::core::math::numbers::LOG_2_E; ///< log_2 e
    inline constexpr double LOG_10_E = ::core::math::numbers::LOG_10_E; ///< log_10 e
    inline constexpr double PI = ::core::math::numbers::PI; ///< pi
    inline constexpr double INV_PI = ::core::math::numbers::INV_PI; ///< 1/pi
    inline constexpr double INV_SQRT_PI = ::core::math::numbers::INV_SQRT_PI; ///< 1/sqrt(pi)
    inline constexpr double LN_2 = ::core::math::numbers::LN_2; ///< log_e 2
    inline constexpr double LN_10 = ::core::math::numbers::LN_10; ///< log_e 10
    inline constexpr double SQRT_2 = ::core::math::numbers::SQRT_2; ///< sqrt(2)
    inline constexpr double SQRT_3 = ::core::math::numbers::SQRT_3; ///< sqrt(3)
    inline constexpr double INV_SQRT_3 = ::core::math::numbers::INV_SQRT_3; ///< 1/sqrt(3)
    inline constexpr double E_GAMMA = ::core::math::numbers::E_GAMMA; ///< Euler-Mascheroni constant
    inline constexpr double PHI = ::core::math::numbers::PHI; ///< Golden ratio (1+sqrt(5))/2
}
