#pragma once

/**
 * @class Number
 * @brief Abstract class representing a numeric type for wrapper classes.
 * 
 * @tparam T 
 */
template <typename T>
    requires Integral<T> || FloatingPoint<T>
class [[nodiscard]] Number {
protected:
    T value{0}; ///< The underlying numeric value.
public:
    static constexpr T MAX_VALUE = NumericLimits<T>::max(); ///< A constant representing the maximum value of the number type.
    static constexpr T MIN_VALUE = NumericLimits<T>::min(); ///< A constant representing the minimum value of the number type.

    constexpr Number() = default;
    constexpr Number(T v):
        value{v} {}
    constexpr virtual ~Number() = default;

    constexpr Number& operator=(T v) noexcept {
        value = v;
        return *this;
    }

    constexpr Number(const Number&) = default;
    constexpr Number(Number&&) noexcept = default;
    constexpr Number& operator=(const Number&) = default;
    constexpr Number& operator=(Number&&) noexcept = default;

    [[nodiscard]]
    constexpr Number operator+(T v) const noexcept {
        return Number{value + v};
    }

    [[nodiscard]]
    constexpr Number operator-(T v) const noexcept {
        return Number{value - v};
    }

    [[nodiscard]]
    constexpr Number operator*(T v) const noexcept {
        return Number{value * v};
    }

    [[nodiscard]]
    constexpr Number operator/(T v) const noexcept {
        return Number{value / v};
    }

    [[nodiscard]]
    constexpr Number operator%(T v) const noexcept 
        requires Integral<T> && Integral<decltype(value)> {
        return Number{value % v};
    }

    constexpr Number& operator+=(T v) noexcept {
        value += v;
        return *this;
    }

    constexpr Number& operator-=(T v) noexcept {
        value -= v;
        return *this;
    }

    constexpr Number& operator*=(T v) noexcept {
        value *= v;
        return *this;
    }

    constexpr Number& operator/=(T v) noexcept {
        value /= v;
        return *this;
    }

    constexpr Number& operator%=(T v) noexcept 
        requires Integral<T> && Integral<decltype(value)> {
        value %= v;
        return *this;
    }

    constexpr Number& operator++() noexcept {
        ++value;
        return *this;
    }

    constexpr Number operator++([[maybe_unused]] int _) noexcept {
        (void)_;
        Number temp = *this;
        ++value;
        return temp;
    }

    constexpr Number& operator--() noexcept {
        --value;
        return *this;
    }

    constexpr Number operator--([[maybe_unused]] int _) noexcept {
        (void)_;
        Number temp = *this;
        --value;
        return temp;
    }

    [[nodiscard]]
    constexpr Number operator~() const noexcept 
        requires Integral<T> {
        return Number{~value};
    }

    [[nodiscard]]
    constexpr Number operator&(T v) const noexcept 
        requires Integral<T> {
        return Number{value & v};
    }

    [[nodiscard]]
    constexpr Number operator|(T v) const noexcept 
        requires Integral<T> {
        return Number{value | v};
    }

    [[nodiscard]]
    constexpr Number operator^(T v) const noexcept 
        requires Integral<T> {
        return Number{value ^ v};
    }

    constexpr Number& operator&=(T v) noexcept 
        requires Integral<T> {
        value &= v;
        return *this;
    }

    constexpr Number& operator|=(T v) noexcept 
        requires Integral<T> {
        value |= v;
        return *this;
    }

    constexpr Number& operator^=(T v) noexcept 
        requires Integral<T> {
        value ^= v;
        return *this;
    }

    [[nodiscard]]
    constexpr Number operator-() const noexcept {
        return Number{-value};
    }

    [[nodiscard]]
    constexpr Number operator<<(int shift) const noexcept 
        requires Integral<T> {
        return Number{value << shift};
    }

    [[nodiscard]]
    constexpr Number operator>>(int shift) const noexcept 
        requires Integral<T> {
        return Number{value >> shift};
    }

