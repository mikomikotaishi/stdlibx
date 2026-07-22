/**
 * @file math.gmp.cppm
 * @module stdx:math.gmp
 * @brief Module file for wrappers for GMP (GNU Multiple Precision Arithmetic Library) operations.
 *
 * This file contains the implementation of GMP wrapper classes and functions in the standard library.
 * Warning: we have to do some strange compiler checks because of GCC bugs in loading definitions from modules.
 */

module;

#ifdef STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY
#include <gmpxx.h>
#endif

#include "Macros.hpp"

export module stdx:math.gmp;

import :main;

#ifdef __GNUC__
using namespace stdx::core;
#endif

/**
 * @namespace stdx::math::gmp
 * @brief GNU Multiple Precision Arithmetic Library operations within stdx::math.
 */
export namespace stdx::math::inline gmp {
    #ifdef STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY
    using BitCount = ::mp_bitcnt_t;

    class [[nodiscard]] GnuInteger {
    private:
        ::mpz_class _value;
    protected:
        GnuInteger(::mpz_class value):
            _value{Ops::move(value)} {}
    public:
        template <Numeric T>
            requires (!SameAs<T, bool>)
        explicit GnuInteger(T value = 0):
            _value{value} {}

        THROWS(InvalidArgumentException)
        explicit GnuInteger(const char s[], i32 base = 0):
            _value(s, base) {}

        THROWS(InvalidArgumentException)
        explicit GnuInteger(const String& s, i32 base = 0):
            _value(s, base) {}

        THROWS(InvalidArgumentException)
        explicit GnuInteger(StringView s, i32 base = 0):
            _value(s.data(), base) {}

        [[nodiscard]]
        String to_string(i32 base = 10) const {
            return _value.get_str(base);
        }

        [[nodiscard]]
        i32 signum() const {
            return mpz_sgn(_value.get_mpz_t());
        }

        [[nodiscard]]
        i64 to_long() const {
            return _value.get_si();
        }

        [[nodiscard]]
        GnuInteger abs() const {
            return GnuInteger(::abs(_value));
        }

        [[nodiscard]]
        GnuInteger gcd(const GnuInteger& b) const {
            ::mpz_class result;
            ::mpz_gcd(result.get_mpz_t(), _value.get_mpz_t(), b._value.get_mpz_t());
            return GnuInteger(Ops::move(result));
        }

        [[nodiscard]]
        static GnuInteger gcd(const GnuInteger& a, const GnuInteger& b) {
            ::mpz_class result;
            ::mpz_gcd(result.get_mpz_t(), a._value.get_mpz_t(), b._value.get_mpz_t());
            return GnuInteger(Ops::move(result));
        }

        [[nodiscard]]
        GnuInteger lcm(const GnuInteger& b) const {
            ::mpz_class result;
            ::mpz_lcm(result.get_mpz_t(), _value.get_mpz_t(), b._value.get_mpz_t());
            return GnuInteger(Ops::move(result));
        }

        [[nodiscard]]
        static GnuInteger lcm(const GnuInteger& a, const GnuInteger& b) {
            ::mpz_class result;
            ::mpz_lcm(result.get_mpz_t(), a._value.get_mpz_t(), b._value.get_mpz_t());
            return GnuInteger(Ops::move(result));
        }

        [[nodiscard]]
        GnuInteger pow(u32 exponent) const {
            ::mpz_class result;
            ::mpz_pow_ui(result.get_mpz_t(), _value.get_mpz_t(), exponent);
            return GnuInteger(Ops::move(result));
        }

        [[nodiscard]]
        GnuInteger operator+() const {
            return GnuInteger(_value);
        }

        [[nodiscard]]
        GnuInteger operator-() const {
            return GnuInteger(-_value);
        }

        [[nodiscard]]
        GnuInteger operator+(const GnuInteger& b) const {
            ::mpz_class result = _value;
            result += b._value;
            return GnuInteger(Ops::move(result));
        }

        [[nodiscard]]
        GnuInteger operator-(const GnuInteger& b) const {
            ::mpz_class result = _value;
            result -= b._value;
            return GnuInteger(Ops::move(result));
        }

