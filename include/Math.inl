#pragma once

class Math final {
public:
    Math() = delete;

    static constexpr double E = ::core::math::numbers::E; ///< e
    static constexpr double LOG_2_E = ::core::math::numbers::LOG_2_E; ///< log_2 e
    static constexpr double LOG_10_E = ::core::math::numbers::LOG_10_E; ///< log_10 e
    static constexpr double PI = ::core::math::numbers::PI; ///< pi
    static constexpr double INV_PI = ::core::math::numbers::INV_PI; ///< 1/pi
    static constexpr double INV_SQRT_PI = ::core::math::numbers::INV_SQRT_PI; ///< 1/sqrt(pi)
    static constexpr double LN_2 = ::core::math::numbers::LN_2; ///< log_e 2
    static constexpr double LN_10 = ::core::math::numbers::LN_10; ///< log_e 10
    static constexpr double SQRT_2 = ::core::math::numbers::SQRT_2; ///< sqrt(2)
    static constexpr double SQRT_3 = ::core::math::numbers::SQRT_3; ///< sqrt(3)
    static constexpr double INV_SQRT_3 = ::core::math::numbers::INV_SQRT_3; ///< 1/sqrt(3)
    static constexpr double E_GAMMA = ::core::math::numbers::E_GAMMA; ///< Euler-Mascheroni constant
    static constexpr double PHI = ::core::math::numbers::PHI; ///< Golden ratio (1+sqrt(5))/2

    #ifndef STDLIBX_NO_STD_CORE_FEATURES
    static constexpr auto HUGE_VAL = ::stdx::math::HUGE_VAL;
    static constexpr auto HUGE_VALF = ::stdx::math::HUGE_VALF;
    static constexpr auto HUGE_VALL = ::stdx::math::HUGE_VALL;
    static constexpr auto INFINITY = ::stdx::math::INFINITY;
    static constexpr auto NAN = ::stdx::math::NAN;
    static constexpr auto MATH_ERRHANDLING = ::stdx::math::MATH_ERRHANDLING;
    static constexpr auto MATH_ERRNO = ::stdx::math::MATH_ERRNO;
    static constexpr auto MATH_ERREXCEPT = ::stdx::math::MATH_ERREXCEPT;
    static constexpr auto FP_NORMAL = ::stdx::math::FP_NORMAL;
    static constexpr auto FP_SUBNORMAL = ::stdx::math::FP_SUBNORMAL;
    static constexpr auto FP_ZERO = ::stdx::math::FP_ZERO;
    static constexpr auto FP_INFINITE = ::stdx::math::FP_INFINITE;
    static constexpr auto FP_NAN = ::stdx::math::FP_NAN;

    static constexpr auto RAND_MAX = ::stdx::random::RAND_MAX;

    static constexpr auto FE_ALL_EXCEPT = ::stdx::math::FE_ALL_EXCEPT;
    static constexpr auto FE_DIVBYZERO = ::stdx::math::FE_DIVBYZERO;
    static constexpr auto FE_INEXACT = ::stdx::math::FE_INEXACT;
    static constexpr auto FE_INVALID = ::stdx::math::FE_INVALID;
    static constexpr auto FE_OVERFLOW = ::stdx::math::FE_OVERFLOW;
    static constexpr auto FE_UNDERFLOW = ::stdx::math::FE_UNDERFLOW;

    static constexpr auto FE_TONEAREST = ::stdx::math::FE_TONEAREST;
    static constexpr auto FE_DOWNWARD = ::stdx::math::FE_DOWNWARD;
    static constexpr auto FE_UPWARD = ::stdx::math::FE_UPWARD;
    static constexpr auto FE_TOWARDZERO = ::stdx::math::FE_TOWARDZERO;

    static inline const auto FE_DFL_ENV = ::stdx::math::FE_DFL_ENV;
    #endif

    // Basic operations

