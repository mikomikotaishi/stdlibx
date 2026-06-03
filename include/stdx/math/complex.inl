#pragma once

/**
 * @namespace stdx::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace stdx::math {
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
