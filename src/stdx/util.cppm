/**
 * @file util.cppm
 * @module stdx:util
 * @brief Module file for utility operations.
 *
 * This file contains the imports for the utility operations.
 */

module;

export module stdx:util;

import :main;

using stdx::collections::HashMap;
using stdx::collections::TreeMap;
using stdx::collections::Vector;
using stdx::io::OutputStream;
using stdx::io::StringStream;
using stdx::iter::InputIterator;
using stdx::fmt::FormatContext;
using stdx::fmt::FormatParseContext;
using stdx::fmt::Formatter;
using stdx::meta::FalseType;
using stdx::meta::IsConvertibleValue;
using stdx::meta::IsSameValue;
using stdx::meta::RemoveReferenceType;
using stdx::meta::TrueType;
using stdx::ranges::IotaView;
using stdx::ranges::views::Take;
using stdx::text::CharsFormat;

using namespace stdx::core;
using namespace stdx::literals;

#if 0

namespace _detail {

template <typename T>
concept Collection = !IsSameValue<T, String> 
    && !IsSameValue<T, StringView>
    && requires(T t) {
    typename T::value_type;
    { t.begin() } -> InputIterator;
    { t.end() } -> InputIterator;
    { t.size() } -> ConvertibleTo<usize>;
};

template <typename T>
concept Streamable = requires(StringStream& out, const T& val) {
    { out << val } -> ConvertibleTo<StringStream&>;
};

constexpr usize REPRESENTATION_MAX_CONTAINER_SIZE = 5uz;

template <typename T>
[[nodiscard]]
String represent(const T& val) {
    if constexpr (IsSameValue<T, bool>) {
        return val ? "true" : "false";
    } else if constexpr (IsConvertibleValue<T, StringView>) {
        return stdx::fmt::format("\"{}\"", StringView(val));
    } else if constexpr (Collection<T>) {
        StringStream out;
        out << "{";
        const usize size = val.size();
        if (size > 1) {
            bool first = true;
            for (const auto& v: val | Take(math::min<usize>(size, REPRESENTATION_MAX_CONTAINER_SIZE))) {
                if (!first) {
                    out << " ";
                }
                first = false;
                out << represent(v);
                if (size <= REPRESENTATION_MAX_CONTAINER_SIZE) {
                    out << " ";
                } else {
                    out << "...";
                }
            }
        }
        if (size > 0) {
            out << represent(*stdx::iter::prev(val.end()));
        }
        out << "}";
        return out.str();
    } else if constexpr (Streamable<T>) {
        StringStream out;
        out << val;
        return out.str();
    } else {
        return "<Not representable!>";
    }
}

template <typename T>
concept StandardSignedInteger = IsSameValue<T, i8> || IsSameValue<T, i16> || IsSameValue<T, i32> || IsSameValue<T, i64>;

template <typename T>
concept StandardUnsignedInteger = IsSameValue<T, u8> || IsSameValue<T, u16> || IsSameValue<T, u32> || IsSameValue<T, u64>;

template <typename T>
concept StandardInteger = StandardSignedInteger<T> || StandardUnsignedInteger<T>;

template <typename F, typename Tpl, typename Ext, usize... I>
constexpr decltype(auto) apply_plus_one_impl(F&& f, Tpl&& t, Ext&& x, [[maybe_unused]] IndexSequence<I...> ind_seq) noexcept {
    (void)ind_seq;
    return invoke(stdx::util::forward<F>(f), get<I>(stdx::util::forward<Tpl>(t))..., stdx::util::forward<Ext>(x));
}

template <typename F, typename Tpl, typename Ext>
constexpr decltype(auto) apply_plus_one(F&& f, Tpl&& t, Ext&& x) noexcept {
    return _detail::apply_plus_one_impl(
        stdx::util::forward<F>(f),
        stdx::util::forward<Tpl>(t),
        stdx::util::forward<Ext>(x),
        make_index_sequence<TupleSizeValue<RemoveReferenceType<Tpl>>>{}
    );
}

[[nodiscard]]
constexpr Pair<const char*, const char*> pointer_range(StringView s) noexcept {
    return Pair(s.data(), s.data() + s.size());
}

template <class CharT, class Traits1, class Traits2>
[[nodiscard]]
constexpr bool starts_with(BasicStringView<CharT, Traits1> s, BasicStringView<CharT, Traits2> prefix) noexcept {
    return s.substr(0, prefix.size()) == prefix;
}

enum class CharsFormat: u8 {
    SCIENTIFIC = 1 << 0,
    FIXED = 1 << 1,
    HEX = 1 << 2,
    BINARY = 1 << 3,
    GENERAL = FIXED | SCIENTIFIC
};

struct [[nodiscard]] ConsumeBinaryPrefixResult {
    bool is_binary;
    StringView rest;

