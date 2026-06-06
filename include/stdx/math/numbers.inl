#pragma once

/**
 * @namespace stdx::math::numbers
 * @brief Standard library numerical constants (for compatibility).
 */
export namespace stdx::math::inline numbers {
    inline constexpr f64 E = std::numbers::e; ///< e
    inline constexpr f64 LOG_2_E = std::numbers::log2e; ///< log_2 e
    inline constexpr f64 LOG_10_E = std::numbers::log10e; ///< log_10 e
    inline constexpr f64 PI = std::numbers::pi; ///< pi
    inline constexpr f64 INV_PI = std::numbers::inv_pi; ///< 1/pi
    inline constexpr f64 INV_SQRT_PI = std::numbers::inv_sqrtpi; ///< 1/sqrt(pi)
    inline constexpr f64 LN_2 = std::numbers::ln2; ///< log_e 2
    inline constexpr f64 LN_10 = std::numbers::ln10; ///< log_e 10
    inline constexpr f64 SQRT_2 = std::numbers::sqrt2; ///< sqrt(2)
    inline constexpr f64 SQRT_3 = std::numbers::sqrt3; ///< sqrt(3)
    inline constexpr f64 INV_SQRT_3 = std::numbers::inv_sqrt3; ///< 1/sqrt(3)
    inline constexpr f64 E_GAMMA = std::numbers::egamma; ///< Euler-Mascheroni constant
    inline constexpr f64 PHI = std::numbers::phi; ///< Golden ratio (1+sqrt(5))/2

    template <FloatingPoint T>
    inline constexpr T E_VALUE = std::numbers::e_v<T>; ///< e
    template <FloatingPoint T>
    inline constexpr T LOG_2_E_VALUE = std::numbers::log2e_v<T>; ///< log_2 e
    template <FloatingPoint T>
    inline constexpr T LOG_10_E_VALUE = std::numbers::log10e_v<T>; ///< log_10 e
    template <FloatingPoint T>
    inline constexpr T PI_VALUE = std::numbers::pi_v<T>; ///< pi
    template <FloatingPoint T>
    inline constexpr T INV_PI_VALUE = std::numbers::inv_pi_v<T>; ///< 1/pi
    template <FloatingPoint T>
    inline constexpr T INV_SQRT_PI_VALUE = std::numbers::inv_sqrtpi_v<T>; ///< 1/sqrt(pi)
    template <FloatingPoint T>
    inline constexpr T LN_2_VALUE = std::numbers::ln2_v<T>; ///< log_e 2
    template <FloatingPoint T>
    inline constexpr T LN_10_VALUE = std::numbers::ln10_v<T>; ///< log_e 10
    template <FloatingPoint T>
    inline constexpr T SQRT_2_VALUE = std::numbers::sqrt2_v<T>; ///< sqrt(2)
    template <FloatingPoint T>
    inline constexpr T SQRT_3_VALUE = std::numbers::sqrt3_v<T>; ///< sqrt(3)
    template <FloatingPoint T>
    inline constexpr T INV_SQRT_3_VALUE = std::numbers::inv_sqrt3_v<T>; ///< 1/sqrt(3)
    template <FloatingPoint T>
    inline constexpr T E_GAMMA_VALUE = std::numbers::egamma_v<T>; ///< Euler-Mascheroni constant
    template <FloatingPoint T>
    inline constexpr T PHI_VALUE = std::numbers::phi_v<T>; ///< Golden ratio (1+sqrt(5))/2
}
