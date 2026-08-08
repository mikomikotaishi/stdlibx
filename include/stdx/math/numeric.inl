#pragma once

/**
 * @namespace stdx::math
 * @brief Standard library mathematical operations.
 */
export namespace stdx::math {
    using std::iota;
    using std::accumulate;
    using std::reduce;
    using std::transform_reduce;
    using std::inner_product;
    using std::adjacent_difference;
    using std::partial_sum;
    using std::inclusive_scan;
    using std::exclusive_scan;
    using std::transform_inclusive_scan;
    using std::transform_exclusive_scan;
    using std::gcd;
    using std::lcm;
    using std::midpoint;

    /**
     * The paper's names, on both standard libraries.
     *
     * libc++ spells them this way already. libstdc++ 16 spells them
     * saturating_add and friends and has no add_sat at all - not in its headers,
     * not in its std module - while still defining
     * __cpp_lib_saturation_arithmetic, so the feature macro promises one of two
     * spellings rather than either one in particular.
     *
     * Forwarded rather than re-exported for that reason: a `using std::add_sat`
     * would make the name stdx::math offers depend on which library it was built
     * against, which is the one thing a portability layer must not do. The
     * wrappers cost nothing once inlined.
     */
    #ifdef __cpp_lib_saturation_arithmetic
    template <typename T>
    [[nodiscard]]
    constexpr T add_sat(T a, T b) noexcept {
        #ifdef __GLIBCXX__
        return std::saturating_add(a, b);
        #else
        return std::add_sat(a, b);
        #endif
    }

    template <typename T>
    [[nodiscard]]
    constexpr T sub_sat(T a, T b) noexcept {
        #ifdef __GLIBCXX__
        return std::saturating_sub(a, b);
        #else
        return std::sub_sat(a, b);
        #endif
    }

    template <typename T>
    [[nodiscard]]
    constexpr T mul_sat(T a, T b) noexcept {
        #ifdef __GLIBCXX__
        return std::saturating_mul(a, b);
        #else
        return std::mul_sat(a, b);
        #endif
    }

    template <typename T>
    [[nodiscard]]
    constexpr T div_sat(T a, T b) noexcept {
        #ifdef __GLIBCXX__
        return std::saturating_div(a, b);
        #else
        return std::div_sat(a, b);
        #endif
    }

    template <typename T, typename U>
    [[nodiscard]]
    constexpr T saturate_cast(U value) noexcept {
        #ifdef __GLIBCXX__
        return std::saturating_cast<T>(value);
        #else
        return std::saturate_cast<T>(value);
        #endif
    }
    #endif
}