    constexpr ConsumeBinaryPrefixResult(bool is_binary, StringView rest):
        is_binary{is_binary}, rest{rest} {}
};

[[nodiscard]]
constexpr ConsumeBinaryPrefixResult consume_binary_prefix(StringView s) noexcept {
    if (s.starts_with("0b"sv) || s.starts_with("0B"sv)) {
        s.remove_prefix(2);
        return ConsumeBinaryPrefixResult(true, s);
    }
    return ConsumeBinaryPrefixResult(false, s);
}

struct [[nodiscard]] ConsumeHexadecimalPrefixResult {
    bool is_hexadecimal;
    StringView rest;

    constexpr ConsumeHexadecimalPrefixResult(bool is_hexadecimal, StringView rest):
        is_hexadecimal{is_hexadecimal}, rest{rest} {}
};

[[nodiscard]]
constexpr ConsumeHexadecimalPrefixResult consume_hexadecimal_prefix(StringView s) noexcept {
    if (s.starts_with("0x"sv) || s.starts_with("0X"sv)) {
        s.remove_prefix(2);
        return ConsumeHexadecimalPrefixResult(true, s);
    }
    return ConsumeHexadecimalPrefixResult(false, s);
}

template <typename T, usize Param>
T perform_from_chars(StringView s) throws (InvalidArgumentException, InvalidRangeException) {
    T x{0};
    auto [first, last] = pointer_range(s);
    auto [ptr, ec] = stdx::text::from_chars(first, last, x, Param);
    switch (ec) {
        case Errc::Self():
            if (ptr == last) {
                return x;
            }
            throw InvalidArgumentException(stdx::fmt::format("Pattern {} does not match to the end!", s));
        case Errc::INVALID_ARGUMENT:
            throw InvalidArgumentException(stdx::fmt::format("Pattern {} not found!", s));
        case Errc::RESULT_OUT_OF_RANGE:
            throw InvalidRangeException(stdx::fmt::format("'{}' not representable!", s));
        default:
            stdx::sys::unreachable();
    }
}

template <typename T, usize Param = 0uz>
struct ParseNumber {
    T operator()(StringView s) {
        return perform_from_chars<T, Param>(s);
    }
};

template <typename T>
struct ParseNumber<T, 2uz> {
    T operator()(StringView s) throws (InvalidArgumentException) {
        if (auto [ok, rest] = consume_binary_prefix(s); ok) {
            return perform_from_chars<T, 2uz>(rest);
        }
        throw InvalidArgumentException(stdx::fmt::format("Pattern '{}' not found!", s));
    }
};

template <typename T>
struct ParseNumber<T, 16uz> {
    T operator()(StringView s) throws (InvalidArgumentException, InvalidRangeException) {
        if (auto [ok, rest] = consume_hexadecimal_prefix(s); ok) {
            try {
                return perform_from_chars<T, 16uz>(rest);
            } catch (const InvalidArgumentException& e) {
                throw InvalidArgumentException(stdx::fmt::format("Failed to parse '{}' as hexadecimal: {}", s, e.what()));
            } catch (const InvalidRangeException& e) {
                throw InvalidRangeException(stdx::fmt::format("Failed to parse '{}' as hexadecimal: {}", s, e.what()));
            }
        }
        throw InvalidArgumentException(stdx::fmt::format("Failed to parse '{}' not identified as hexadecimal!", s));
    }
};

template <typename T>
struct ParseNumber<T> {
    T operator()(StringView s) throws (InvalidArgumentException, InvalidRangeException) {
        // Parse for hexadecimal
        auto [ok, rest] = consume_hexadecimal_prefix(s);
        if (ok) {
            try {
                return perform_from_chars<T, 16uz>(rest);
            } catch (const InvalidArgumentException& e) {
                throw InvalidArgumentException(stdx::fmt::format("Failed to parse '{}' as hexadecimal: {}", s, e.what()));
            } catch (const InvalidRangeException& e) {
                throw InvalidRangeException(stdx::fmt::format("Failed to parse '{}' as hexadecimal: {}", s, e.what()));
            }
        }

        // Parse for binary
        auto [ok_binary, rest_binary] = consume_binary_prefix(s);
        if (ok_binary) {
            try {
                return perform_from_chars<T, 2uz>(rest_binary);
            } catch (const InvalidArgumentException& e) {
                throw InvalidArgumentException(stdx::fmt::format("Failed to parse '{}' as binary: {}", s, e.what()));
            } catch (const InvalidRangeException& e) {
                throw InvalidRangeException(stdx::fmt::format("Failed to parse '{}' as binary: {}", s, e.what()));
            }
        }

        // Parse for octal
        if (s.starts_with("0"sv)) {
            try {
                return perform_from_chars<T, 8uz>(rest);
            } catch (const InvalidArgumentException& e) {
                throw InvalidArgumentException(stdx::fmt::format("Failed to parse '{}' as octal: {}", s, e.what()));
            } catch (const InvalidRangeException& e) {
                throw InvalidRangeException(stdx::fmt::format("Failed to parse '{}' as octal: {}", s, e.what()));
            }
        }

        // Parse for decimal
        try {
            return perform_from_chars<T, 10uz>(rest);
        } catch (const InvalidArgumentException& e) {
            throw InvalidArgumentException(stdx::fmt::format("Failed to parse '{}' as decimal integer: {}", s, e.what()));
        } catch (const InvalidRangeException& e) {
            throw InvalidRangeException(stdx::fmt::format("Failed to parse '{}' as decimal integer: {}", s, e.what()));
        }
    }
};

template <typename T>
concept FloatingPoint = IsSameValue<T, float> || IsSameValue<T, double> || IsSameValue<T, long double>;

template <FloatingPoint T>
T perform_floating_from_chars(StringView s, CharsFormat fmt) throws (InvalidArgumentException, InvalidRangeException) {
    if (stdx::text::isspace(static_cast<unsigned char>(s[0])) || s[0] == '+') {
        throw InvalidArgumentException(stdx::fmt::format("Pattern '{}' not found!", s));
    }
    
    T x{0};
    auto [first, last] = pointer_range(s);
    auto [ptr, ec] = stdx::text::from_chars(first, last, x, fmt);

    switch (ec) {
        case Errc::Self():
            if (ptr == last) {
                return x;
            }
            throw InvalidArgumentException(stdx::fmt::format("Pattern '{}' does not match to the end!", s));
        case Errc::INVALID_ARGUMENT:
            throw InvalidArgumentException(stdx::fmt::format("Pattern '{}' not found!", s));
        case Errc::RESULT_OUT_OF_RANGE:
            throw InvalidRangeException(stdx::fmt::format("'{}' not representable!", s));
        default:
            stdx::sys::unreachable();
    }
}

// Floating-point parsing with general format (auto-detect)
template <FloatingPoint T>
struct ParseNumber<T, static_cast<usize>(CharsFormat::GENERAL)> {
    T operator()(StringView s) throws (InvalidArgumentException, InvalidRangeException) {
        if (auto [ok, rest] = consume_hexadecimal_prefix(s); ok) {
            throw InvalidArgumentException("CharsFormat::GENERAL does not parse hexfloat");
        }
        if (auto [ok, rest] = consume_binary_prefix(s); ok) {
            throw InvalidArgumentException("CharsFormat::GENERAL does not parse binfloat");
        }

        try {
            return perform_floating_from_chars<T>(s, CharsFormat::GENERAL);
        } catch (const InvalidArgumentException& e) {
            throw InvalidArgumentException(stdx::fmt::format("Failed to parse '{}' as number: {}", s, e.what()));
        } catch (const InvalidRangeException& e) {
            throw InvalidRangeException(stdx::fmt::format("Failed to parse '{}' as number: {}", s, e.what()));
        }
    }
};

// Floating-point parsing with hexadecimal format
template <FloatingPoint T>
struct ParseNumber<T, static_cast<usize>(CharsFormat::HEX)> {
    T operator()(StringView s) throws (InvalidArgumentException, InvalidRangeException) {
        if (auto [ok, rest] = consume_binary_prefix(s); ok) {
            throw InvalidArgumentException("CharsFormat::HEX does not parse binfloat");
        }
        if (auto [ok, rest] = consume_hexadecimal_prefix(s); !ok) {
            throw InvalidArgumentException("CharsFormat::HEX parses hexfloat");
        }

        try {
            return perform_floating_from_chars<T>(s, CharsFormat::HEX);
        } catch (const InvalidArgumentException& e) {
            throw InvalidArgumentException(stdx::fmt::format("Failed to parse '{}' as hexadecimal: {}", s, e.what()));
        } catch (const InvalidRangeException& e) {
            throw InvalidRangeException(stdx::fmt::format("Failed to parse '{}' as hexadecimal: {}", s, e.what()));
        }
    }
};

// Floating-point parsing with binary format
template <FloatingPoint T>
struct ParseNumber<T, static_cast<usize>(CharsFormat::BINARY)> {
    T operator()(StringView s) throws (InvalidArgumentException, InvalidRangeException) {
        if (auto [ok, rest] = consume_hexadecimal_prefix(s); ok) {
            throw InvalidArgumentException("CharsFormat::BINARY does not parse hexfloat");
        }
        if (auto [ok, rest] = consume_binary_prefix(s); !ok) {
            throw InvalidArgumentException("CharsFormat::BINARY parses binfloat");
        }

        return perform_floating_from_chars<T>(s, CharsFormat::BINARY);
    }
};

// Floating-point parsing with scientific format
template <FloatingPoint T>
struct ParseNumber<T, static_cast<usize>(CharsFormat::SCIENTIFIC)> {
    T operator()(StringView s) throws (InvalidArgumentException, InvalidRangeException) {
        if (auto [ok, rest] = consume_hexadecimal_prefix(s); ok) {
            throw InvalidArgumentException("CharsFormat::SCIENTIFIC does not parse hexfloat");
        }
        if (auto [ok, rest] = consume_binary_prefix(s); ok) {
            throw InvalidArgumentException("CharsFormat::SCIENTIFIC does not parse binfloat");
        }
        if (s.find_first_of("eE"sv) == String::npos) {
            throw InvalidArgumentException("CharsFormat::SCIENTIFIC requires exponent part");
        }

        try {
            return perform_floating_from_chars<T>(s, CharsFormat::SCIENTIFIC);
        } catch (const InvalidArgumentException& e) {
            throw InvalidArgumentException(stdx::fmt::format("Failed to parse '{}' as scientific notation: {}", s, e.what()));
        } catch (const InvalidRangeException& e) {
            throw InvalidRangeException(stdx::fmt::format("Failed to parse '{}' as scientific notation: {}", s, e.what()));
        }
    }
};

// Floating-point parsing with fixed format
template <FloatingPoint T>
struct ParseNumber<T, static_cast<usize>(CharsFormat::FIXED)> {
    T operator()(StringView s) throws (InvalidArgumentException, InvalidRangeException) {
        if (auto [ok, rest] = consume_hexadecimal_prefix(s); ok) {
            throw InvalidArgumentException("CharsFormat::FIXED does not parse hexfloat");
        }
        if (auto [ok, rest] = consume_binary_prefix(s); ok) {
            throw InvalidArgumentException("CharsFormat::FIXED does not parse binfloat");
        }
        if (s.find_first_of("eE"sv) != String::npos) {
            throw InvalidArgumentException("CharsFormat::FIXED does not parse exponent part");
        }

        try {
            return perform_floating_from_chars<T>(s, CharsFormat::FIXED);
        } catch (const InvalidArgumentException& e) {
            throw InvalidArgumentException(stdx::fmt::format("Failed to parse '{}' as fixed notation: {}", s, e.what()));
        } catch (const InvalidRangeException& e) {
            throw InvalidRangeException(stdx::fmt::format("Failed to parse '{}' as fixed notation: {}", s, e.what()));
        }
    }
};

template <typename StrIter>
String join(StrIter first, StrIter last, const String& separator) {
    if (first == last) {
        return "";
    }
    StringStream value;
    value << StringView(*first);
    ++first;
    while (first != last) {
        value << StringView(separator) << StringView(*first);
        ++first;
    }
    return value.str();
}

template <typename StrIt>
[[nodiscard]]
String join(StrIt first, StrIt last, StringView separator) {
    if (first == last) {
        return "";
    }
    StringStream value;
    value << StringView(*first);
    ++first;
    while (first != last) {
        value << separator << StringView(*first);
        ++first;
    }
    return value.str();
}

template <typename T>
concept CanInvokeToString = requires(T val) {
    { to_string(val) } -> ConvertibleTo<String>;
};

template <typename T>
concept ChoiceTypeSupported = [] {
    using CleanType = stdx::meta::DecayType<T>;
    return stdx::meta::IsIntegralValue<CleanType>
        || IsSameValue<CleanType, String>
        || IsSameValue<CleanType, StringView>
        || IsSameValue<CleanType, const char*>;
}();

template <typename StrT>
[[nodiscard]]
usize get_levenshtein_distance(const StrT& s1, const StrT& s2) {
    Vector<Vector<usize>> dp(s1.size() + 1, Vector<usize>(s2.size() + 1, 0uz));

    for (usize i: IotaView(0uz, s1.size())) {
        for (usize j: IotaView(0uz, s2.size())) {
            if (i == 0uz) {
                dp[i][j] = j;
            } else if (j == 0uz) {
                dp[i][j] = i;
            } else if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1uz + stdx::math::min<usize>({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            }
        }
    }
    return dp[s1.size()][s2.size()];
}

template <typename V>
[[nodiscard]]
String get_most_similar_string(const TreeMap<String, V>& map, StringView input) {
    String most_similar;
    usize min_distance = NumericLimits<usize>::max();

    for (const auto& [key, value]: map) {
        usize distance = get_levenshtein_distance(key, String(input));
        if (distance < min_distance) {
            min_distance = distance;
            most_similar = key;
        }
    }

    return most_similar;
}

enum class NArgsPattern: u8 {
    OPTIONAL,
    ANY,
    AT_LEAST_ONE
};

enum class DefaultArguments: u8 {
    NONE = 0,
    HELP = 1,
    VERSION = 2,
    ALL = HELP | VERSION
};

[[nodiscard]]
constexpr DefaultArguments operator&(DefaultArguments a, DefaultArguments b) noexcept {
    return static_cast<DefaultArguments>(stdx::util::to_underlying(a) & stdx::util::to_underlying(b));
}

[[nodiscard]]
constexpr DefaultArguments operator|(DefaultArguments a, DefaultArguments b) noexcept {
    return static_cast<DefaultArguments>(stdx::util::to_underlying(a) | stdx::util::to_underlying(b));
}

}

/**
 * @namespace stdx::util
 * @brief Wrapper namespace for standard library extension utility operations.
 */
export namespace stdx::util {

class ArgumentParser;

class Argument {
private:
    using ReturnableAction = Function<Any(const String&)>;
    using VoidAction = Function<void(const String&)>;

