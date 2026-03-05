/**
 * @file math.gmp.cppm
 * @module stdx:math.gmp
 * @brief Module file for wrappers for GMP (GNU Multiple Precision Arithmetic Library) operations.
 *
 * This file contains the implementation of GMP wrapper classes and functions in the standard library.
 * Warning: we have to do some strange compiler checks because of GCC bugs in loading definitions from modules.
 */

module;

#ifdef COMPILER_GCC
#warning "GMP support is currently forcefully disabled when compiling with GCC due to compiler bugs. Please use a different compiler if GMP functionality is needed."
#endif

#if defined(STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY) && !defined(COMPILER_GCC)
#include <gmpxx.h>
#endif

#include "Macros.hpp"

export module stdx:math.gmp;

import :main;

using namespace stdx::core;

/**
 * @namespace stdx::math::gmp
 * @brief Wrapper namespace for GNU Multiple Precision Arithmetic Library operations within stdx::math.
 */
export namespace stdx::math::inline gmp {

#if defined(STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY) && !defined(COMPILER_GCC)

using BitCount = ::mp_bitcnt_t;

class [[nodiscard]] GnuInteger {
private:
    mpz_class value;
protected:
    GnuInteger(mpz_class v):
        value{stdx::util::move(v)} {}
public:
    template <Numeric T>
        requires (!SameAs<T, bool>)
    explicit GnuInteger(T v = 0):
        value{v} {}

    explicit GnuInteger(const char s[], i32 base = 0) throws (InvalidArgumentException):
        value(s, base) {}

    explicit GnuInteger(const String& s, i32 base = 0) throws (InvalidArgumentException):
        value(s, base) {}

    explicit GnuInteger(StringView s, i32 base = 0) throws (InvalidArgumentException):
        value(s.data(), base) {}

    [[nodiscard]]
    String to_string(i32 base = 10) const {
        return value.get_str(base);
    }

    [[nodiscard]]
    i32 signum() const {
        return mpz_sgn(value.get_mpz_t());
    }

    [[nodiscard]]
    i64 to_long() const {
        return value.get_si();
    }

    [[nodiscard]]
    GnuInteger abs() const {
        return GnuInteger(::abs(value));
    }

    [[nodiscard]]
    GnuInteger gcd(const GnuInteger& b) const {
        mpz_class result;
        mpz_gcd(result.get_mpz_t(), value.get_mpz_t(), b.value.get_mpz_t());
        return GnuInteger(stdx::util::move(result));
    }

    [[nodiscard]]
    static GnuInteger gcd(const GnuInteger& a, const GnuInteger& b) {
        mpz_class result;
        mpz_gcd(result.get_mpz_t(), a.value.get_mpz_t(), b.value.get_mpz_t());
        return GnuInteger(stdx::util::move(result));
    }

    [[nodiscard]]
    GnuInteger lcm(const GnuInteger& b) const {
        mpz_class result;
        mpz_lcm(result.get_mpz_t(), value.get_mpz_t(), b.value.get_mpz_t());
        return GnuInteger(stdx::util::move(result));
    }

    [[nodiscard]]
    static GnuInteger lcm(const GnuInteger& a, const GnuInteger& b) {
        mpz_class result;
        ::mpz_lcm(result.get_mpz_t(), a.value.get_mpz_t(), b.value.get_mpz_t());
        return GnuInteger(stdx::util::move(result));
    }

    [[nodiscard]]
    GnuInteger pow(u32 exponent) const {
        mpz_class result;
        ::mpz_pow_ui(result.get_mpz_t(), value.get_mpz_t(), exponent);
        return GnuInteger(stdx::util::move(result));
    }

    [[nodiscard]]
    GnuInteger operator+() const {
        return GnuInteger(value);
    }

    [[nodiscard]]
    GnuInteger operator-() const {
        return GnuInteger(-value);
    }

    [[nodiscard]]
    GnuInteger operator+(const GnuInteger& b) const {
        mpz_class result = value;
        result += b.value;
        return GnuInteger(stdx::util::move(result));
    }

