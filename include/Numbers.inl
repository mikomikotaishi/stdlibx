#pragma once

/**
 * @internal
 * @namespace _unicode_detail
 * @brief Detail namespace - not to be exported for external use
 */
namespace _unicode_detail {
    #include "Unicode/UnicodeBlock.inl"
    #include "Unicode/UnicodeNames.inl"
    #include "Unicode/UnicodeScript.inl"
}

template <typename T, typename Derived>
concept HasGet = requires (const Derived& d) {
    { d.get() } -> ConvertibleTo<T>;
};

/**
 * @class Number
 * @brief Abstract class representing a numeric type for wrapper classes.
 * 
 * @tparam T The underlying numeric type (e.g., int, float, double).
 * @tparam Derived The derived class that inherits from Number.
 */
export template <typename T, typename Derived>
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
    constexpr Derived operator+(T v) const noexcept {
        return Derived{static_cast<const Derived&>(*this).get() + v};
    }

    [[nodiscard]]
    constexpr Derived operator-(T v) const noexcept {
        return Derived{static_cast<const Derived&>(*this).get() - v};
    }

    [[nodiscard]]
    constexpr Derived operator*(T v) const noexcept {
        return Derived{static_cast<const Derived&>(*this).get() * v};
    }

    [[nodiscard]]
    constexpr Derived operator/(T v) const noexcept {
        return Derived{static_cast<const Derived&>(*this).get() / v};
    }

    [[nodiscard]]
    constexpr Derived operator%(T v) const noexcept 
        requires Integral<T> && Integral<decltype(value)> {
        return Derived{static_cast<const Derived&>(*this).get() % v};
    }

    constexpr Derived& operator+=(T v) noexcept {
        value += v;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator-=(T v) noexcept {
        value -= v;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator*=(T v) noexcept {
        value *= v;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator/=(T v) noexcept {
        value /= v;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator%=(T v) noexcept 
        requires Integral<T> && Integral<decltype(value)> {
        value %= v;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator++() noexcept {
        ++value;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator--() noexcept {
        --value;
        return static_cast<Derived&>(*this);
    }

    [[nodiscard]]
    constexpr Derived operator~() const noexcept 
        requires Integral<T> {
        return Derived{~static_cast<const Derived&>(*this).get()};
    }

    [[nodiscard]]
    constexpr Derived operator&(T v) const noexcept 
        requires Integral<T> {
        return Derived{static_cast<const Derived&>(*this).get() & v};
    }

    [[nodiscard]]
    constexpr Derived operator|(T v) const noexcept 
        requires Integral<T> {
        return Derived{static_cast<const Derived&>(*this).get() | v};
    }

    [[nodiscard]]
    constexpr Derived operator^(T v) const noexcept 
        requires Integral<T> {
        return Derived{static_cast<const Derived&>(*this).get() ^ v};
    }

    constexpr Derived& operator&=(T v) noexcept 
        requires Integral<T> {
        value &= v;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator|=(T v) noexcept 
        requires Integral<T> {
        value |= v;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator^=(T v) noexcept 
        requires Integral<T> {
        value ^= v;
        return static_cast<Derived&>(*this);
    }

    [[nodiscard]]
    constexpr Derived operator-() const noexcept {
        return Derived{-static_cast<const Derived&>(*this).get()};
    }

    [[nodiscard]]
    constexpr Derived operator<<(int shift) const noexcept 
        requires Integral<T> {
        return Derived{static_cast<const Derived&>(*this).get() << shift};
    }

    [[nodiscard]]
    constexpr Derived operator>>(int shift) const noexcept 
        requires Integral<T> {
        return Derived{static_cast<const Derived&>(*this).get() >> shift};
    }

    constexpr Derived& operator<<=(int shift) noexcept 
        requires Integral<T> {
        value <<= shift;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator>>=(int shift) noexcept 
        requires Integral<T> {
        value >>= shift;
        return static_cast<Derived&>(*this);
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

/**
 * @class CharacterBase
 * @brief A base class for character types, providing common character-related methods.
 * 
 * @tparam Ch The character type (e.g., char, wchar, char16, char32).
 * @tparam Derived The derived class that inherits from CharacterBase.
 */
template <CharacterLike Ch, typename Derived>
class [[nodiscard]] CharacterBase {
public:
    [[nodiscard]]
    bool is_alphanumeric() const noexcept requires HasGet<Ch, Derived> {
        return ::core::text::isalnum(
            static_cast<unsigned char>(
                static_cast<const Derived&>(*this).get()
            )
        );
    }

    [[nodiscard]]
    bool is_alphabetic() const noexcept requires HasGet<Ch, Derived> {
        return ::core::text::isalpha(
            static_cast<unsigned char>(
                static_cast<const Derived&>(*this).get()
            )
        );
    }

    [[nodiscard]]
    bool is_lower_case() const noexcept requires HasGet<Ch, Derived> {
        return ::core::text::islower(
            static_cast<unsigned char>(
                static_cast<const Derived&>(*this).get()
            )
        );
    }

    [[nodiscard]]
    bool is_upper_case() const noexcept requires HasGet<Ch, Derived> {
        return ::core::text::isupper(
            static_cast<unsigned char>(
                static_cast<const Derived&>(*this).get()
            )
        );
    }

    [[nodiscard]]
    bool is_digit() const noexcept requires HasGet<Ch, Derived> {
        return ::core::text::isdigit(
            static_cast<unsigned char>(
                static_cast<const Derived&>(*this).get()
            )
        );
    }

    [[nodiscard]]
    bool is_whitespace() const noexcept requires HasGet<Ch, Derived> {
        return ::core::text::isspace(
            static_cast<unsigned char>(
                static_cast<const Derived&>(*this).get()
            )
        );
    }

    [[nodiscard]]
    bool is_control() const noexcept requires HasGet<Ch, Derived> {
        return ::core::text::iscntrl(
            static_cast<unsigned char>(
                static_cast<const Derived&>(*this).get()
            )
        );
    }

    [[nodiscard]]
    bool is_printable() const noexcept requires HasGet<Ch, Derived> {
        return ::core::text::isprint(static_cast<unsigned char>(
            static_cast<const Derived&>(*this).get()
        ));
    }
};

/**
 * @class FloatingPointBase
 * @brief A base class for floating-point types, providing common methods for handling special floating-point values like NaN and infinity.
 * 
 * @tparam F The floating-point type (e.g., float, double, long double).
 * @tparam Derived The derived class that inherits from FloatingPointBase.
 */
template <FloatingPoint F, typename Derived>
class [[nodiscard]] FloatingPointBase {
public:
    static constexpr F POSITIVE_INFINITY = NumericLimits<F>::infinity(); ///< A constant representing positive infinity for the floating-point type.
    static constexpr F NEGATIVE_INFINITY = -NumericLimits<F>::infinity(); ///< A constant representing negative infinity for the floating-point type.
    static constexpr F NaN = NumericLimits<F>::quiet_NaN(); ///< A constant representing Not-a-Number (NaN) for the floating-point type.

    [[nodiscard]]
    bool is_nan() const noexcept requires HasGet<F, Derived> {
        return ::core::math::isnan(static_cast<const Derived&>(*this).get());
    }

    [[nodiscard]]
    bool is_infinite() const noexcept requires HasGet<F, Derived> {
        return ::core::math::isinf(static_cast<const Derived&>(*this).get());
    }

    [[nodiscard]]
    bool is_finite() const noexcept requires HasGet<F, Derived> {
        return ::core::math::isfinite(static_cast<const Derived&>(*this).get());
    }
};

export class [[nodiscard]] Boolean final {
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

export class [[nodiscard]] SignedByte final: public Number<i8, SignedByte> {
public:
    [[nodiscard]]
    constexpr operator i8() const noexcept {
        return value;
    }
};

export class [[nodiscard]] Byte final: public Number<u8, Byte> {
public:
    [[nodiscard]]
    constexpr operator u8() const noexcept {
        return value;
    }
};

export class [[nodiscard]] Short final: public Number<i16, Short> {
public:
    [[nodiscard]]
    constexpr operator i16() const noexcept {
        return value;
    }
};

export class [[nodiscard]] Integer final: public Number<i32, Integer> {
public:
    [[nodiscard]]
    constexpr operator i32() const noexcept {
        return value;
    }
};

export class [[nodiscard]] Long final: public Number<i64, Long> {
public:
    [[nodiscard]]
    constexpr operator i64() const noexcept {
        return value;
    }
};

export class [[nodiscard]] UnsignedShort final: public Number<u16, UnsignedShort> {
public:
    [[nodiscard]]
    constexpr operator u16() const noexcept {
        return value;
    }
};

export class [[nodiscard]] UnsignedInteger final: public Number<u32, UnsignedInteger> {
public:
    [[nodiscard]]
    constexpr operator u32() const noexcept {
        return value;
    }
};

export class [[nodiscard]] UnsignedLong final: public Number<u64, UnsignedLong> {
public:
    [[nodiscard]]
    constexpr operator u64() const noexcept {
        return value;
    }
};

export class [[nodiscard]] Character final: public Number<char, Character>, private CharacterBase<char, Character> {
public:
    using UnicodeBlock = _unicode_detail::UnicodeBlock;
    using UnicodeNames = _unicode_detail::UnicodeNames;
    using UnicodeScript = _unicode_detail::UnicodeScript;

    [[nodiscard]]
    constexpr operator char() const noexcept {
        return value;
    }
};

export class [[nodiscard]] UnsignedCharacter final: public Number<unsigned char, UnsignedCharacter>, private CharacterBase<unsigned char, UnsignedCharacter> {
public:
    using UnicodeBlock = _unicode_detail::UnicodeBlock;
    using UnicodeNames = _unicode_detail::UnicodeNames;
    using UnicodeScript = _unicode_detail::UnicodeScript;

    [[nodiscard]]
    constexpr operator unsigned char() const noexcept {
        return value;
    }
};

export class [[nodiscard]] Character8 final: public Number<char8, Character8>, private CharacterBase<char8, Character8> {
public:
    using UnicodeBlock = _unicode_detail::UnicodeBlock;
    using UnicodeNames = _unicode_detail::UnicodeNames;
    using UnicodeScript = _unicode_detail::UnicodeScript;

    [[nodiscard]]
    constexpr operator char8() const noexcept {
        return value;
    }
};

export class [[nodiscard]] Character16 final: public Number<char16, Character16>, private CharacterBase<char16, Character16> {
public:
    using UnicodeBlock = _unicode_detail::UnicodeBlock;
    using UnicodeNames = _unicode_detail::UnicodeNames;
    using UnicodeScript = _unicode_detail::UnicodeScript;

    [[nodiscard]]
    constexpr operator char16() const noexcept {
        return value;
    }
};

export class [[nodiscard]] Character32 final: public Number<char32, Character32>, private CharacterBase<char32, Character32> {
public:
    using UnicodeBlock = _unicode_detail::UnicodeBlock;
    using UnicodeNames = _unicode_detail::UnicodeNames;
    using UnicodeScript = _unicode_detail::UnicodeScript;

    [[nodiscard]]
    constexpr operator char32() const noexcept {
        return value;
    }
};

export class [[nodiscard]] WideCharacter final: public Number<wchar, WideCharacter>, private CharacterBase<wchar, WideCharacter> {
public:
    using UnicodeBlock = _unicode_detail::UnicodeBlock;
    using UnicodeNames = _unicode_detail::UnicodeNames;
    using UnicodeScript = _unicode_detail::UnicodeScript;

    [[nodiscard]]
    constexpr operator wchar() const noexcept {
        return value;
    }
};

export class [[nodiscard]] SignedSize final: public Number<isize, SignedSize> {
public:
    [[nodiscard]]
    constexpr operator isize() const noexcept {
        return value;
    }
};

export class [[nodiscard]] UnsignedSize final: public Number<usize, UnsignedSize> {
public:
    [[nodiscard]]
    constexpr operator usize() const noexcept {
        return value;
    }
};

#ifdef __STDCPP_FLOAT16_T__
export class [[nodiscard]] Half final: public Number<f16, Half>, private FloatingPointBase<f16, Half> {
public:
    [[nodiscard]]
    constexpr operator f16() const noexcept {
        return value;
    }
};
#endif

#ifdef __STDCPP_BFLOAT16_T__
export class [[nodiscard]] BrainHalf final: public Number<bf16, BrainHalf>, private FloatingPointBase<bf16, BrainHalf> {
public:
    [[nodiscard]]
    constexpr operator bf16() const noexcept {
        return value;
    }
};
#endif

export class [[nodiscard]] Float final: public Number<f32, Float>, private FloatingPointBase<f32, Float> {
public:
    [[nodiscard]]
    constexpr operator f32() const noexcept {
        return value;
    }
};

export class [[nodiscard]] Double final: public Number<f64, Double>, private FloatingPointBase<f64, Double> {
public:
    [[nodiscard]]
    constexpr operator f64() const noexcept {
        return value;
    }
};

export class [[nodiscard]] Quad final: public Number<f128, Quad>, private FloatingPointBase<f128, Quad> {
public:
    [[nodiscard]]
    constexpr operator f128() const noexcept {
        return value;
    }
};

export using Integer8 = SignedByte;
export using Integer16 = Short;
export using Integer32 = Integer;
export using Integer64 = Long;
export using UnsignedInteger8 = Byte;
export using UnsignedInteger16 = UnsignedShort;
export using UnsignedInteger32 = UnsignedInteger;
export using UnsignedInteger64 = UnsignedLong;
#ifdef __STDCPP_FLOAT16_T__
export using Float16 = Half;
#endif
#ifdef __STDCPP_BFLOAT16_T__
export using BrainFloat16 = BrainHalf;
#endif
export using Float32 = Float;
export using Float64 = Double;
export using Float128 = Quad;
