/**
 * @file cmath.cppm
 * @module core:math.cmath
 * @brief Module file for standard library C math operations.
 *
 * This file contains the implementation of the C math operations in the standard library.
 */

module;

#include <cmath>
#include <cstdlib>

export module core:math.cmath;

/**
 * @internal
 * @namespace _detail::core::math
 * @brief Wrapper namespace for standard library mathematical operations.
 * Detail namespace - not to be exported for external use
 */
namespace _detail::core::math {
    constexpr auto HugeVal = HUGE_VAL;
    constexpr auto HugeValF = HUGE_VALF;
    constexpr auto HugeValL = HUGE_VALL;
    constexpr auto Infinity = INFINITY;
    constexpr auto NaN = NAN;
    constexpr auto MathErrHandling = math_errhandling;
    constexpr auto MathErrNo = MATH_ERRNO;
    constexpr auto MathErrExcept = MATH_ERREXCEPT;
    constexpr auto FpNormal = FP_NORMAL;
    constexpr auto FpSubnormal = FP_SUBNORMAL;
    constexpr auto FpZero = FP_ZERO;
    constexpr auto FpInfinite = FP_INFINITE;
    constexpr auto FpNaN = FP_NAN;
}

#undef HUGE_VAL
#undef HUGE_VALF
#undef HUGE_VALL
#undef INFINITY
#undef NAN
#undef math_errhandling
#undef MATH_ERRNO
#undef MATH_ERREXCEPT
#undef FP_NORMAL 
#undef FP_SUBNORMAL
#undef FP_ZERO 
#undef FP_INFINITE
#undef FP_NAN