    [[nodiscard]]
    GnuInteger operator-(const GnuInteger& b) const {
        mpz_class result = value;
        result -= b.value;
        return GnuInteger(stdx::util::move(result));
    }

    [[nodiscard]]
    GnuInteger operator*(const GnuInteger& b) const {
        mpz_class result = value;
        result *= b.value;
        return GnuInteger(stdx::util::move(result));
    }

    [[nodiscard]]
    GnuInteger operator/(const GnuInteger& b) const {
        mpz_class result = value;
        result /= b.value;
        return GnuInteger(stdx::util::move(result));
    }

    [[nodiscard]]
    GnuInteger operator%(const GnuInteger& b) const {
        mpz_class result = value;
        result %= b.value;
        return GnuInteger(stdx::util::move(result));
    }

    GnuInteger& operator+=(const GnuInteger& b) {
        value += b.value;
        return *this;
    }

    GnuInteger& operator-=(const GnuInteger& b) {
        value -= b.value;
        return *this;
    }

    GnuInteger& operator*=(const GnuInteger& b) {
        value *= b.value;
        return *this;
    }

    GnuInteger& operator/=(const GnuInteger& b) {
        value /= b.value;
        return *this;
    }

    GnuInteger& operator%=(const GnuInteger& b) {
        value %= b.value;
        return *this;
    }

    [[nodiscard]]
    bool operator==(const GnuInteger& b) const {
        return value == b.value;
    }

    [[nodiscard]]
    StrongOrdering operator<=>(const GnuInteger& b) const {
        const i32 cmp = mpz_cmp(value.get_mpz_t(), b.value.get_mpz_t());
        if (cmp < 0) {
            return StrongOrdering::LESS;
        }
        if (cmp > 0) {
            return StrongOrdering::GREATER;
        }
        return StrongOrdering::EQUAL;
    }

    friend class GnuRational;
};

class [[nodiscard]] GnuRational {
private:
    mpq_class value;
protected:
    GnuRational(mpq_class v):
        value{stdx::util::move(v)} {}
public:
    template <Numeric T>
        requires (!SameAs<T, bool>)
    explicit GnuRational(T v = 0):
        value{v} {}

    template <Numeric T1, Numeric T2>
        requires (!SameAs<T1, bool>) && (!SameAs<T2, bool>)
    GnuRational(T1 numerator, T2 denominator):
        value(numerator.value, denominator.value) {}

    GnuRational(const GnuInteger& numerator, const GnuInteger& denominator):
        value(numerator.value, denominator.value) {
        value.canonicalize();
    }

    explicit GnuRational(const char s[], i32 base = 0) throws (InvalidArgumentException):
        value(s, base) {}

    explicit GnuRational(const String& s, i32 base = 0) throws (InvalidArgumentException):
        value(s, base) {}

    explicit GnuRational(StringView s, i32 base = 0) throws (InvalidArgumentException):
        value(s.data(), base) {}

    [[nodiscard]]
    String to_string(i32 base = 10) const {
        return value.get_str(base);
    }

    [[nodiscard]]
    GnuInteger numerator() const {
        return GnuInteger(value.get_num());
    }

    [[nodiscard]]
    GnuInteger denominator() const {
        return GnuInteger(value.get_den());
    }

    void canonicalize() {
        value.canonicalize();
    }

    [[nodiscard]]
    GnuRational operator+(const GnuRational& b) const {
        mpq_class result = value;
        result += b.value;
        return GnuRational(stdx::util::move(result));
    }

    [[nodiscard]]
    GnuRational operator-(const GnuRational& b) const {
        mpq_class result = value;
        result -= b.value;
        return GnuRational(stdx::util::move(result));
    }

    [[nodiscard]]
    GnuRational operator*(const GnuRational& b) const {
        mpq_class result = value;
        result *= b.value;
        return GnuRational(stdx::util::move(result));
    }

    [[nodiscard]]
    GnuRational operator/(const GnuRational& b) const {
        mpq_class result = value;
        result /= b.value;
        return GnuRational(stdx::util::move(result));
    }

    GnuRational& operator+=(const GnuRational& b) {
        value += b.value;
        return *this;
    }

