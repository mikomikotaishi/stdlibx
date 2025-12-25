/**
 * @file cmath.cppm
 * @module std:math.cmath
 * @brief Module file for standard library C math operations.
 *
 * This file contains the implementation of the C math operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:math.cmath;
#else
export module stdlib:math.cmath;
#endif

import core;

/**
 * @namespace std::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::math {
#else 
export namespace stdlib::math {
#endif
    using core::math::HUGE_VAL;
    using core::math::HUGE_VALF;
    using core::math::HUGE_VALL;
    using core::math::INFINITY;
    using core::math::NAN;
    using core::math::MATH_ERRHANDLING;
    using core::math::MATH_ERRNO;
    using core::math::MATH_ERREXCEPT;
    using core::math::FP_NORMAL;
    using core::math::FP_SUBNORMAL;
    using core::math::FP_ZERO;
    using core::math::FP_INFINITE;
    using core::math::FP_NAN;

    // Absolute value
    using core::math::abs;
    using core::math::fabs;
    using core::math::fabsf;
    using core::math::fabsl;

    // Remainder of floating point division
    using core::math::fmod;
    using core::math::fmodf;
    using core::math::fmodl;

    // Signed remainder of division
    using core::math::remainder;
    using core::math::remainderf;
    using core::math::remainderl;

    // Signed remainder and last three bits of division
    using core::math::remquo;
    using core::math::remquof;
    using core::math::remquol;

    // Fused multiply-add
    using core::math::fma;
    using core::math::fmaf;
    using core::math::fmal;

    // Maximum
    using core::math::fmax;
    using core::math::fmaxf;
    using core::math::fmaxl;

    // Minimum
    using core::math::fmin;
    using core::math::fminf;
    using core::math::fminl;

    // Positive difference
    using core::math::fdim;
    using core::math::fdimf;
    using core::math::fdiml;

    // Not-a-number
    using core::math::nan;
    using core::math::nanf;
    using core::math::nanl;

    // Linear interpolation
    using core::math::lerp;

    // Exponent base e
    using core::math::exp;
    using core::math::expf;
    using core::math::expl;

    // Exponent base 2
    using core::math::exp2;
    using core::math::exp2f;
    using core::math::exp2l;

    // Exponent base e, minus 1
    using core::math::expm1;
    using core::math::expm1f;
    using core::math::expm1l;

    // Natural logarithm (base e)
    using core::math::log;
    using core::math::logf;
    using core::math::logl;

    // Logarithm base 10
    using core::math::log10;
    using core::math::log10f;
    using core::math::log10l;

    // Logarithm base 2
    using core::math::log2;
    using core::math::log2f;
    using core::math::log2l;

    // Natural logarithm (base e), of 1 plus x
    using core::math::log1p;
    using core::math::log1pf;
    using core::math::log1pl;

    // Power
    using core::math::pow;
    using core::math::powf;
    using core::math::powl;

    // Square root
    using core::math::sqrt;
    using core::math::sqrtf;
    using core::math::sqrtl;

    // Cube root
    using core::math::cbrt;
    using core::math::cbrtf;
    using core::math::cbrtl;

    // Hypotenuse
    using core::math::hypot;
    using core::math::hypotf;
    using core::math::hypotl;

    // Sine
    using core::math::sin;
    using core::math::sinf;
    using core::math::sinl;

    // Cosine
    using core::math::cos;
    using core::math::cosf;
    using core::math::cosl;

    // Tangent
    using core::math::tan;
    using core::math::tanf;
    using core::math::tanl;

    // Arcsine
    using core::math::asin;
    using core::math::asinf;
    using core::math::asinl;

    // Arccosine
    using core::math::acos;
    using core::math::acosf;
    using core::math::acosl;

    // Arctangent
    using core::math::atan;
    using core::math::atanf;
    using core::math::atanl;

    // atan2 (Arc tangent with quadrants)
    using core::math::atan2;
    using core::math::atan2f;
    using core::math::atan2l;

    // Hyperbolic sine
    using core::math::sinh;
    using core::math::sinhf;
    using core::math::sinhl;

    // Hyperbolic cosine
    using core::math::cosh;
    using core::math::coshf;
    using core::math::coshl;

    // Hyperbolic tangent
    using core::math::tanh;
    using core::math::tanhf;
    using core::math::tanhl;

    // Inverse hyperbolic sine
    using core::math::asinh;
    using core::math::asinhf;
    using core::math::asinhl;

    // Inverse hyperbolic cosine
    using core::math::acosh;
    using core::math::acoshf;
    using core::math::acoshl;

    // Inverse hyperbolic tangent
    using core::math::atanh;
    using core::math::atanhf;
    using core::math::atanhl;

    // Error function
    using core::math::erf;
    using core::math::erff;
    using core::math::erfl;

    // Complementary error function
    using core::math::erfc;
    using core::math::erfcf;
    using core::math::erfcl;

    // Gamma function
    using core::math::tgamma;
    using core::math::tgammaf;
    using core::math::tgammal;

    // Natural logarithm of gamma function
    using core::math::lgamma;
    using core::math::lgammaf;
    using core::math::lgammal;

    // Ceiling function
    using core::math::ceil;
    using core::math::ceilf;
    using core::math::ceill;

    // Floor function
    using core::math::floor;
    using core::math::floorf;
    using core::math::floorl;

    // Truncate function
    using core::math::trunc;
    using core::math::truncf;
    using core::math::truncl;

    // Round
    using core::math::round;
    using core::math::roundf;
    using core::math::roundl;
    using core::math::lround;
    using core::math::lroundf;
    using core::math::lroundl;
    using core::math::llround;
    using core::math::llroundf;
    using core::math::llroundl;

    // Nearest integer
    using core::math::nearbyint;
    using core::math::nearbyintf;
    using core::math::nearbyintl;

    // Rounding nearest integer
    using core::math::rint;
    using core::math::rintf;
    using core::math::rintl;
    using core::math::lrint;
    using core::math::lrintf;
    using core::math::lrintl;
    using core::math::llrint;
    using core::math::llrintf;
    using core::math::llrintl;

    // Decompose number into significand/base-2 exponent
    using core::math::frexp;
    using core::math::frexpf;
    using core::math::frexpl;

    // Multiply number by power of 2
    using core::math::ldexp;
    using core::math::ldexpf;
    using core::math::ldexpl;

    // Decompose number into integer/fraction
    using core::math::modf;
    using core::math::modff;
    using core::math::modfl;

    // Multiply number by power of FLT_RADIX
    using core::math::scalbn;
    using core::math::scalbnf;
    using core::math::scalbnl;
    using core::math::scalbln;
    using core::math::scalblnf;
    using core::math::scalblnl;

    // Extract exponent
    using core::math::ilogb;
    using core::math::ilogbf;
    using core::math::ilogbl;

    // Extract exponent
    using core::math::logb;
    using core::math::logbf;
    using core::math::logbl;

    // Next representable floating-point value
    using core::math::nextafter;
    using core::math::nextafterf;
    using core::math::nextafterl;
    using core::math::nexttoward;
    using core::math::nexttowardf;
    using core::math::nexttowardl;

    // Copy sign
    using core::math::copysign;
    using core::math::copysignf;
    using core::math::copysignl;

    // Classify floating point number
    using core::math::fpclassify;

    // Check if finite
    using core::math::isfinite;

    // Check if infinite
    using core::math::isinf;

    // Check if NaN
    using core::math::isnan;

    // Check if normal
    using core::math::isnormal;

    // Check if negative
    using core::math::signbit;

    // Check if greater
    using core::math::isgreater;

    // Check if greater/equal
    using core::math::isgreaterequal;

    // Check if less
    using core::math::isless;

    // Check if less/equal
    using core::math::islessequal;

    // Check if less/greater
    using core::math::islessgreater;

    // Check if unordered
    using core::math::isunordered;

    // Associated Laguerre polynomials
    using core::math::assoc_laguerre;
    using core::math::assoc_laguerref;
    using core::math::assoc_laguerrel;

    // Associated Legendre polynomials
    using core::math::assoc_legendre;
    using core::math::assoc_legendref;
    using core::math::assoc_legendrel;

    // Beta function
    using core::math::beta;
    using core::math::betaf;
    using core::math::betal;

    // Complete elliptic integral (first kind)
    using core::math::comp_ellint_1;
    using core::math::comp_ellint_1f;
    using core::math::comp_ellint_1l;

    // Complete elliptic integral (second kind)
    using core::math::comp_ellint_2;
    using core::math::comp_ellint_2f;
    using core::math::comp_ellint_2l;

    // Complete elliptic integral (third kind)
    using core::math::comp_ellint_3;
    using core::math::comp_ellint_3f;
    using core::math::comp_ellint_3l;

    // Regular modified cylindrical Bessel functions
    using core::math::cyl_bessel_i;
    using core::math::cyl_bessel_if;
    using core::math::cyl_bessel_il;

    // Cylindrical Bessel functions (first kind)
    using core::math::cyl_bessel_j;
    using core::math::cyl_bessel_jf;
    using core::math::cyl_bessel_jl;

    // Irregular modified cylindrical Bessel functions
    using core::math::cyl_bessel_k;
    using core::math::cyl_bessel_kf;
    using core::math::cyl_bessel_kl;

    // Cylindrical Neumann functions
    using core::math::cyl_neumann;
    using core::math::cyl_neumannf;
    using core::math::cyl_neumannl;

    // Incomplete elliptic integral (first kind)
    using core::math::ellint_1;
    using core::math::ellint_1f;
    using core::math::ellint_1l;

    // Incomplete elliptic integral (second kind)
    using core::math::ellint_2;
    using core::math::ellint_2f;
    using core::math::ellint_2l;

    // Incomplete elliptic integral (third kind)
    using core::math::ellint_3;
    using core::math::ellint_3f;
    using core::math::ellint_3l;

    // Exponential integral
    using core::math::expint;
    using core::math::expintf;
    using core::math::expintl;

    // Hermite polynomials
    using core::math::hermite;
    using core::math::hermitef;
    using core::math::hermitel;

    // Legendre polynomials
    using core::math::legendre;
    using core::math::legendref;
    using core::math::legendrel;

    // Laguerre polynomials
    using core::math::laguerre;
    using core::math::laguerref;
    using core::math::laguerrel;

    // Riemann zeta function
    using core::math::riemann_zeta;
    using core::math::riemann_zetaf;
    using core::math::riemann_zetal;

    // Spherical Bessel functions (first kind)
    using core::math::sph_bessel;
    using core::math::sph_besself;
    using core::math::sph_bessell;

    // Spherical associated Legendre functions
    using core::math::sph_legendre;
    using core::math::sph_legendref;
    using core::math::sph_legendrel;

    // Spherical Neumann function
    using core::math::sph_neumann;
    using core::math::sph_neumannf;
    using core::math::sph_neumannl;
}
