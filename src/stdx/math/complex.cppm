/**
 * @file complex.cppm
 * @module stdx:math.complex
 * @brief Module file for standard library complex number operations.
 *
 * This file contains the implementation of the complex number operations in the standard library.
 */

module;

export module stdx:math.complex;

import core;

/**
 * @namespace stdx::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace stdx::math {
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
 * @namespace stdx::literals::complex_literals
 * @brief Complex literals namespace
 */
export namespace stdx::inline literals::inline complex_literals {
    using core::literals::complex_literals::operator""i;
    using core::literals::complex_literals::operator""if;
    using core::literals::complex_literals::operator""il;
}
