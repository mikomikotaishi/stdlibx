/**
 * @file complex.cppm
 * @module std:math.complex
 * @brief Module file for standard library complex number operations.
 *
 * This file contains the implementation of the complex number operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:math.complex;
#else
export module stdlib:math.complex;
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
    using core::math::Complex;

    using core::math::operator+;
    using core::math::operator-;
    using core::math::operator*;
    using core::math::operator/;
    using core::math::operator==;
    using core::math::operator<<;
    using core::math::operator>>;

    using core::math::real;
    using core::math::imag;
    using core::math::abs;
    using core::math::arg;
    using core::math::norm;
    using core::math::conj;
    using core::math::proj;
    using core::math::polar;

    using core::math::exp;
    using core::math::log;
    using core::math::log10;
    using core::math::pow;
    using core::math::sqrt;
    using core::math::sin;
    using core::math::cos;
    using core::math::tan;
    using core::math::asin;
    using core::math::acos;
    using core::math::atan;
    using core::math::sinh;
    using core::math::cosh;
    using core::math::tanh;
    using core::math::asinh;
    using core::math::acosh;
    using core::math::atanh;
}

/**
 * @namespace std::literals::string_view_literals
 * @brief Complex literals namespace
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::inline literals::inline complex_literals {
#else 
export namespace stdlib::inline literals::inline complex_literals {
#endif
    using core::literals::complex_literals::operator""i;
    using core::literals::complex_literals::operator""if;
    using core::literals::complex_literals::operator""il;
}