    friend class ArgumentParser;
    friend class Formatter;

    class NArgsRange {
    private:
        usize min;
        usize max;
    public:
        NArgsRange(usize min, usize max) throws (InvalidArgumentException):
            min{min}, max{max} {
            if (min > max) {
                throw InvalidArgumentException("Range of number of arguments is invalid!");
            }
        }

        [[nodiscard]]
        bool contains(usize value) const noexcept {
            return value >= min && value <= max;
        }

        [[nodiscard]]
        bool is_exact() const noexcept {
            return min == max;
        }

        [[nodiscard]]
        bool is_right_bounded() const noexcept {
            return max < NumericLimits<usize>::max();
        }

        [[nodiscard]]
        usize get_min() const noexcept {
            return min;
        }

        [[nodiscard]]
        usize get_max() const noexcept {
            return max;
        }

        [[nodiscard]]
        StrongOrdering operator<=>(const NArgsRange& other) const noexcept {
            return min != other.min
                ? min <=> other.min
                : max <=> other.max;
        }
    };

    Vector<String> names;
    Vector<Variant<ReturnableAction, VoidAction>> actions;
    Variant<ReturnableAction, VoidAction> default_action{ 
        InPlaceType<ReturnableAction>,
        [](const String& s) -> String { return s; }
    };
    String help;
    String meta_variable;
    String default_value_representation;
    Optional<String> default_value_string;
    Any default_value;
    NArgsRange num_args_range{1, 1};
    StringView used_name;
    StringView prefix_chars;
    usize group_index = 0;
    u32 usage_newline_counter = 0;
    bool accepts_optional_like_value : 1;
    bool optional : 1;
    bool required : 1;
    bool repeatable : 1;
    bool used : 1;
    bool hidden : 1;