    constexpr Number& operator<<=(int shift) noexcept 
        requires Integral<T> {
        value <<= shift;
        return *this;
    }

    constexpr Number& operator>>=(int shift) noexcept 
        requires Integral<T> {
        value >>= shift;
        return *this;
    }

    [[nodiscard]]
    StrongOrdering operator<=>(const Number& other) const noexcept = default;

    [[nodiscard]]
    constexpr operator T() const noexcept {
        return value;
    }

    [[nodiscard]]
    constexpr T get() const noexcept {
        return value;
    }

    [[nodiscard]]
    byte byte_value() const noexcept {
        return static_cast<byte>(static_cast<unsigned char>(value));
    }

    [[nodiscard]]
    static byte byte_value(T x) noexcept {
        return static_cast<byte>(static_cast<unsigned char>(x));
    }

    [[nodiscard]]
    i8 signed_byte_value() const noexcept {
        return static_cast<i8>(value);
    }

    [[nodiscard]]
    static i8 signed_byte_value(T x) noexcept {
        return static_cast<i8>(x);
    }

    [[nodiscard]]
    u8 unsigned_byte_value() const noexcept {
        return static_cast<u8>(value);
    }

    [[nodiscard]]
    static u8 unsigned_byte_value(T x) noexcept {
        return static_cast<u8>(x);
    }

    [[nodiscard]]
    virtual i16 short_value() const noexcept {
        return static_cast<i16>(value);
    }

    [[nodiscard]]
    virtual u16 unsigned_short_value() const noexcept {
        return static_cast<u16>(value);
    }

    [[nodiscard]]
    virtual i32 int_value() const noexcept {
        return static_cast<i32>(value);
    }

    [[nodiscard]]
    virtual u32 unsigned_int_value() const noexcept {
        return static_cast<u32>(value);
    }

    [[nodiscard]]
    virtual i64 long_value() const noexcept {
        return static_cast<i64>(value);
    }

    [[nodiscard]]
    virtual u64 unsigned_long_value() const noexcept {
        return static_cast<u64>(value);
    }

    [[nodiscard]]
    virtual f32 float_value() const noexcept {
        return static_cast<f32>(value);
    }

    [[nodiscard]]
    virtual f64 double_value() const noexcept {
        return static_cast<f64>(value);
    }

    [[nodiscard]]
    virtual f128 long_double_value() const noexcept {
        return static_cast<f128>(value);
    }

    [[nodiscard]]
    static T parse_or(StringView s, T default_value = T{0}) noexcept {
        T v{};
        auto [ptr, ec] = ::core::text::from_chars(s.data(), s.data() + s.size(), v);
        if (ec != Errc::SUCCESS) {
            return default_value;
        }
        return v;
    }

    [[nodiscard]]
    static Optional<T> try_parse(StringView s) noexcept {
        T v{};
        auto [ptr, ec] = ::core::text::from_chars(s.data(), s.data() + s.size(), v);
        if (ec != Errc::SUCCESS) {
            return nullopt;
        }
        return v;
    }

    #ifndef STDLIBX_NO_STD_CORE_FEATURES
    [[nodiscard]]
    static T parse(StringView s) throws (NumberFormatException) {
        T v{};
        auto [ptr, ec] = ::core::text::from_chars(s.data(), s.data() + s.size(), v);
        if (ec != Errc::SUCCESS) {
            throw NumberFormatException("Invalid format for number parsing.");
        }
        return v;
    }

    [[nodiscard]]
    String to_string() const noexcept {
        return stdx::fmt::format("{}", value);
    }

    [[nodiscard]]
    static String to_string(T v) noexcept {
        return stdx::fmt::format("{}", v);
    }
    #endif
};

template <CharacterLike Ch>
class [[nodiscard]] CharacterBase: public Number<Ch> {
public:
    [[nodiscard]]
    bool is_alphanumeric() const noexcept {
        return ::core::text::isalnum(static_cast<unsigned char>(this->value));
    }

