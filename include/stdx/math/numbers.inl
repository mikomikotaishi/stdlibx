#pragma once

/**
 * @namespace stdx::math::numbers
 * @brief Wrapper namespace for standard library numerical constants (for compatibility).
 */
export namespace stdx::math::inline numbers {
    inline constexpr f64 E = ::core::math::numbers::E; ///< e
    inline constexpr f64 LOG_2_E = ::core::math::numbers::LOG_2_E; ///< log_2 e
    inline constexpr f64 LOG_10_E = ::core::math::numbers::LOG_10_E; ///< log_10 e
    inline constexpr f64 PI = ::core::math::numbers::PI; ///< pi
    inline constexpr f64 INV_PI = ::core::math::numbers::INV_PI; ///< 1/pi
    inline constexpr f64 INV_SQRT_PI = ::core::math::numbers::INV_SQRT_PI; ///< 1/sqrt(pi)
    inline constexpr f64 LN_2 = ::core::math::numbers::LN_2; ///< log_e 2
    inline constexpr f64 LN_10 = ::core::math::numbers::LN_10; ///< log_e 10
    inline constexpr f64 SQRT_2 = ::core::math::numbers::SQRT_2; ///< sqrt(2)
    inline constexpr f64 SQRT_3 = ::core::math::numbers::SQRT_3; ///< sqrt(3)
    inline constexpr f64 INV_SQRT_3 = ::core::math::numbers::INV_SQRT_3; ///< 1/sqrt(3)
    inline constexpr f64 E_GAMMA = ::core::math::numbers::E_GAMMA; ///< Euler-Mascheroni constant
    inline constexpr f64 PHI = ::core::math::numbers::PHI; ///< Golden ratio (1+sqrt(5))/2

    using ::core::math::numbers::E_VALUE;
    using ::core::math::numbers::LOG_2_E_VALUE;
    using ::core::math::numbers::LOG_10_E_VALUE;
    using ::core::math::numbers::PI_VALUE;
    using ::core::math::numbers::INV_PI_VALUE;
    using ::core::math::numbers::INV_SQRT_PI_VALUE;
    using ::core::math::numbers::LN_2_VALUE;
    using ::core::math::numbers::LN_10_VALUE;
    using ::core::math::numbers::SQRT_2_VALUE;
    using ::core::math::numbers::SQRT_3_VALUE;
    using ::core::math::numbers::INV_SQRT_3_VALUE;
    using ::core::math::numbers::E_GAMMA_VALUE;
    using ::core::math::numbers::PHI_VALUE;
}
