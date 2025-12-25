/**
 * @file complex.cppm
 * @module core:math.complex
 * @brief Module file for standard library complex number operations.
 *
 * This file contains the implementation of the complex number operations in the standard library.
 */

module;

#include <complex>

export module core:math.complex;

/**
 * @namespace core::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace core::math {
    template <typename T>
    using Complex = std::complex<T>;

    using std::operator+;
    using std::operator-;
    using std::operator*;
    using std::operator/;
    using std::operator==;
    using std::operator<<;
    using std::operator>>;

    using std::real;
    using std::imag;
    using std::abs;
    using std::arg;
    using std::norm;
    using std::conj;
    using std::proj;
    using std::polar;

    using std::exp;
    using std::log;
    using std::log10;
    using std::pow;
    using std::sqrt;
    using std::sin;
    using std::cos;
    using std::tan;
    using std::asin;
    using std::acos;
    using std::atan;
    using std::sinh;
    using std::cosh;
    using std::tanh;
    using std::asinh;
    using std::acosh;
    using std::atanh;
}

/**
 * @namespace core::literals::string_view_literals
 * @brief Complex literals namespace
 */
export namespace core::inline literals::inline complex_literals {
    using std::literals::complex_literals::operator""i;
    using std::literals::complex_literals::operator""if;
    using std::literals::complex_literals::operator""il;
}