    [[nodiscard]]
    bool is_alphabetic() const noexcept {
        return ::core::text::isalpha(static_cast<unsigned char>(this->value));
    }

    [[nodiscard]]
    bool is_lower_case() const noexcept {
        return ::core::text::islower(static_cast<unsigned char>(this->value));
    }

    [[nodiscard]]
    bool is_upper_case() const noexcept {
        return ::core::text::isupper(static_cast<unsigned char>(this->value));
    }

    [[nodiscard]]
    bool is_digit() const noexcept {
        return ::core::text::isdigit(static_cast<unsigned char>(this->value));
    }

    [[nodiscard]]
    bool is_whitespace() const noexcept {
        return ::core::text::isspace(static_cast<unsigned char>(this->value));
    }

    [[nodiscard]]
    bool is_control() const noexcept {
        return ::core::text::iscntrl(static_cast<unsigned char>(this->value));
    }

    [[nodiscard]]
    bool is_printable() const noexcept {
        return ::core::text::isprint(static_cast<unsigned char>(this->value));
    }
};

template <FloatingPoint F>
class [[nodiscard]] FloatingPointBase: public Number<F> {
public:
    static constexpr F POSITIVE_INFINITY = NumericLimits<F>::infinity(); ///< A constant representing positive infinity for the floating-point type.
    static constexpr F NEGATIVE_INFINITY = -NumericLimits<F>::infinity(); ///< A constant representing negative infinity for the floating-point type.
    static constexpr F NaN = NumericLimits<F>::quiet_NaN(); ///< A constant representing Not-a-Number (NaN) for the floating-point type.

    [[nodiscard]]
    bool is_nan() const noexcept {
        return ::core::math::isnan(this->value);
    }

    [[nodiscard]]
    bool is_infinite() const noexcept {
        return ::core::math::isinf(this->value);
    }

    [[nodiscard]]
    bool is_finite() const noexcept {
        return ::core::math::isfinite(this->value);
    }
};

class [[nodiscard]] Boolean final {
private:
    bool value = false;
public:
    constexpr Boolean() = default;
    constexpr Boolean(bool v):
        value{v} {}
    constexpr ~Boolean() = default;

    constexpr Boolean& operator=(bool v) noexcept {
        value = v;
        return *this;
    }

    constexpr Boolean(const Boolean&) = default;
    constexpr Boolean(Boolean&&) noexcept = default;
    constexpr Boolean& operator=(const Boolean&) = default;
    constexpr Boolean& operator=(Boolean&&) noexcept = default;

    [[nodiscard]]
    constexpr operator bool() const noexcept {
        return value;
    }

    [[nodiscard]]
    constexpr bool get() const noexcept {
        return value;
    }

    [[nodiscard]]
    constexpr Boolean operator!() const noexcept {
        return Boolean{!value};
    }

    [[nodiscard]]
    static bool parse_or(StringView s, bool default_value = false) noexcept {
        if (s == "true" || s == "1") {
            return true;
        } else if (s == "false" || s == "0") {
            return false;
        }
        return default_value;
    }

    [[nodiscard]]
    static Optional<bool> try_parse(StringView s) noexcept {
        if (s == "true" || s == "1") {
            return true;
        } else if (s == "false" || s == "0") {
            return false;
        }
        return nullopt;
}

    #ifndef STDLIBX_NO_STD_CORE_FEATURES
        static bool parse(StringView s) throws (NumberFormatException) {
        if (s == "true" || s == "1") {
            return true;
        } else if (s == "false" || s == "0") {
            return false;
        }
        throw NumberFormatException("Invalid format for boolean parsing.");
    }

    [[nodiscard]]
    String to_string() const noexcept {
        return value ? "true" : "false";
    }

    [[nodiscard]]
    static String to_string(bool v) noexcept {
        return v ? "true" : "false";
    }
    #endif
};