    template <usize N, usize... I>
    explicit Argument(StringView prefix_chars, Array<StringView, N>&& a, [[maybe_unused]] IndexSequence<I...> ind_seq):
        prefix_chars{prefix_chars}, accepts_optional_like_value{false}, optional{(optional(a[I], prefix_chars) || ...)},
        required{false}, repeatable{false}, used{false}, hidden{false} {
        (names.emplace_back(a[I]), ...);
        ranges::sort(
            names,
            []<TotallyOrdered T>(const T& lhs, const T& rhs) -> bool 
                requires requires(const T& t) { { t.size() } -> ConvertibleTo<usize>; }
            {
                return lhs.size() == rhs.size()
                    ? lhs < rhs
                    : lhs.size() < rhs.size();
            }
        );
    }
public:
    template <usize N>
    explicit Argument(StringView prefix_chars, Array<StringView, N>&& a):
        Argument(prefix_chars, stdx::util::move(a), make_index_sequence<N>{}) {}
};

/**
 * @class ParsedArguments
 * @brief Container for parsed argument values with type-safe access.
 * Adapted from p-ranav/argparse (https://github.com/p-ranav/argparse)
 */
class ParsedArguments {
private:
    HashMap<String, Any> values; ///< Values
    HashMap<String, Vector<String>> multiValues; ///< Multiple values
public:
    /**
     * @brief Get a single value with type checking.
     *
     * @tparam T The expected type of the value.
     * @param name The argument name.
     * @return The value cast to type T.
     *
     * @throws ArgumentParserException if argument not found or type mismatch.
    */
    template <typename T>
    T get(const String& name) const;