    [[nodiscard]]
    static constexpr Numeric auto abs(Numeric auto x) noexcept {
        using std::abs;
        return abs(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto fmod(Numeric auto x, Numeric auto y) noexcept {
        using std::fmod;
        return fmod(x, y);
    }

    [[nodiscard]]
    static constexpr Numeric auto remainder(Numeric auto x, Numeric auto y) noexcept {
        using std::remainder;
        return remainder(x, y);
    }

    [[nodiscard]]
    static constexpr Numeric auto remquo(Numeric auto x, Numeric auto y, i32& quo) noexcept {
        using std::remquo;
        return remquo(x, y, quo);
    }

    [[nodiscard]]
    static constexpr Numeric auto fma(Numeric auto x, Numeric auto y, Numeric auto z) noexcept {
        using std::fma;
        return fma(x, y, z);
    }

    [[nodiscard]]
    static constexpr Numeric auto fmax(Numeric auto x, Numeric auto y) noexcept {
        using std::fmax;
        return fmax(x, y);
    }

    [[nodiscard]]
    static constexpr Numeric auto fmin(Numeric auto x, Numeric auto y) noexcept {
        using std::fmin;
        return fmin(x, y);
    }

    [[nodiscard]]
    static constexpr Numeric auto fdim(Numeric auto x, Numeric auto y) noexcept {
        using std::fdim;
        return fdim(x, y);
    }

    [[nodiscard]]
    static constexpr i32 signum(Numeric auto x) noexcept {
        if (x > 0) {
            return 1;
        } else if (x < 0) {
            return -1;
        } else {
            return 0;
        }
    }

    // Linear interpolation

    [[nodiscard]]
    static constexpr Numeric auto lerp(Numeric auto a, Numeric auto b, Numeric auto t) noexcept {
        using std::lerp;
        return lerp(a, b, t);
    }

    // Minimum and maximum functions

    template <typename T>
    [[nodiscard]]
    static constexpr const T& max(const T& a, const T& b) noexcept {
        using std::max;
        return max(a, b);
    }

    template <typename T, typename Compare>
    [[nodiscard]]
    static constexpr const T& max(const T& a, const T& b, Compare c) noexcept {
        using std::max;
        return max(a, b, c);
    }

    template <typename T>
    [[nodiscard]]
    static constexpr T max(InitializerList<T> list) noexcept {
        using std::max;
        return max(list);
    }

    template <typename T, typename Compare>
    [[nodiscard]]
    static constexpr T max(InitializerList<T> list, Compare c) noexcept {
        using std::max;
        return max(list, c);
    }

    template <typename T>
    [[nodiscard]]
    static constexpr const T& min(const T& a, const T& b) noexcept {
        using std::min;
        return min(a, b);
    }

    template <typename T, typename Compare>
    [[nodiscard]]
    static constexpr const T& min(const T& a, const T& b, Compare c) noexcept {
        using std::min;
        return min(a, b, c);
    }

    template <typename T>
    [[nodiscard]]
    static constexpr T min(InitializerList<T> list) noexcept {
        using std::min;
        return min(list);
    }

    template <typename T, typename Compare>
    [[nodiscard]]
    static constexpr T min(InitializerList<T> list, Compare c) noexcept {
        using std::min;
        return min(list, c);
    }

    template <typename T>
    [[nodiscard]]
    static constexpr Pair<const T&, const T&> minmax(const T& a, const T& b) noexcept {
        using std::minmax;
        return minmax(a, b);
    }

    template <typename T, typename Compare>
    static constexpr Pair<const T&, const T&> minmax(const T& a, const T& b, Compare c) noexcept {
        using std::minmax;
        return minmax(a, b, c);
    }

    template <typename T>
    static constexpr Pair<T, T> minmax(InitializerList<T> list) noexcept {
        using std::minmax;
        return minmax(list);
    }

    template <typename T, typename Compare>
    static constexpr Pair<T, T> minmax(InitializerList<T> list, Compare c) noexcept {
        using std::minmax;
        return minmax(list, c);
    }

    template <typename T>
    [[nodiscard]]
    static constexpr const T& clamp(const T& x, const T& low, const T& high) noexcept {
        using std::clamp;
        return clamp(x, low, high);
    }

    template <typename T, typename Compare>
    [[nodiscard]]
    static constexpr const T& clamp(const T& x, const T& low, const T& high, Compare c) noexcept {
        using std::clamp;
        return clamp(x, low, high, c);
    }

    // Bit manipulation functions

    [[nodiscard]]
    static constexpr Integral auto byteswap(Integral auto n) noexcept {
        using std::byteswap;
        return byteswap(n);
    }

    [[nodiscard]]
    static constexpr bool has_single_bit(Integral auto n) noexcept {
        using std::has_single_bit;
        return has_single_bit(n);
    }

    [[nodiscard]]
    static constexpr Integral auto bit_ceil(Integral auto n) noexcept {
        using std::bit_ceil;
        return bit_ceil(n);
    }

    [[nodiscard]]
    static constexpr Integral auto bit_floor(Integral auto n) noexcept {
        using std::bit_floor;
        return bit_floor(n);
    }

    [[nodiscard]]
    static constexpr i32 bit_width(Integral auto n) noexcept {
        using std::bit_width;
        return bit_width(n);
    }

    [[nodiscard]]
    static constexpr Integral auto rotate_left(Integral auto x, i32 s) noexcept {
        using std::rotl;
        return rotl(x, s);
    }

    [[nodiscard]]
    static constexpr Integral auto rotate_right(Integral auto x, i32 s) noexcept {
        using std::rotr;
        return rotr(x, s);
    }

    [[nodiscard]]
    static constexpr i32 count_left_zero(Integral auto n) noexcept {
        using std::countl_zero;
        return countl_zero(n);
    }

    [[nodiscard]]
    static constexpr i32 count_left_one(Integral auto n) noexcept {
        using std::countl_one;
        return countl_one(n);
    }

    [[nodiscard]]
    static constexpr i32 count_right_zero(Integral auto n) noexcept {
        using std::countr_zero;
        return countr_zero(n);
    }

    [[nodiscard]]
    static constexpr i32 count_right_one(Integral auto n) noexcept {
        using std::countr_one;
        return countr_one(n);
    }

    [[nodiscard]]
    static constexpr i32 count_pop(Integral auto n) noexcept {
        using std::popcount;
        return popcount(n);
    }

    // Exponential and logarithmic functions

    [[nodiscard]]
    static constexpr Numeric auto exp(Numeric auto x) noexcept {
        using std::exp;
        return exp(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto exp2(Numeric auto x) noexcept {
        using std::exp2;
        return exp2(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto expm1(Numeric auto x) noexcept {
        using std::expm1;
        return expm1(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto log(Numeric auto x) noexcept {
        using std::log;
        return log(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto log2(Numeric auto x) noexcept {
        using std::log2;
        return log2(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto log10(Numeric auto x) noexcept {
        using std::log10;
        return log10(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto log1p(Numeric auto x) noexcept {
        using std::log1p;
        return log1p(x);
    }

    // Power functions

    [[nodiscard]]
    static constexpr Numeric auto pow(Numeric auto x, Numeric auto y) noexcept {
        using std::pow;
        return pow(x, y);
    }

    [[nodiscard]]
    static constexpr Numeric auto sqrt(Numeric auto x) noexcept {
        using std::sqrt;
        return sqrt(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto cbrt(Numeric auto x) noexcept {
        using std::cbrt;
        return cbrt(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto hypot(Numeric auto x, Numeric auto y) noexcept {
        using std::hypot;
        return hypot(x, y);
    }

    // Trigonometric functions

    [[nodiscard]]
    static constexpr Numeric auto sin(Numeric auto x) noexcept {
        using std::sin;
        return sin(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto cos(Numeric auto x) noexcept {
        using std::cos;
        return cos(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto tan(Numeric auto x) noexcept {
        using std::tan;
        return tan(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto asin(Numeric auto x) noexcept {
        using std::asin;
        return asin(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto acos(Numeric auto x) noexcept {
        using std::acos;
        return acos(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto atan(Numeric auto x) noexcept {
        using std::atan;
        return atan(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto atan2(Numeric auto y, Numeric auto x) noexcept {
        using std::atan2;
        return atan2(y, x);
    }

    [[nodiscard]]
    static constexpr Numeric auto to_radians(Numeric auto degrees) noexcept {
        return degrees * (PI / 180);
    }

    [[nodiscard]]
    static constexpr Numeric auto to_degrees(Numeric auto radians) noexcept {
        return radians * (180 / PI);
    }

    // Hyperbolic functions

    [[nodiscard]]
    static constexpr Numeric auto sinh(Numeric auto x) noexcept {
        using std::sinh;
        return sinh(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto cosh(Numeric auto x) noexcept {
        using std::cosh;
        return cosh(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto tanh(Numeric auto x) noexcept {
        using std::tanh;
        return tanh(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto asinh(Numeric auto x) noexcept {
        using std::asinh;
        return asinh(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto acosh(Numeric auto x) noexcept {
        using std::acosh;
        return acosh(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto atanh(Numeric auto x) noexcept {
        using std::atanh;
        return atanh(x);
    }

    // Error and gamma functions

    [[nodiscard]]
    static constexpr Numeric auto erf(Numeric auto x) noexcept {
        using std::erf;
        return erf(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto erfc(Numeric auto x) noexcept {
        using std::erfc;
        return erfc(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto tgamma(Numeric auto x) noexcept {
        using std::tgamma;
        return tgamma(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto lgamma(Numeric auto x) noexcept {
        using std::lgamma;
        return lgamma(x);
    }

    // Nearest integer functions

    [[nodiscard]]
    static constexpr Numeric auto ceil(Numeric auto x) noexcept {
        using std::ceil;
        return ceil(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto floor(Numeric auto x) noexcept {
        using std::floor;
        return floor(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto trunc(Numeric auto x) noexcept {
        using std::trunc;
        return trunc(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto round(Numeric auto x) noexcept {
        using std::round;
        return round(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto nearbyint(Numeric auto x) noexcept {
        using std::nearbyint;
        return nearbyint(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto rint(Numeric auto x) noexcept {
        using std::rint;
        return rint(x);
    }

    // Floating-point manipulation functions

    [[nodiscard]]
    static constexpr Numeric auto frexp(Numeric auto x, i32* exp) noexcept {
        using std::frexp;
        return frexp(x, exp);
    }

    [[nodiscard]]
    static constexpr Numeric auto ldexp(Numeric auto x, i32 exp) noexcept {
        using std::ldexp;
        return ldexp(x, exp);
    }

    [[nodiscard]]
    static constexpr Numeric auto modf(Numeric auto x, FloatingPoint auto* ipart) noexcept {
        using std::modf;
        return modf(x, ipart);
    }

    [[nodiscard]]
    static constexpr Numeric auto scalbn(Numeric auto x, i32 exp) noexcept {
        using std::scalbn;
        return scalbn(x, exp);
    }

    [[nodiscard]]
    static constexpr Numeric auto scalbln(Numeric auto x, long exp) noexcept {
        using std::scalbln;
        return scalbln(x, exp);
    }

    [[nodiscard]]
    static constexpr Numeric auto logb(Numeric auto x) noexcept {
        using std::logb;
        return logb(x);
    }

    [[nodiscard]]
    static constexpr i32 ilogb(Numeric auto x) noexcept {
        using std::ilogb;
        return ilogb(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto nextafter(Numeric auto from, FloatingPoint auto to) noexcept {
        using std::nextafter;
        return nextafter(from, to);
    }

    [[nodiscard]]
    static constexpr Numeric auto nexttoward(Numeric auto from, FloatingPoint auto to) noexcept {
        using std::nexttoward;
        return nexttoward(from, to);
    }

    [[nodiscard]]
    static constexpr Numeric auto copysign(Numeric auto mag, Numeric auto sgn) noexcept {
        using std::copysign;
        return copysign(mag, sgn);
    }

    // Classification and comparison functions

    [[nodiscard]]
    static constexpr i32 floating_point_classify(Numeric auto x) noexcept {
        using std::fpclassify;
        return fpclassify(x);
    }

    [[nodiscard]]
    static constexpr bool is_finite(Numeric auto x) noexcept {
        using std::isfinite;
        return isfinite(x);
    }

    [[nodiscard]]
    static constexpr bool is_infinite(Numeric auto x) noexcept {
        using std::isinf;
        return isinf(x);
    }

    [[nodiscard]]
    static constexpr bool is_nan(Numeric auto x) noexcept {
        using std::isnan;
        return isnan(x);
    }

    [[nodiscard]]
    static constexpr bool is_normal(Numeric auto x) noexcept {
        using std::isnormal;
        return isnormal(x);
    }

    [[nodiscard]]
    static constexpr bool is_negative(Numeric auto x) noexcept {
        using std::signbit;
        return signbit(x);
    }

    [[nodiscard]]
    static constexpr bool is_greater(Numeric auto x, Numeric auto y) noexcept {
        using std::isgreater;
        return isgreater(x, y);
    }

    [[nodiscard]]
    static constexpr bool is_greater_equal(Numeric auto x, Numeric auto y) noexcept {
        using std::isgreaterequal;
        return isgreaterequal(x, y);
    }

    [[nodiscard]]
    static constexpr bool is_less(Numeric auto x, Numeric auto y) noexcept {
        using std::isless;
        return isless(x, y);
    }

    [[nodiscard]]
    static constexpr bool is_less_equal(Numeric auto x, Numeric auto y) noexcept {
        using std::islessequal;
        return islessequal(x, y);
    }

    [[nodiscard]]
    static constexpr bool is_less_greater(Numeric auto x, Numeric auto y) noexcept {
        using std::islessgreater;
        return islessgreater(x, y);
    }

    [[nodiscard]]
    static constexpr bool is_unordered(Numeric auto x, Numeric auto y) noexcept {
        using std::isunordered;
        return isunordered(x, y);
    }

    // Special functions

    [[nodiscard]]
    static constexpr Numeric auto assoc_laguerre(u32 n, u32 m, Numeric auto x) noexcept {
        using std::assoc_laguerre;
        return assoc_laguerre(n, m, x);
    }

    [[nodiscard]]
    static constexpr Numeric auto assoc_legendre(u32 l, u32 m, Numeric auto x) noexcept {
        using std::assoc_legendre;
        return assoc_legendre(l, m, x);
    }

    [[nodiscard]]
    static constexpr Numeric auto beta(Numeric auto x, Numeric auto y) noexcept {
        using std::beta;
        return beta(x, y);
    }

    [[nodiscard]]
    static constexpr Numeric auto comp_ellint_1(Numeric auto k) noexcept {
        using std::comp_ellint_1;
        return comp_ellint_1(k);
    }

    [[nodiscard]]
    static constexpr Numeric auto comp_ellint_2(Numeric auto k) noexcept {
        using std::comp_ellint_2;
        return comp_ellint_2(k);
    }

    [[nodiscard]]
    static constexpr Numeric auto comp_ellint_3(Numeric auto k, Numeric auto n) noexcept {
        using std::comp_ellint_3;
        return comp_ellint_3(k, n);
    }

    [[nodiscard]]
    static constexpr Numeric auto cyl_bessel_i(Numeric auto v, Numeric auto x) noexcept {
        using std::cyl_bessel_i;
        return cyl_bessel_i(v, x);
    }

    [[nodiscard]]
    static constexpr Numeric auto cyl_bessel_j(Numeric auto v, Numeric auto x) noexcept {
        using std::cyl_bessel_j;
        return cyl_bessel_j(v, x);
    }

    [[nodiscard]]
    static constexpr Numeric auto cyl_bessel_k(Numeric auto v, Numeric auto x) noexcept {
        using std::cyl_bessel_k;
        return cyl_bessel_k(v, x);
    }

    [[nodiscard]]
    static constexpr Numeric auto cyl_neumann(Numeric auto v, Numeric auto x) noexcept {
        using std::cyl_neumann;
        return cyl_neumann(v, x);
    }

    [[nodiscard]]
    static constexpr Numeric auto ellint_1(Numeric auto x, Numeric auto k) noexcept {
        using std::ellint_1;
        return ellint_1(x, k);
    }

    [[nodiscard]]
    static constexpr Numeric auto ellint_2(Numeric auto x, Numeric auto k) noexcept {
        using std::ellint_2;
        return ellint_2(x, k);
    }

    [[nodiscard]]
    static constexpr Numeric auto ellint_3(Numeric auto x, Numeric auto k, Numeric auto n) noexcept {
        using std::ellint_3;
        return ellint_3(x, k, n);
    }

    [[nodiscard]]
    static constexpr Numeric auto expint(Numeric auto x) noexcept {
        using std::expint;
        return expint(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto hermite(Numeric auto n, Numeric auto x) noexcept {
        using std::hermite;
        return hermite(n, x);
    }

    [[nodiscard]]
    static constexpr Numeric auto legendre(u32 n, Numeric auto x) noexcept {
        using std::legendre;
        return legendre(n, x);
    }

    [[nodiscard]]
    static constexpr Numeric auto laguerre(u32 n, Numeric auto x) noexcept {
        using std::laguerre;
        return laguerre(n, x);
    }

    [[nodiscard]]
    static constexpr Numeric auto riemann_zeta(Numeric auto x) noexcept {
        using std::riemann_zeta;
        return riemann_zeta(x);
    }

    [[nodiscard]]
    static constexpr Numeric auto sph_bessel(Numeric auto n, Numeric auto x) noexcept {
        using std::sph_bessel;
        return sph_bessel(n, x);
    }

    [[nodiscard]]
    static constexpr Numeric auto sph_legendre(u32 n, u32 m, Numeric auto x) noexcept {
        using std::sph_legendre;
        return sph_legendre(n, m, x);
    }

    [[nodiscard]]
    static constexpr Numeric auto sph_neumann(Numeric auto n, Numeric auto x) noexcept {
        using std::sph_neumann;
        return sph_neumann(n, x);
    }

    // Complex number functions

    template <Numeric T>
    [[nodiscard]]
    static constexpr T real(const Complex<T>& z) noexcept {
        using std::real;
        return real(z);
    }

    [[nodiscard]]
    static constexpr Numeric auto real(Numeric auto x) noexcept {
        using std::real;
        return real(x);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr T imag(const Complex<T>& z) noexcept {
        using std::imag;
        return imag(z);
    }

    [[nodiscard]]
    static constexpr Numeric auto imag(Numeric auto x) noexcept {
        using std::imag;
        return imag(x);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr T abs(const Complex<T>& z) noexcept {
        using std::abs;
        return abs(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr T arg(const Complex<T>& z) noexcept {
        using std::arg;
        return arg(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr T norm(const Complex<T>& z) noexcept {
        using std::norm;
        return norm(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> conj(const Complex<T>& z) noexcept {
        using std::conj;
        return conj(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> proj(const Complex<T>& z) noexcept {
        using std::proj;
        return proj(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> polar(const T& r, const T& theta = T()) noexcept {
        using std::polar;
        return polar(r, theta);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> exp(const Complex<T>& z) noexcept {
        using std::exp;
        return exp(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> log(const Complex<T>& z) noexcept {
        using std::log;
        return log(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> log10(const Complex<T>& z) noexcept {
        using std::log10;
        return log10(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> pow(const Complex<T>& x, const Complex<T>& y) noexcept {
        using std::pow;
        return pow(x, y);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> pow(const Complex<T>& x, const T& y) noexcept {
        using std::pow;
        return pow(x, y);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> pow(const T& x, const Complex<T>& y) noexcept {
        using std::pow;
        return pow(x, y);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> sqrt(const Complex<T>& z) noexcept {
        using std::sqrt;
        return sqrt(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> sin(const Complex<T>& z) noexcept {
        using std::sin;
        return sin(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> cos(const Complex<T>& z) noexcept {
        using std::cos;
        return cos(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> tan(const Complex<T>& z) noexcept {
        using std::tan;
        return tan(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> asin(const Complex<T>& z) noexcept {
        using std::asin;
        return asin(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> acos(const Complex<T>& z) noexcept {
        using std::acos;
        return acos(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> atan(const Complex<T>& z) noexcept {
        using std::atan;
        return atan(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> sinh(const Complex<T>& z) noexcept {
        using std::sinh;
        return sinh(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> cosh(const Complex<T>& z) noexcept {
        using std::cosh;
        return cosh(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> tanh(const Complex<T>& z) noexcept {
        using std::tanh;
        return tanh(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> asinh(const Complex<T>& z) noexcept {
        using std::asinh;
        return asinh(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> acosh(const Complex<T>& z) noexcept {
        using std::acosh;
        return acosh(z);
    }

    template <Numeric T>
    [[nodiscard]]
    static constexpr Complex<T> atanh(const Complex<T>& z) noexcept {
        using std::atanh;
        return atanh(z);
    }

    #ifndef STDLIBX_NO_STD_CORE_FEATURES
    [[nodiscard]]
    static f64 random() {
        static thread_local MersenneTwister rng{RandomDevice{}()};
        static thread_local UniformRealDistribution<double> dist(0.0, 1.0);
        return dist(rng);
    }
    #endif
};