        [[nodiscard]]
        GnuInteger operator*(const GnuInteger& b) const {
            ::mpz_class result = _value;
            result *= b._value;
            return GnuInteger(Ops::move(result));
        }

        [[nodiscard]]
        GnuInteger operator/(const GnuInteger& b) const {
            ::mpz_class result = _value;
            result /= b._value;
            return GnuInteger(Ops::move(result));
        }

        [[nodiscard]]
        GnuInteger operator%(const GnuInteger& b) const {
            ::mpz_class result = _value;
            result %= b._value;
            return GnuInteger(Ops::move(result));
        }

        GnuInteger& operator+=(const GnuInteger& b) {
            _value += b._value;
            return *this;
        }

        GnuInteger& operator-=(const GnuInteger& b) {
            _value -= b._value;
            return *this;
        }

        GnuInteger& operator*=(const GnuInteger& b) {
            _value *= b._value;
            return *this;
        }

        GnuInteger& operator/=(const GnuInteger& b) {
            _value /= b._value;
            return *this;
        }

        GnuInteger& operator%=(const GnuInteger& b) {
            _value %= b._value;
            return *this;
        }

        [[nodiscard]]
        bool operator==(const GnuInteger& b) const {
            return _value == b._value;
        }

        [[nodiscard]]
        StrongOrdering operator<=>(const GnuInteger& b) const {
            const i32 cmp = ::mpz_cmp(_value.get_mpz_t(), b._value.get_mpz_t());
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
        ::mpq_class _value;
    protected:
        GnuRational(::mpq_class value):
            _value{Ops::move(value)} {}
    public:
        template <Numeric T>
            requires (!SameAs<T, bool>)
        explicit GnuRational(T v = 0):
            _value{v} {}

        template <Numeric T1, Numeric T2>
            requires (!SameAs<T1, bool>) && (!SameAs<T2, bool>)
        GnuRational(T1 numerator, T2 denominator):
            _value(numerator.value, denominator.value) {}

        GnuRational(const GnuInteger& numerator, const GnuInteger& denominator):
            _value(numerator._value, denominator._value) {
            _value.canonicalize();
        }

        THROWS(InvalidArgumentException)
        explicit GnuRational(const char s[], i32 base = 0):
            _value(s, base) {}

        THROWS(InvalidArgumentException)
        explicit GnuRational(const String& s, i32 base = 0):
            _value(s, base) {}

        THROWS(InvalidArgumentException)
        explicit GnuRational(StringView s, i32 base = 0):
            _value(s.data(), base) {}

        [[nodiscard]]
        String to_string(i32 base = 10) const {
            return _value.get_str(base);
        }

        [[nodiscard]]
        GnuInteger numerator() const {
            return GnuInteger(_value.get_num());
        }

        [[nodiscard]]
        GnuInteger denominator() const {
            return GnuInteger(_value.get_den());
        }

        void canonicalize() {
            _value.canonicalize();
        }

        [[nodiscard]]
        GnuRational operator+(const GnuRational& b) const {
            ::mpq_class result = _value;
            result += b._value;
            return GnuRational(Ops::move(result));
        }

        [[nodiscard]]
        GnuRational operator-(const GnuRational& b) const {
            ::mpq_class result = _value;
            result -= b._value;
            return GnuRational(Ops::move(result));
        }

        [[nodiscard]]
        GnuRational operator*(const GnuRational& b) const {
            ::mpq_class result = _value;
            result *= b._value;
            return GnuRational(Ops::move(result));
        }

        [[nodiscard]]
        GnuRational operator/(const GnuRational& b) const {
            ::mpq_class result = _value;
            result /= b._value;
            return GnuRational(Ops::move(result));
        }

        GnuRational& operator+=(const GnuRational& b) {
            _value += b._value;
            return *this;
        }

        GnuRational& operator-=(const GnuRational& b) {
            _value -= b._value;
            return *this;
        }

        GnuRational& operator*=(const GnuRational& b) {
            _value *= b._value;
            return *this;
        }

        GnuRational& operator/=(const GnuRational& b) {
            _value /= b._value;
            return *this;
        }

        [[nodiscard]]
        bool operator==(const GnuRational& b) const {
            return _value == b._value;
        }

        [[nodiscard]]
        StrongOrdering operator<=>(const GnuRational& b) const {
            const i32 cmp = ::mpq_cmp(_value.get_mpq_t(), b._value.get_mpq_t());
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
        ::mpf_class _value;
    protected:
        GnuFloat(::mpf_class value):
            _value{Ops::move(value)} {}
    public:
        template <Numeric T>
            requires (!SameAs<T, bool>)
        explicit GnuFloat(T value = 0):
            _value{value} {}

        THROWS(InvalidArgumentException)
        explicit GnuFloat(const char s[]):
            _value{s} {}

        THROWS(InvalidArgumentException)
        explicit GnuFloat(const String& s):
            _value{s} {}

        THROWS(InvalidArgumentException)
        explicit GnuFloat(StringView s):
            _value{s.data()} {}

        THROWS(InvalidArgumentException)
        GnuFloat(const char s[], BitCount prec, i32 base = 0):
            _value(s, prec, base) {}

        THROWS(InvalidArgumentException)
        GnuFloat(const String& s, BitCount prec, i32 base = 0):
            _value(s, prec, base) {}

        THROWS(InvalidArgumentException)
        GnuFloat(StringView s, BitCount prec, i32 base = 0):
            _value(s.data(), prec, base) {}

        [[nodiscard]]
        f64 to_double() const {
            return _value.get_d();
        }

        [[nodiscard]]
        GnuFloat operator+(const GnuFloat& b) const {
            ::mpf_class result = _value;
            result += b._value;
            return GnuFloat(Ops::move(result));
        }

        [[nodiscard]]
        GnuFloat operator-(const GnuFloat& b) const {
            ::mpf_class result = _value;
            result -= b._value;
            return GnuFloat(Ops::move(result));
        }

        [[nodiscard]]
        GnuFloat operator*(const GnuFloat& b) const {
            ::mpf_class result = _value;
            result *= b._value;
            return GnuFloat(Ops::move(result));
        }

        [[nodiscard]]
        GnuFloat operator/(const GnuFloat& b) const {
            ::mpf_class result = _value;
            result /= b._value;
            return GnuFloat(Ops::move(result));
        }

        GnuFloat& operator+=(const GnuFloat& b) {
            _value += b._value;
            return *this;
        }

        GnuFloat& operator-=(const GnuFloat& b) {
            _value -= b._value;
            return *this;
        }

        GnuFloat& operator*=(const GnuFloat& b) {
            _value *= b._value;
            return *this;
        }

        GnuFloat& operator/=(const GnuFloat& b) {
            _value /= b._value;
            return *this;
        }

        [[nodiscard]]
        bool operator==(const GnuFloat& b) const {
            return _value == b._value;
        }

        [[nodiscard]]
        PartialOrdering operator<=>(const GnuFloat& b) const {
            const i32 cmp = ::mpf_cmp(_value.get_mpf_t(), b._value.get_mpf_t());
            if (cmp < 0) {
                return PartialOrdering::LESS;
            } else if (cmp > 0) {
                return PartialOrdering::GREATER;
            }
            return PartialOrdering::EQUIVALENT;
        }
    };
    #endif
}

#ifdef STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY
using stdx::math::gmp::GnuInteger;
using stdx::math::gmp::GnuRational;
using stdx::math::gmp::GnuFloat;
#endif

export namespace stdx::literals::inline gmp_literals {
    #ifdef STDLIBX_EXTENSIONS_COMPILE_MATH_GMP_LIBRARY
    [[nodiscard]]
    THROWS(InvalidArgumentException)
    inline GnuInteger operator""_mpz(const char* s, usize len) {
        return GnuInteger(StringView(s, len));
    }

    [[nodiscard]]
    THROWS(InvalidArgumentException)
    inline GnuRational operator""_mpq(const char* s, usize len) {
        return GnuRational(StringView(s, len));
    }

    [[nodiscard]]
    THROWS(InvalidArgumentException)
    inline GnuFloat operator""_mpf(const char* s, usize len) {
        return GnuFloat(StringView(s, len));
    }
    #endif
}


