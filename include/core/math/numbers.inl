/**
 * @namespace core::math
 * @brief Wrapper namespace for standard library numerical constants (for compatibility).
 */
export namespace core::math {
    #if __has_include(<numbers>)
    class Numbers final {
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

    inline namespace numbers {
        inline constexpr double E = Numbers::E; ///< e
        inline constexpr double LOG_2_E = Numbers::LOG_2_E; ///< log_2 e
        inline constexpr double LOG_10_E = Numbers::LOG_10_E; ///< log_10 e
        inline constexpr double PI = Numbers::PI; ///< pi
        inline constexpr double INV_PI = Numbers::INV_PI; ///< 1/pi
        inline constexpr double INV_SQRT_PI = Numbers::INV_SQRT_PI; ///< 1/sqrt(pi)
        inline constexpr double LN_2 = Numbers::LN_2; ///< log_e 2
        inline constexpr double LN_10 = Numbers::LN_10; ///< log_e 10
        inline constexpr double SQRT_2 = Numbers::SQRT_2; ///< sqrt(2)
        inline constexpr double SQRT_3 = Numbers::SQRT_3; ///< sqrt(3)
        inline constexpr double INV_SQRT_3 = Numbers::INV_SQRT_3; ///< 1/sqrt(3)
        inline constexpr double E_GAMMA = Numbers::E_GAMMA; ///< Euler-Mascheroni constant
        inline constexpr double PHI = Numbers::PHI; ///< Golden ratio (1+sqrt(5))/2
    }
    #endif
}