/**
 * @namespace core::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace core::math {
    constexpr auto HUGE_VAL = _detail::core::math::HugeVal;
    constexpr auto HUGE_VALF = _detail::core::math::HugeValF;
    constexpr auto HUGE_VALL = _detail::core::math::HugeValL;
    constexpr auto INFINITY = _detail::core::math::Infinity;
    constexpr auto NAN = _detail::core::math::NaN;
    constexpr auto MATH_ERRHANDLING = _detail::core::math::MathErrHandling;
    constexpr auto MATH_ERRNO = _detail::core::math::MathErrNo;
    constexpr auto MATH_ERREXCEPT = _detail::core::math::MathErrExcept;
    constexpr auto FP_NORMAL = _detail::core::math::FpNormal;
    constexpr auto FP_SUBNORMAL = _detail::core::math::FpSubnormal;
    constexpr auto FP_ZERO = _detail::core::math::FpZero;
    constexpr auto FP_INFINITE = _detail::core::math::FpInfinite;
    constexpr auto FP_NAN = _detail::core::math::FpNaN;

    // Absolute value
    using std::abs;
    using std::fabs;
    using std::fabsf;
    using std::fabsl;

    using std::labs;
    using std::llabs;

    // Quotient and remainder of integer division
    using std::div;
    using std::ldiv;
    using std::lldiv;

    // Remainder of floating point division
    using std::fmod;
    using std::fmodf;
    using std::fmodl;

    // Signed remainder of division
    using std::remainder;
    using std::remainderf;
    using std::remainderl;

    // Signed remainder and last three bits of division
    using std::remquo;
    using std::remquof;
    using std::remquol;

    // Fused multiply-add
    using std::fma;
    using std::fmaf;
    using std::fmal;

    // Maximum
    using std::fmax;
    using std::fmaxf;
    using std::fmaxl;

    // Minimum
    using std::fmin;
    using std::fminf;
    using std::fminl;

    // Positive difference
    using std::fdim;
    using std::fdimf;
    using std::fdiml;

    // Not-a-number
    using std::nan;
    using std::nanf;
    using std::nanl;

    // Linear interpolation
    using std::lerp;

    // Exponent base e
    using std::exp;
    using std::expf;
    using std::expl;

    // Exponent base 2
    using std::exp2;
    using std::exp2f;
    using std::exp2l;

    // Exponent base e, minus 1
    using std::expm1;
    using std::expm1f;
    using std::expm1l;

    // Natural logarithm (base e)
    using std::log;
    using std::logf;
    using std::logl;

    // Logarithm base 10
    using std::log10;
    using std::log10f;
    using std::log10l;

    // Logarithm base 2
    using std::log2;
    using std::log2f;
    using std::log2l;

    // Natural logarithm (base e), of 1 plus x
    using std::log1p;
    using std::log1pf;
    using std::log1pl;

    // Power
    using std::pow;
    using std::powf;
    using std::powl;

    // Square root
    using std::sqrt;
    using std::sqrtf;
    using std::sqrtl;

    // Cube root
    using std::cbrt;
    using std::cbrtf;
    using std::cbrtl;

    // Hypotenuse
    using std::hypot;
    using std::hypotf;
    using std::hypotl;

    // Sine
    using std::sin;
    using std::sinf;
    using std::sinl;

    // Cosine
    using std::cos;
    using std::cosf;
    using std::cosl;

    // Tangent
    using std::tan;
    using std::tanf;
    using std::tanl;

    // Arcsine
    using std::asin;
    using std::asinf;
    using std::asinl;

    // Arccosine
    using std::acos;
    using std::acosf;
    using std::acosl;

    // Arctangent
    using std::atan;
    using std::atanf;
    using std::atanl;

    // atan2 (Arc tangent with quadrants)
    using std::atan2;
    using std::atan2f;
    using std::atan2l;

    // Hyperbolic sine
    using std::sinh;
    using std::sinhf;
    using std::sinhl;

    // Hyperbolic cosine
    using std::cosh;
    using std::coshf;
    using std::coshl;

    // Hyperbolic tangent
    using std::tanh;
    using std::tanhf;
    using std::tanhl;

    // Inverse hyperbolic sine
    using std::asinh;
    using std::asinhf;
    using std::asinhl;

    // Inverse hyperbolic cosine
    using std::acosh;
    using std::acoshf;
    using std::acoshl;

    // Inverse hyperbolic tangent
    using std::atanh;
    using std::atanhf;
    using std::atanhl;

    // Error function
    using std::erf;
    using std::erff;
    using std::erfl;

    // Complementary error function
    using std::erfc;
    using std::erfcf;
    using std::erfcl;

    // Gamma function
    using std::tgamma;
    using std::tgammaf;
    using std::tgammal;

    // Natural logarithm of gamma function
    using std::lgamma;
    using std::lgammaf;
    using std::lgammal;

    // Ceiling function
    using std::ceil;
    using std::ceilf;
    using std::ceill;

    // Floor function
    using std::floor;
    using std::floorf;
    using std::floorl;

    // Truncate function
    using std::trunc;
    using std::truncf;
    using std::truncl;

    // Round
    using std::round;
    using std::roundf;
    using std::roundl;
    using std::lround;
    using std::lroundf;
    using std::lroundl;
    using std::llround;
    using std::llroundf;
    using std::llroundl;

    // Nearest integer
    using std::nearbyint;
    using std::nearbyintf;
    using std::nearbyintl;

    // Rounding nearest integer
    using std::rint;
    using std::rintf;
    using std::rintl;
    using std::lrint;
    using std::lrintf;
    using std::lrintl;
    using std::llrint;
    using std::llrintf;
    using std::llrintl;

    // Decompose number into significand/base-2 exponent
    using std::frexp;
    using std::frexpf;
    using std::frexpl;

    // Multiply number by power of 2
    using std::ldexp;
    using std::ldexpf;
    using std::ldexpl;

    // Decompose number into integer/fraction
    using std::modf;
    using std::modff;
    using std::modfl;

    // Multiply number by power of FLT_RADIX
    using std::scalbn;
    using std::scalbnf;
    using std::scalbnl;
    using std::scalbln;
    using std::scalblnf;
    using std::scalblnl;

    // Extract exponent
    using std::ilogb;
    using std::ilogbf;
    using std::ilogbl;

    // Extract exponent
    using std::logb;
    using std::logbf;
    using std::logbl;

    // Next representable floating-point value
    using std::nextafter;
    using std::nextafterf;
    using std::nextafterl;
    using std::nexttoward;
    using std::nexttowardf;
    using std::nexttowardl;

    // Copy sign
    using std::copysign;
    using std::copysignf;
    using std::copysignl;

    // Classify floating point number
    using std::fpclassify;

    // Check if finite
    using std::isfinite;

    // Check if infinite
    using std::isinf;

    // Check if NaN
    using std::isnan;

    // Check if normal
    using std::isnormal;

    // Check if negative
    using std::signbit;

    // Check if greater
    using std::isgreater;

    // Check if greater/equal
    using std::isgreaterequal;

    // Check if less
    using std::isless;

    // Check if less/equal
    using std::islessequal;

    // Check if less/greater
    using std::islessgreater;

    // Check if unordered
    using std::isunordered;

    // Associated Laguerre polynomials
    using std::assoc_laguerre;
    using std::assoc_laguerref;
    using std::assoc_laguerrel;

    // Associated Legendre polynomials
    using std::assoc_legendre;
    using std::assoc_legendref;
    using std::assoc_legendrel;

    // Beta function
    using std::beta;
    using std::betaf;
    using std::betal;

    // Complete elliptic integral (first kind)
    using std::comp_ellint_1;
    using std::comp_ellint_1f;
    using std::comp_ellint_1l;

    // Complete elliptic integral (second kind)
    using std::comp_ellint_2;
    using std::comp_ellint_2f;
    using std::comp_ellint_2l;

    // Complete elliptic integral (third kind)
    using std::comp_ellint_3;
    using std::comp_ellint_3f;
    using std::comp_ellint_3l;

    // Regular modified cylindrical Bessel functions
    using std::cyl_bessel_i;
    using std::cyl_bessel_if;
    using std::cyl_bessel_il;

    // Cylindrical Bessel functions (first kind)
    using std::cyl_bessel_j;
    using std::cyl_bessel_jf;
    using std::cyl_bessel_jl;

    // Irregular modified cylindrical Bessel functions
    using std::cyl_bessel_k;
    using std::cyl_bessel_kf;
    using std::cyl_bessel_kl;

    // Cylindrical Neumann functions
    using std::cyl_neumann;
    using std::cyl_neumannf;
    using std::cyl_neumannl;

    // Incomplete elliptic integral (first kind)
    using std::ellint_1;
    using std::ellint_1f;
    using std::ellint_1l;

    // Incomplete elliptic integral (second kind)
    using std::ellint_2;
    using std::ellint_2f;
    using std::ellint_2l;

    // Incomplete elliptic integral (third kind)
    using std::ellint_3;
    using std::ellint_3f;
    using std::ellint_3l;

    // Exponential integral
    using std::expint;
    using std::expintf;
    using std::expintl;

    // Hermite polynomials
    using std::hermite;
    using std::hermitef;
    using std::hermitel;

    // Legendre polynomials
    using std::legendre;
    using std::legendref;
    using std::legendrel;

    // Laguerre polynomials
    using std::laguerre;
    using std::laguerref;
    using std::laguerrel;

    // Riemann zeta function
    using std::riemann_zeta;
    using std::riemann_zetaf;
    using std::riemann_zetal;

    // Spherical Bessel functions (first kind)
    using std::sph_bessel;
    using std::sph_besself;
    using std::sph_bessell;

    // Spherical associated Legendre functions
    using std::sph_legendre;
    using std::sph_legendref;
    using std::sph_legendrel;

    // Spherical Neumann function
    using std::sph_neumann;
    using std::sph_neumannf;
    using std::sph_neumannl;
}