    /**
     * @brief Get a value with a default fallback.
     *
     * @tparam T The expect ed type of the value.
     * @param name The argument name.
     * @param defaultValue The default value if argument is not found.
     * @return The value or default value.
     */
    template <typename T>
    T get_or(const String& name, const T& defaultValue) const;

    /**
     * @brief Get multiple values for multi-value arguments.
     *
     * @param name The argument name.
     * @return Vector of string values.
     */
    Vector<String> get_multiple(const String& name) const;

    /**
     * @brief Check if an argument was provided.
     *
     * @param name The argument name.
     * @return true if argument exists, false otherwise.
     */
    bool has(const String& name) const;

    /**
     * @brief Set a single value.
     *
     * @param name The argument name.
     * @param value The value to set.
     */
    void set(const String& name, const Any& value);

    /**
     * @brief Set multiple values.
     *
     * @param name The argument name.
     * @param values The values to set.
     */
    void set_multiple(const String& name, const Vector<String>& values);

    /**
     * @brief Get all argument names that were parsed.
     *
     * @return Vector of argument names.
     */
    Vector<String> get_argument_names() const;
};

}

using stdx::util::Argument;

template <>
struct Formatter<Argument::NArgsRange> {
    static constexpr const char* parse(FormatParseContext& ctx) noexcept {
        return ctx.begin();
    }

    static FormatContext::iterator format(const Argument::NArgsRange& range, FormatContext& ctx) const {
        if (range.is_exact()) {
            if (range.get_min() != 0 && range.get_min() != 1) {
                return stdx::fmt::format_to(ctx.out(), "[nargs: {}] ", range.get_min());
            }
        } else {
            if (!range.is_right_bounded()) {
                return stdx::fmt::format_to(ctx.out(), "[nargs: {} or more] ", range.get_min());
            } else {
                return stdx::fmt::format_to(ctx.out(), "[nargs={}..{}] ", range.get_min(), range.get_max());
            }
        }
        return ctx.out();
    }
};

#endif