class [[nodiscard]] SignedByte final: public Number<i8> {
public:
    [[nodiscard]]
    constexpr operator i8() const noexcept {
        return value;
    }
};

class [[nodiscard]] Byte final: public Number<u8> {
public:
    [[nodiscard]]
    constexpr operator u8() const noexcept {
        return value;
    }
};

class [[nodiscard]] Short final: public Number<i16> {
public:
    [[nodiscard]]
    constexpr operator i16() const noexcept {
        return value;
    }
};

class [[nodiscard]] Integer final: public Number<i32> {
public:
    [[nodiscard]]
    constexpr operator i32() const noexcept {
        return value;
    }
};

class [[nodiscard]] Long final: public Number<i64> {
public:
    [[nodiscard]]
    constexpr operator i64() const noexcept {
        return value;
    }
};

class [[nodiscard]] UnsignedShort final: public Number<u16> {
public:
    [[nodiscard]]
    constexpr operator u16() const noexcept {
        return value;
    }
};

class [[nodiscard]] UnsignedInteger final: public Number<u32> {
public:
    [[nodiscard]]
    constexpr operator u32() const noexcept {
        return value;
    }
};

class [[nodiscard]] UnsignedLong final: public Number<u64> {
public:
    [[nodiscard]]
    constexpr operator u64() const noexcept {
        return value;
    }
};

class [[nodiscard]] Character final: public CharacterBase<char> {
public:
    [[nodiscard]]
    constexpr operator char() const noexcept {
        return value;
    }
};

class [[nodiscard]] UnsignedCharacter final: public CharacterBase<unsigned char> {
public:
    [[nodiscard]]
    constexpr operator unsigned char() const noexcept {
        return value;
    }
};

class [[nodiscard]] Character8 final: public CharacterBase<char8> {
public:
    [[nodiscard]]
    constexpr operator char8() const noexcept {
        return value;
    }
};

class [[nodiscard]] Character16 final: public CharacterBase<char16> {
public:
    [[nodiscard]]
    constexpr operator char16() const noexcept {
        return value;
    }
};

class [[nodiscard]] Character32 final: public CharacterBase<char32> {
public:
    [[nodiscard]]
    constexpr operator char32() const noexcept {
        return value;
    }
};

class [[nodiscard]] WideCharacter final: public CharacterBase<wchar> {
public:
    [[nodiscard]]
    constexpr operator wchar() const noexcept {
        return value;
    }
};

class [[nodiscard]] SignedSize final: public Number<isize> {
public:
    [[nodiscard]]
    constexpr operator isize() const noexcept {
        return value;
    }
};

class [[nodiscard]] UnsignedSize final: public Number<usize> {
public:
    [[nodiscard]]
    constexpr operator usize() const noexcept {
        return value;
    }
};

class [[nodiscard]] Float final: public FloatingPointBase<f32> {
public:
    [[nodiscard]]
    constexpr operator f32() const noexcept {
        return value;
    }
};

class [[nodiscard]] Double final: public FloatingPointBase<f64> {
public:
    [[nodiscard]]
    constexpr operator f64() const noexcept {
        return value;
    }
};

class [[nodiscard]] LongDouble final: public FloatingPointBase<f128> {
public:
    [[nodiscard]]
    constexpr operator f128() const noexcept {
        return value;
    }
};

using Integer8 = SignedByte;
using Integer16 = Short;
using Integer32 = Integer;
using Integer64 = Long;
using UnsignedInteger8 = Byte;
using UnsignedInteger16 = UnsignedShort;
using UnsignedInteger32 = UnsignedInteger;
using UnsignedInteger64 = UnsignedLong;
using Float32 = Float;
using Float64 = Double;
using Float128 = LongDouble;
// using Float16 = std::float16;
// using Float32 = std::float32;
// using Float64 = std::float64;
// using Float128 = std::float128;