    GnuRational& operator-=(const GnuRational& b) {
        value -= b.value;
        return *this;
    }

    GnuRational& operator*=(const GnuRational& b) {
        value *= b.value;
        return *this;
    }

    GnuRational& operator/=(const GnuRational& b) {
        value /= b.value;
        return *this;
    }

    [[nodiscard]]
    bool operator==(const GnuRational& b) const {
        return value == b.value;
    }

    [[nodiscard]]
    StrongOrdering operator<=>(const GnuRational& b) const {
        const i32 cmp = mpq_cmp(value.get_mpq_t(), b.value.get_mpq_t());
        if (cmp < 0) {
            return StrongOrdering::LESS;
        } else if (cmp > 0) {
            return StrongOrdering::GREATER;
        }
        return StrongOrdering::EQUAL;
    }
};

class [[nodiscard]] GnuFloat {
private:
    mpf_class value;
protected:
    GnuFloat(mpf_class v):
        value{stdx::util::move(v)} {}
public:
    template <Numeric T>
        requires (!SameAs<T, bool>)
    explicit GnuFloat(T v = 0):
        value{v} {}

    explicit GnuFloat(const char s[]) throws (InvalidArgumentException):
        value{s} {}

    explicit GnuFloat(const String& s) throws (InvalidArgumentException):
        value{s} {}

    explicit GnuFloat(StringView s) throws (InvalidArgumentException):
        value{s.data()} {}

    GnuFloat(const char s[], BitCount prec, i32 base = 0) throws (InvalidArgumentException):
        value(s, prec, base) {}

    GnuFloat(const String& s, BitCount prec, i32 base = 0) throws (InvalidArgumentException):
        value(s, prec, base) {}

    GnuFloat(StringView s, BitCount prec, i32 base = 0) throws (InvalidArgumentException):
        value(s.data(), prec, base) {}

    [[nodiscard]]
    f64 to_double() const {
        return value.get_d();
    }

    [[nodiscard]]
    GnuFloat operator+(const GnuFloat& b) const {
        mpf_class result = value;
        result += b.value;
        return GnuFloat(stdx::util::move(result));
    }

    [[nodiscard]]
    GnuFloat operator-(const GnuFloat& b) const {
        mpf_class result = value;
        result -= b.value;
        return GnuFloat(stdx::util::move(result));
    }

    [[nodiscard]]
    GnuFloat operator*(const GnuFloat& b) const {
        mpf_class result = value;
        result *= b.value;
        return GnuFloat(stdx::util::move(result));
    }

    [[nodiscard]]
    GnuFloat operator/(const GnuFloat& b) const {
        mpf_class result = value;
        result /= b.value;
        return GnuFloat(stdx::util::move(result));
    }

    GnuFloat& operator+=(const GnuFloat& b) {
        value += b.value;
        return *this;
    }

    GnuFloat& operator-=(const GnuFloat& b) {
        value -= b.value;
        return *this;
    }

    GnuFloat& operator*=(const GnuFloat& b) {
        value *= b.value;
        return *this;
    }

    GnuFloat& operator/=(const GnuFloat& b) {
        value /= b.value;
        return *this;
    }

    [[nodiscard]]
    bool operator==(const GnuFloat& b) const {
        return value == b.value;
    }

    [[nodiscard]]
    PartialOrdering operator<=>(const GnuFloat& b) const {
        const i32 cmp = mpf_cmp(value.get_mpf_t(), b.value.get_mpf_t());
        if (cmp < 0) {
            return PartialOrdering::LESS;
        } else if (cmp > 0) {
            return PartialOrdering::GREATER;
        }
        return PartialOrdering::EQUIVALENT;
    }
};

[[nodiscard]]
GnuInteger operator""_mpz(const char s[]) throws (InvalidArgumentException) {
    return GnuInteger(s);
}

[[nodiscard]]
GnuRational operator""_mpq(const char s[]) throws (InvalidArgumentException) {
    return GnuRational(s);
}

[[nodiscard]]
GnuFloat operator""_mpf(const char s[]) throws (InvalidArgumentException) {
    return GnuFloat(s);
}

#endif

}


