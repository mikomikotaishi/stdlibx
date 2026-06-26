#pragma once

using stdx::collections::LinkedList;
using stdx::collections::HashMap;
using stdx::collections::TreeMap;
using stdx::collections::Vector;
using stdx::io::Cout;
using stdx::io::FormatFlags;
using stdx::io::OutputStream;
using stdx::io::StreamSize;
using stdx::io::StringStream;
using stdx::fmt::FormatContext;
using stdx::fmt::FormatParseContext;
using stdx::fs::Path;
using stdx::meta::FalseType;
using stdx::meta::ConditionalType;
using stdx::meta::DecayType;
using stdx::meta::InvokeResultType;
using stdx::meta::IsConstValue;
using stdx::meta::IsConvertibleValue;
using stdx::meta::IsIntegralValue;
using stdx::meta::IsSameValue;
using stdx::meta::IsVoidValue;
using stdx::meta::IsVolatileValue;
using stdx::meta::RemoveReferenceType;
using stdx::meta::TrueType;
using stdx::ranges::IotaView;
using stdx::text::CharTraits;

#ifdef __cpp_lib_reflection
using stdx::meta::reflect::ReflectableClass;
#endif

using namespace stdx::literals;

namespace stdx::util {

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
concept Streamable = requires (StringStream& out, const T& val) {
    { out << val } -> ConvertibleTo<StringStream&>;
};

constexpr usize REPRESENTATION_MAX_CONTAINER_SIZE = 5uz;

template <typename T>
[[nodiscard]]
String represent(const T& val) {
    if constexpr (IsSameValue<T, bool>) {
        return val ? "true" : "false";
    } else if constexpr (IsConvertibleValue<T, StringView>) {
        return stdx::fmt::format("\"{}\"", val);
    } else if constexpr (Collection<T>) {
        StringStream out;
        out << "{";
        const usize size = val.size();
        if (size > 1) {
            out << represent(*val.begin());
            usize count = 1;
            const usize limit = Math::min<usize>(size, REPRESENTATION_MAX_CONTAINER_SIZE);
            for (auto it = Iterators::next(val.begin()); count < limit - 1; ++it, ++count) {
                out << stdx::fmt::format(" {}", represent(*it));
            }
            if (size <= REPRESENTATION_MAX_CONTAINER_SIZE) {
                out << " ";
            } else {
                out << "...";
            }
        }
        if (size > 0) {
            out << represent(*Iterators::prev(val.end()));
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

template <typename F, typename Tpl, typename Ext, usize... I>
constexpr decltype(auto) apply_plus_one_impl(F&& f, Tpl&& t, Ext&& x, [[maybe_unused]] IndexSequence<I...> ind_seq) noexcept {
    return invoke(Ops::forward<F>(f), get<I>(Ops::forward<Tpl>(t))..., Ops::forward<Ext>(x));
}

template <typename F, typename Tpl, typename Ext>
constexpr decltype(auto) apply_plus_one(F&& f, Tpl&& t, Ext&& x) noexcept {
    return apply_plus_one_impl(
        Ops::forward<F>(f),
        Ops::forward<Tpl>(t),
        Ops::forward<Ext>(x),
        IndexSequenceOf<TupleSizeValue<RemoveReferenceType<Tpl>>>{}
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
        case Errc::SUCCESS:
            if (ptr == last) {
                return x;
            }
            throw InvalidArgumentException(stdx::fmt::format("Pattern {} does not match to the end!", s));
        case Errc::INVALID_ARGUMENT:
            throw InvalidArgumentException(stdx::fmt::format("Pattern {} not found!", s));
        case Errc::RESULT_OUT_OF_RANGE:
            throw InvalidRangeException(stdx::fmt::format("'{}' not representable!", s));
        default:
            Ops::unreachable();
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

template <FloatingPoint T>
T perform_floating_from_chars(StringView s, CharsFormat fmt) throws (InvalidArgumentException, InvalidRangeException) {
    if (Character::is_whitespace(static_cast<unsigned char>(s[0])) || s[0] == '+') {
        throw InvalidArgumentException(stdx::fmt::format("Pattern '{}' not found!", s));
    }
    
    T x{0};
    auto [first, last] = pointer_range(s);
    auto [ptr, ec] = stdx::text::from_chars(first, last, x, fmt);

    switch (ec) {
        case Errc::SUCCESS:
            if (ptr == last) {
                return x;
            }
            throw InvalidArgumentException(stdx::fmt::format("Pattern '{}' does not match to the end!", s));
        case Errc::INVALID_ARGUMENT:
            throw InvalidArgumentException(stdx::fmt::format("Pattern '{}' not found!", s));
        case Errc::RESULT_OUT_OF_RANGE:
            throw InvalidRangeException(stdx::fmt::format("'{}' not representable!", s));
        default:
            Ops::unreachable();
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

template <typename It>
String join(It first, It last, const String& separator) {
    if (first == last) {
        return "";
    }
    StringStream value;
    value << *first;
    ++first;
    while (first != last) {
        value << stdx::fmt::format("{}{}", separator, *first);
        ++first;
    }
    return value.str();
}

template <typename It>
[[nodiscard]]
String join(It first, It last, StringView separator) {
    if (first == last) {
        return "";
    }
    StringStream value;
    value << *first;
    ++first;
    while (first != last) {
        value << stdx::fmt::format("{}{}", separator, *first);
        ++first;
    }
    return value.str();
}

template <typename T>
concept CanInvokeToString = requires(T val) {
    { to_string(val) } -> ConvertibleTo<String>;
};

template <typename T>
concept ChoiceTypeSupported = [] -> bool {
    return IsIntegralValue<DecayType<T>>
        || IsSameValue<DecayType<T>, String>
        || IsSameValue<DecayType<T>, StringView>
        || IsSameValue<DecayType<T>, const char*>;
}();

template <typename Str>
[[nodiscard]]
usize get_levenshtein_distance(const Str& s1, const Str& s2) {
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
                dp[i][j] = 1uz + Math::min<usize>({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            }
        }
    }
    return dp[s1.size()][s2.size()];
}

template <typename V>
[[nodiscard]]
String get_most_similar_string(const TreeMap<String, V>& map, StringView input) {
    String most_similar;
    usize min_distance = UnsignedSize::MAX_VALUE;

    for (const auto& [key, value]: map) {
        usize distance = get_levenshtein_distance(key, String(input));
        if (distance < min_distance) {
            min_distance = distance;
            most_similar = key;
        }
    }

    return most_similar;
}

template <typename... Ts>
[[nodiscard]]
consteval bool is_one_of(char c, Ts... xs) noexcept {
    return ((c == xs) || ...);
}

}

/**
 * @namespace stdx::util
 * @brief Standard library extension utility operations.
 */
export namespace stdx::util {

enum class NArgsPattern: u8 {
    OPTIONAL,
    ANY,
    AT_LEAST_ONE,
};

enum class DefaultArguments: u8 {
    NONE = 0,
    HELP = 1,
    VERSION = 2,
    ALL = HELP | VERSION,
};

[[nodiscard]]
constexpr DefaultArguments operator&(DefaultArguments a, DefaultArguments b) noexcept {
    return static_cast<DefaultArguments>(Ops::to_underlying(a) & Ops::to_underlying(b));
}

[[nodiscard]]
constexpr DefaultArguments operator|(DefaultArguments a, DefaultArguments b) noexcept {
    return static_cast<DefaultArguments>(Ops::to_underlying(a) | Ops::to_underlying(b));
}

class CommandLineParserException: public RuntimeException {
public:
    using RuntimeException::RuntimeException;
};

class ArgumentParser;

class Argument {
public:
    using ReturnableAction = Function<Any(const String&)>;
    using VoidAction = Function<void(const String&)>;

    class NArgsRange {
    private:
        usize min_val;
        usize max_val;
    public:
        NArgsRange(usize min, usize max) throws (InvalidArgumentException):
            min_val{min}, max_val{max} {
            if (min > max) {
                throw InvalidArgumentException("Range of number of arguments is invalid!");
            }
        }

        [[nodiscard]]
        bool contains(usize value) const noexcept {
            return value >= min_val && value <= max_val;
        }

        [[nodiscard]]
        bool is_exact() const noexcept {
            return min_val == max_val;
        }

        [[nodiscard]]
        bool is_right_bounded() const noexcept {
            return max_val < UnsignedSize::MAX_VALUE;
        }

        [[nodiscard]]
        usize min() const noexcept {
            return min_val;
        }

        [[nodiscard]]
        usize max() const noexcept {
            return max_val;
        }

        [[nodiscard]]
        bool operator==(const NArgsRange& other) const noexcept = default;

        [[nodiscard]]
        StrongOrdering operator<=>(const NArgsRange& other) const noexcept {
            return min_val != other.min_val ? min_val <=> other.min_val : max_val <=> other.max_val;
        }
    };

private:
    Vector<String> names;
    Vector<Variant<ReturnableAction, VoidAction>> actions;
    Variant<ReturnableAction, VoidAction> default_action {
        InPlaceType<ReturnableAction>, [](const String& s) -> String { return s; }
    };
    String help_text;
    String meta_variable;
    String default_value_repr;
    Optional<String> default_value_string;
    Any default_val;
    Any implicit_val;
    Optional<Vector<String>> choice_values;
    Vector<Any> values;
    NArgsRange num_args_range = {1, 1};
    StringView used_name;
    StringView prefix_chars;
    usize group_index = 0;
    u32 usage_newline_counter = 0;
    bool accepts_optional_like_value : 1 = false;
    bool is_optional : 1 = false;
    bool is_required : 1 = false;
    bool is_repeatable : 1 = false;
    bool is_used : 1 = false;
    bool is_hidden : 1 = false;

    friend class ArgumentParser;

    static constexpr i32 EOF_CHAR = CharTraits<char>::eof();

    [[nodiscard]]
    static i32 lookahead(StringView s) {
        return s.empty() ? EOF_CHAR : static_cast<i32>(static_cast<unsigned char>(s[0]));
    }

    [[nodiscard]]
    static bool is_decimal_literal(StringView s) {
        auto is_digit = [](char c) constexpr -> bool {
            return c >= '0' && c <= '9';
        };
        auto consume_digits = [=](StringView sd) -> StringView {
            const char* it = stdx::util::find_if_not(sd.begin(), sd.end(), is_digit);
            return sd.substr(static_cast<usize>(it - sd.begin()));
        };

        switch (lookahead(s)) {
            case '0':
                s.remove_prefix(1);
                if (s.empty()) {
                    return true;
                }
                goto integer_part;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                s = consume_digits(s);
                if (s.empty()) {
                    return true;
                }
                goto integer_part_consumed;
            case '.':
                s.remove_prefix(1);
                goto post_decimal_point;
            default:
                return false;
        }

    integer_part:
        s = consume_digits(s);
    integer_part_consumed:
        switch (lookahead(s)) {
            case '.':
                s.remove_prefix(1);
                if (is_digit(lookahead(s))) {
                    goto post_decimal_point;
                } else {
                    goto exponent_part_opt;
                }
            case 'e':
            case 'E':
                s.remove_prefix(1);
                goto post_e;
            default:
                return false;
        }

    post_decimal_point:
        if (is_digit(lookahead(s))) {
            s = consume_digits(s);
            goto exponent_part_opt;
        }
        return false;

    exponent_part_opt:
        switch (lookahead(s)) {
            case EOF_CHAR:
                return true;
            case 'e':
            case 'E':
                s.remove_prefix(1);
                goto post_e;
            default:
                return false;
        }

    post_e:
        if (lookahead(s) == '-' || lookahead(s) == '+') {
            s.remove_prefix(1);
        }
        if (is_digit(lookahead(s))) {
            s = consume_digits(s);
            return s.empty();
        }
        return false;
    }

    [[nodiscard]]
    static bool check_positional(StringView name, StringView pfx) {
        i32 first = lookahead(name);
        if (first == EOF_CHAR) {
            return true;
        }
        if (pfx.find(static_cast<char>(first)) != StringView::npos) {
            name.remove_prefix(1);
            if (name.empty()) {
                return true;
            }
            return is_decimal_literal(name);
        }
        return true;
    }

    [[nodiscard]]
    static bool check_optional(StringView name, StringView pfx) {
        return !check_positional(name, pfx);
    }

    [[noreturn]]
    void throw_nargs_range_validation() const throws (CommandLineParserException) {
        StringStream stream;
        stream << (!used_name.empty() ? used_name : StringView(names.front())) << ": ";
        if (num_args_range.is_exact()) {
            stream << num_args_range.min();
        } else if (num_args_range.is_right_bounded()) {
            stream << stdx::fmt::format("{} to {}", num_args_range.min(), num_args_range.max());
        } else {
            stream << stdx::fmt::format("{} or more", num_args_range.min());
        }
        stream << stdx::fmt::format(" argument(s) expected. {} provided.", values.size());
        throw CommandLineParserException(stream.str());
    }

    [[noreturn]]
    void throw_required_arg_not_used() const throws (CommandLineParserException) {
        throw CommandLineParserException(stdx::fmt::format("{}: required.", names.front()));
    }

    [[noreturn]]
    void throw_required_arg_no_value_provided() const throws (CommandLineParserException) {
        throw CommandLineParserException(stdx::fmt::format("{}: no value provided.", used_name));
    }

    template <typename T>
    [[nodiscard]]
    static T any_cast_container(const Vector<Any>& operand) {
        using ValueType = typename T::value_type;
        T result;
        for (const Any& value: operand) {
            result.push_back(any_cast<ValueType>(value));
        }
        return result;
    }

    void find_default_value_in_choices_or_throw() const throws (CommandLineParserException) {
        const Vector<String>& ch = choice_values.value();
        if (default_val.has_value()) {
            if (stdx::util::find(ch.begin(), ch.end(), default_value_string) == ch.end()) {
                String csv = stdx::util::accumulate(
                    ch.begin(),
                    ch.end(),
                    ""s,
                    [](const String& a, const String& b) -> String {
                        return stdx::fmt::format("{}{}", a, (a.empty() ? "" : ", ") + b);
                    }
                );
                throw CommandLineParserException(stdx::fmt::format("Invalid default value {} - allowed options: {{{}}}", default_value_repr, csv));
            }
        }
    }

    template <typename Iterator>
    [[nodiscard]]
    bool is_value_in_choices(Iterator option_it) const {
        const Vector<String>& ch = choice_values.value();
        return stdx::util::find(ch.begin(), ch.end(), *option_it) != ch.end();
    }

    template <typename Iterator>
    [[noreturn]]
    void throw_invalid_arguments(Iterator option_it) const throws (CommandLineParserException) {
        const Vector<String>& ch = choice_values.value();
        String csv = stdx::util::accumulate(
            ch.begin(),
            ch.end(),
            ""s,
            [](const String& a, const String& b) -> String {
                return stdx::fmt::format("{}{}", a, (a.empty() ? "" : ", ") + b);
            }
        );
        throw CommandLineParserException(stdx::fmt::format("Invalid argument {} - allowed options: {{{}}}", represent(*option_it), csv));
    }

    void set_usage_newline_counter(u32 i) {
        usage_newline_counter = i;
    }

    void set_group_index(usize i) {
        group_index = i;
    }

    template <usize N, usize... I>
    explicit Argument(StringView pfx, Array<StringView, N>&& a, [[maybe_unused]] IndexSequence<I...> ind_seq):
        prefix_chars{pfx}, is_optional{(check_optional(a[I], pfx) || ...)} {
        (names.emplace_back(a[I]), ...);
        stdx::ranges::sort(names,
            []<TotallyOrdered T>(const T& lhs, const T& rhs) -> bool
                requires requires (const T& t) { { t.size() } -> ConvertibleTo<usize>; }
            { return lhs.size() == rhs.size() ? lhs < rhs : lhs.size() < rhs.size(); }
        );
    }

public:
    template <usize N>
    explicit Argument(StringView prefix_chars, Array<StringView, N>&& a):
        Argument(prefix_chars, Ops::move(a), IndexSequenceOf<N>{}) {}

    Argument& help(String text) {
        help_text = Ops::move(text);
        return *this;
    }

    Argument& metavar(String mv) {
        meta_variable = Ops::move(mv); return *this;
    }

    template <typename T>
    Argument& default_value(T&& value) {
        num_args_range = NArgsRange{0, num_args_range.max()};
        default_value_repr = represent(value);
        if constexpr (IsConvertibleValue<T, StringView>) {
            default_value_string = String{StringView{value}};
        } else if constexpr (CanInvokeToString<T>) {
            default_value_string = to_string(value);
        }
        default_val = Ops::forward<T>(value);
        return *this;
    }

    Argument& default_value(const char* value) {
        return default_value(String(value));
    }

    Argument& required() {
        is_required = true; return *this;
    }

    Argument& implicit_value(Any value) {
        implicit_val = Ops::move(value);
        num_args_range = NArgsRange{0, 0};
        return *this;
    }

    Argument& flag() {
        default_value(false);
        implicit_value(true);
        return *this;
    }

    template <typename F, typename... Args>
    Argument& action(F&& callable, Args&&... bound_args) {
        using ActionType = ConditionalType<
            IsVoidValue<InvokeResultType<F, Args..., const String&>>,
            VoidAction,
            ReturnableAction
        >;
        if constexpr (sizeof...(Args) == 0) {
            actions.emplace_back(ActionType(Ops::forward<F>(callable)));
        } else {
            actions.emplace_back(ActionType(
                [
                    f = Ops::forward<F>(callable),
                    tup = make_tuple(Ops::forward<Args>(bound_args)...)
                ](const String& opt) mutable {
                    return apply_plus_one(f, tup, opt);
                }
            ));
        }
        return *this;
    }

    Argument& store_into(bool& var) {
        if (!default_val.has_value() && !implicit_val.has_value()) {
            flag();
        }
        if (default_val.has_value()) {
            var = any_cast<bool>(default_val);
        }
        action(
            [&var](const String& _) -> bool {
                var = true;
                return var;
            }
        );
        return *this;
    }

    template <Integral T>
    Argument& store_into(T& var) {
        if (default_val.has_value()) {
            var = any_cast<T>(default_val);
        }
        action(
            [&var](const auto& s) -> T {
                var = ParseNumber<T, 10uz>()(s);
                return var;
            }
        );
        return *this;
    }

    template <FloatingPoint T>
    Argument& store_into(T& var) {
        if (default_val.has_value()) {
            var = any_cast<T>(default_val);
        }
        action(
            [&var](const auto& s) -> T {
                var = ParseNumber<T, static_cast<usize>(CharsFormat::GENERAL)>()(s);
                return var;
            }
        );
        return *this;
    }

    Argument& store_into(String& var) {
        if (default_val.has_value()) {
            var = any_cast<String>(default_val);
        }
        action(
            [&var](const String& s) -> String {
                var = s;
                return var;
            }
        );
        return *this;
    }

    Argument& store_into(Path& var) {
        if (default_val.has_value()) {
            var = any_cast<Path>(default_val);
        }
        action(
            [&var](const String& s) -> Path {
                var = s;
                return var;
            }
        );
        return *this;
    }

    Argument& store_into(Vector<String>& var) {
        if (default_val.has_value()) {
            var = any_cast<Vector<String>>(default_val);
        }
        action(
            [this, &var](const String& s) -> Vector<String> {
                if (!is_used) {
                    var.clear();
                }
                is_used = true;
                var.push_back(s);
                return var;
            }
        );
        return *this;
    }

    Argument& store_into(Vector<i32>& var) {
        if (default_val.has_value()) {
            var = any_cast<Vector<i32>>(default_val);
        }
        action(
            [this, &var](const String& s) {
                if (!is_used) {
                    var.clear();
                }
                is_used = true;
                var.push_back(ParseNumber<i32, 10uz>()(s));
                return var;
            }
        );
        return *this;
    }

    Argument& append() {
        is_repeatable = true;
        return *this;
    }

    Argument& hidden() {
        is_hidden = true;
        return *this;
    }

    template <char Shape, typename T>
    Argument& scan() {
        static_assert(!(IsConstValue<T> || IsVolatileValue<T>), "T should not be cv-qualified");
        if constexpr (is_one_of(Shape, 'd') && Integral<T>) {
            action(ParseNumber<T, 10uz>());
        } else if constexpr (is_one_of(Shape, 'i') && Integral<T>) {
            action(ParseNumber<T>());
        } else if constexpr (is_one_of(Shape, 'u') && UnsignedIntegral<T>) {
            action(ParseNumber<T, 10uz>());
        } else if constexpr (is_one_of(Shape, 'b') && UnsignedIntegral<T>) {
            action(ParseNumber<T, 2uz>());
        } else if constexpr (is_one_of(Shape, 'o') && UnsignedIntegral<T>) {
            action(ParseNumber<T, 8uz>());
        } else if constexpr (is_one_of(Shape, 'x', 'X') && UnsignedIntegral<T>) {
            action(ParseNumber<T, 16uz>());
        } else if constexpr (is_one_of(Shape, 'a', 'A') && FloatingPoint<T>) {
            action(ParseNumber<T, static_cast<usize>(CharsFormat::HEX)>());
        } else if constexpr (is_one_of(Shape, 'e', 'E') && FloatingPoint<T>) {
            action(ParseNumber<T, static_cast<usize>(CharsFormat::SCIENTIFIC)>());
        } else if constexpr (is_one_of(Shape, 'f', 'F') && FloatingPoint<T>) {
            action(ParseNumber<T, static_cast<usize>(CharsFormat::FIXED)>());
        } else if constexpr (is_one_of(Shape, 'g', 'G') && FloatingPoint<T>) {
            action(ParseNumber<T, static_cast<usize>(CharsFormat::GENERAL)>());
        } else {
            static_assert(alignof(T) == 0, "No scan specification for T");
        }
        return *this;
    }

    Argument& nargs(usize num_args) {
        num_args_range = NArgsRange{num_args, num_args};
        return *this;
    }

    Argument& nargs(usize min, usize max) {
        num_args_range = NArgsRange{min, max};
        return *this;
    }

    Argument& nargs(NArgsPattern pattern) {
        switch (pattern) {
            case NArgsPattern::OPTIONAL:
                num_args_range = NArgsRange{0, 1};
                break;
            case NArgsPattern::ANY:
                num_args_range = NArgsRange{0, UnsignedSize::MAX_VALUE};
                break;
            case NArgsPattern::AT_LEAST_ONE:
                num_args_range = NArgsRange{1, UnsignedSize::MAX_VALUE};
                break;
            default:
                Ops::unreachable();
        }
        return *this;
    }

    Argument& remaining() {
        accepts_optional_like_value = true;
        return nargs(NArgsPattern::ANY);
    }

    template <typename T>
    void add_choice(T&& choice) {
        static_assert(ChoiceTypeSupported<T>, "Only string or integer type supported for choice");
        if (!choice_values.has_value()) {
            choice_values = Vector<String>{};
        }
        if constexpr (IsConvertibleValue<T, StringView>) {
            choice_values.value().push_back(String{StringView{Ops::forward<T>(choice)}});
        } else if constexpr (CanInvokeToString<T>) {
            choice_values.value().push_back(to_string(Ops::forward<T>(choice)));
        }
    }

    Argument& choices() throws (CommandLineParserException) {
        if (!choice_values.has_value()) {
            throw CommandLineParserException("Zero choices provided");
        }
        return *this;
    }

    template <typename T, typename... U>
    Argument& choices(T&& first, U&&... rest) throws (CommandLineParserException) {
        add_choice(Ops::forward<T>(first));
        choices(Ops::forward<U>(rest)...);
        return *this;
    }

    template <typename Iter>
    Iter consume(Iter start, Iter end, StringView name_used = {}, bool dry_run = false) throws (CommandLineParserException) {
        if (!is_repeatable && is_used) {
            throw CommandLineParserException(stdx::fmt::format("Duplicate argument {}", name_used));
        }
        this->used_name = name_used;
        usize passed_options = 0;

        if (choice_values.has_value()) {
            for (auto it = start; it != end; ++it) {
                if (is_value_in_choices(it)) {
                    passed_options += 1;
                    continue;
                }
                if (passed_options >= num_args_range.min() && passed_options <= num_args_range.max()) {
                    break;
                }
                throw_invalid_arguments(it);
            }
        }

        const usize num_args_max = choice_values.has_value() ? passed_options : num_args_range.max();
        const usize num_args_min = num_args_range.min();
        usize dist = 0;

        if (num_args_max == 0) {
            if (!dry_run) {
                values.emplace_back(implicit_val);
                for (auto& act: actions) {
                    visit([&](const auto& f) { f({}); }, act);
                }
                if (actions.empty()) {
                    visit([&](const auto& f) { f({}); }, default_action);
                }
                is_used = true;
            }
            return start;
        }

        if ((dist = static_cast<usize>(Iterators::distance(start, end))) >= num_args_min) {
            if (num_args_max < dist) {
                end = Iterators::next(start, static_cast<typename Iter::difference_type>(num_args_max));
            }
            if (!accepts_optional_like_value) {
                end = stdx::util::find_if(
                    start,
                    end,
                    [this](const StringView& arg) -> bool {
                        return check_optional(arg, prefix_chars);
                    }
                );
                dist = static_cast<usize>(Iterators::distance(start, end));
                if (dist < num_args_min) {
                    throw CommandLineParserException(stdx::fmt::format("Too few arguments for '{}'.", used_name));
                }
            }

            struct ActionApply {
                void operator()(ReturnableAction& f) {
                    for (Iter it = first; it != last; ++it) {
                        self.values.push_back(f(*it));
                    }
                }
                void operator()(VoidAction& f) {
                    for (Iter it = first; it != last; ++it) {
                        f(*it);
                    }
                    if (!self.default_val.has_value() && !self.accepts_optional_like_value) {
                        self.values.resize(static_cast<usize>(Iterators::distance(first, last)));
                    }
                }

                Iter first;
                Iter last;
                Argument& self;
            };

            if (!dry_run) {
                for (auto& act: actions) {
                    visit(ActionApply{start, end, *this}, act);
                }
                if (actions.empty()) {
                    visit(ActionApply{start, end, *this}, default_action);
                }
                is_used = true;
            }
            return end;
        }

        if (default_val.has_value()) {
            if (!dry_run) {
                is_used = true;
            }
            return start;
        }
        throw CommandLineParserException(stdx::fmt::format("Too few arguments for '{}'.", used_name));
    }

    void validate() const throws (CommandLineParserException) {
        if (is_optional) {
            if (!is_used && !default_val.has_value() && is_required) {
                throw_required_arg_not_used();
            }
            if (is_used && is_required && values.empty()) {
                throw_required_arg_no_value_provided();
            }
        } else {
            if (!num_args_range.contains(values.size()) && !default_val.has_value()) {
                throw_nargs_range_validation();
            }
        }
        if (choice_values.has_value()) {
            find_default_value_in_choices_or_throw();
        }
    }

    [[nodiscard]]
    String get_names_csv(char separator = ',') const {
        return stdx::util::accumulate(
            names.begin(),
            names.end(),
            ""s,
            [&](const String& result, const String& name) -> String {
                return result.empty() ? name : stdx::fmt::format("{}{}{}", result, separator, name);
            }
        );
    }

    [[nodiscard]]
    String get_usage_full() const {
        StringStream usage;
        usage << get_names_csv('/');
        const String mv = !meta_variable.empty() ? meta_variable : "VAR";
        if (num_args_range.max() > 0) {
            usage << stdx::fmt::format(" {}", mv);
            if (num_args_range.max() > 1) {
                usage << "...";
            }
        }
        return usage.str();
    }

    [[nodiscard]]
    String get_inline_usage() const {
        StringStream usage;
        String longest_name = names.front();
        for (const String& s: names) {
            if (s.size() > longest_name.size()) {
                longest_name = s;
            }
        }
        if (!is_required) {
            usage << "[";
        }
        usage << longest_name;
        const String mv = !meta_variable.empty() ? meta_variable : "VAR";
        if (num_args_range.max() > 0) {
            usage << stdx::fmt::format(" {}", mv);
            if (num_args_range.max() > 1 && meta_variable.find("> <") == String::npos) {
                usage << "...";
            }
        }
        if (!is_required) {
            usage << "]";
        }
        if (is_repeatable) {
            usage << "...";
        }
        return usage.str();
    }

    [[nodiscard]] usize get_arguments_length() const {
        usize names_size = stdx::util::accumulate(
            names.begin(),
            names.end(),
            0uz,
            [](const usize& sum, const String& s) -> usize {
                return sum + s.size();
            }
        );
        if (check_positional(names.front(), prefix_chars)) {
            if (!meta_variable.empty()) {
                return 2 + meta_variable.size();
            }
            return 2 + names_size + (names.size() - 1);
        }
        usize size = names_size + 2 * (names.size() - 1);
        if (!meta_variable.empty() && num_args_range == NArgsRange{1, 1}) {
            size += meta_variable.size() + 1;
        }
        return size + 2;
    }

    template <typename T>
    [[nodiscard]]
    T get() const throws (LogicException) {
        if (!values.empty()) {
            if constexpr (Collection<T>) {
                return any_cast_container<T>(values);
            } else {
                return any_cast<T>(values.front());
            }
        }
        if (default_val.has_value()) {
            return any_cast<T>(default_val);
        }
        if constexpr (Collection<T>) {
            if (!accepts_optional_like_value) {
                return any_cast_container<T>(values);
            }
        }
        throw LogicException(stdx::fmt::format("No value provided for '{}'.", names.back()));
    }

    template <typename T>
    [[nodiscard]]
    Optional<T> present() const throws (LogicException) {
        if (default_val.has_value()) {
            throw LogicException("Argument with default value always presents");
        }
        if (values.empty()) {
            return nullopt;
        }
        if constexpr (Collection<T>) {
            return any_cast_container<T>(values);
        }
        return any_cast<T>(values.front());
    }

    template <typename T> bool operator==(const T& rhs) const {
        if constexpr (!Collection<T>) {
            return get<T>() == rhs;
        } else {
            using ValueType = typename T::value_type;
            T lhs = get<T>();
            return stdx::util::equal(
                lhs.begin(),
                lhs.end(),
                rhs.begin(),
                rhs.end(),
                [](const auto& a, const auto& b) -> bool { return any_cast<const ValueType&>(a) == b; }
            );
        }
    }

    template <typename T>
    [[nodiscard]]
    bool operator!=(const T& rhs) const {
        return !(*this == rhs);
    }

    friend OutputStream& operator<<(OutputStream& stream, const Argument& arg) {
        StringStream name_stream;
        name_stream << "  ";
        if (check_positional(arg.names.front(), arg.prefix_chars)) {
            if (!arg.meta_variable.empty()) {
                name_stream << arg.meta_variable;
            } else {
                name_stream << join(arg.names.begin(), arg.names.end(), " "sv);
            }
        } else {
            name_stream << join(arg.names.begin(), arg.names.end(), ", "sv);
            if (!arg.meta_variable.empty() && arg.num_args_range == NArgsRange{1, 1}) {
                name_stream << stdx::fmt::format(" {}", arg.meta_variable);
            } else if (
                !arg.meta_variable.empty() &&
                arg.num_args_range.min() == arg.num_args_range.max() &&
                arg.meta_variable.find("> <") != String::npos
            ) {
                name_stream << stdx::fmt::format(" {}", arg.meta_variable);
            }
        }

        StreamSize stream_width = stream.width();
        String name_pad = String(name_stream.str().size(), ' ');
        String::size_type pos{};
        String::size_type prev_pos{};
        bool first_line = true;
        stream << name_stream.str();
        StringView hv(arg.help_text);
        while ((pos = arg.help_text.find('\n', prev_pos)) != String::npos) {
            StringView line = hv.substr(prev_pos, pos - prev_pos + 1);
            if (first_line) {
                stream << stdx::fmt::format("  {}", line);
                first_line = false;
            } else {
                stream.width(stream_width);
                stream << stdx::fmt::format("{}  {}", name_pad, line);
            }
            prev_pos += pos - prev_pos + 1;
        }
        if (first_line) {
            stream << stdx::fmt::format("  {}", arg.help_text);
        } else {
            StringView leftover = hv.substr(prev_pos, arg.help_text.size() - prev_pos);
            if (!leftover.empty()) {
                stream.width(stream_width);
                stream << stdx::fmt::format("{}  {}", name_pad, leftover);
            }
        }

        if (!arg.help_text.empty()) {
            stream << " ";
        }
        if (arg.num_args_range.is_exact()) {
            if (arg.num_args_range.min() != 0 && arg.num_args_range.min() != 1) {
                stream << stdx::fmt::format("[nargs: {}]", arg.num_args_range.min());
            }
        } else {
            if (!arg.num_args_range.is_right_bounded()) {
                stream << stdx::fmt::format("[nargs: {} or more]", arg.num_args_range.min());
            } else {
                stream << stdx::fmt::format("[nargs={}..{}]", arg.num_args_range.min(), arg.num_args_range.max());
            }
        }

        bool add_space = false;
        if (arg.default_val.has_value() && arg.num_args_range != NArgsRange{0, 0}) {
            stream << stdx::fmt::format("[default: {}]", arg.default_value_repr);
            add_space = true;
        } else if (arg.is_required) {
            stream << "[required]";
            add_space = true;
        }
        if (arg.is_repeatable) {
            if (add_space) {
                stream << " ";
            }
            stream << "[may be repeated]";
        }
        stream << "\n";
        return stream;
    }
};

/**
 * @class ParsedArguments
 * @brief Container for parsed argument values with type-safe access.
 */
class ParsedArguments {
private:
    HashMap<String, Any> values;
    HashMap<String, Vector<String>> multi_values;
public:
    template <typename T>
    [[nodiscard]]
    T get(const String& name) const {
        auto it = values.find(name);
        if (it == values.end()) {
            throw LogicException(stdx::fmt::format("No such argument: '{}'", name));
        }
        return any_cast<T>(it->second);
    }

    template <typename T>
    [[nodiscard]]
    T get_or(const String& name, const T& fallback) const {
        auto it = values.find(name);
        return it == values.end() ? fallback : any_cast<T>(it->second);
    }

    [[nodiscard]]
    Vector<String> get_multiple(const String& name) const {
        auto it = multi_values.find(name);
        return it == multi_values.end() ? Vector<String>{} : it->second;
    }

    [[nodiscard]]
    bool has(const String& name) const {
        return values.contains(name);
    }

    void set(const String& name, const Any& value) {
        values.insert_or_assign(name, value);
    }

    void set_multiple(const String& name, const Vector<String>& values) {
        multi_values.insert_or_assign(name, values);
    }

    Vector<String> get_argument_names() const {
        Vector<String> names;
        for (const auto& [key, val]: values) {
            names.push_back(key);
        }
        return names;
    }
};

/**
 * @class ArgumentParser
 * @brief Argument parser for modern C++.
 * Adapted from p-ranav/argparse (https://github.com/p-ranav/argparse)
 */
class ArgumentParser {
public:
    class MutuallyExclusiveGroup {
    private:
        ArgumentParser& parent;
        bool required = false;
        Vector<Argument*> elems;

        friend class ArgumentParser;
    public:
        MutuallyExclusiveGroup() = delete;
        explicit MutuallyExclusiveGroup(ArgumentParser& parent, bool required = false):
            parent{parent}, required{required} {}
        MutuallyExclusiveGroup(const MutuallyExclusiveGroup&) = delete;
        MutuallyExclusiveGroup& operator=(const MutuallyExclusiveGroup&) = delete;
        MutuallyExclusiveGroup(MutuallyExclusiveGroup&& other) noexcept:
            parent{other.parent}, required{other.required},
            elems{Ops::move(other.elems)} {
            other.elems.clear();
        }

        template <typename... Ts>
        Argument& add_argument(Ts... args) {
            Argument& argument = parent.add_argument(Ops::forward<Ts>(args)...);
            elems.push_back(&argument);
            return argument;
        }
    };

    using ArgumentIterator = LinkedList<Argument>::iterator;
    using SubparserIterator = LinkedList<ReferenceWrapper<ArgumentParser>>::iterator;
private:
    String prog_name;
    String ver;
    String desc;
    String epilog;
    bool exit_on_default_arguments = true;
    String prefix_chars= "-";
    String assign_chars= "=";
    bool is_parsed = false;
    LinkedList<Argument> positional_arguments;
    LinkedList<Argument> optional_arguments;
    TreeMap<String, ArgumentIterator> argument_map;
    String parser_path;
    LinkedList<ReferenceWrapper<ArgumentParser>> subparsers;
    TreeMap<String, SubparserIterator> subparser_map;
    TreeMap<String, bool> subparser_used;
    Vector<MutuallyExclusiveGroup> mutually_exclusive_groups;
    bool suppress = false;
    usize usage_max_line_width = UnsignedSize::MAX_VALUE;
    bool usage_break_on_mutex = false;
    u32 usage_newline_counter = 0;
    Vector<String> group_names;
protected:
    const MutuallyExclusiveGroup* get_belonging_mutex(const Argument* arg) const {
        for (const MutuallyExclusiveGroup& m: mutually_exclusive_groups) {
            if (stdx::util::find(m.elems.begin(), m.elems.end(), arg) != m.elems.end()) {
                return &m;
            }
        }
        return nullptr;
    }

    [[nodiscard]]
    bool is_valid_prefix_char(char c) const {
        return prefix_chars.find(c) != String::npos;
    }

    [[nodiscard]]
    char get_any_valid_prefix_char() const {
        return prefix_chars[0];
    }

    [[nodiscard]]
    Vector<String> preprocess_arguments(Span<const String> raw) const {
        Vector<String> args;
        for (const String& arg: raw) {
            auto apc = [this](const String& a) -> bool {
                if (a.empty()) {
                    return false;
                }
                auto lp = [this](char c) -> bool {
                    return prefix_chars.find(c) != String::npos;
                };
                if (lp('/')) {
                    return lp(a[0]);
                }
                return a.size() > 1 && lp(a[0]) && lp(a[1]);
            };
            auto pos = arg.find_first_of(assign_chars);
            if (argument_map.find(arg) == argument_map.end() && apc(arg) && pos != String::npos) {
                String opt = arg.substr(0, pos);
                if (argument_map.find(opt) != argument_map.end()) {
                    args.push_back(Ops::move(opt));
                    args.push_back(arg.substr(pos + 1));
                    continue;
                }
            }
            args.push_back(arg);
        }
        return args;
    }

    void parse_args_internal(Span<const String> raw_arguments) {
        Vector<String> arguments = preprocess_arguments(raw_arguments);
        if (prog_name.empty() && !arguments.empty()) {
            prog_name = arguments.front();
        }
        auto end = arguments.end();
        auto pos_it = positional_arguments.begin();

        for (auto it = Iterators::next(arguments.begin()); it != end;) {
            const String& cur = *it;
            if (Argument::check_positional(cur, prefix_chars)) {
                if (pos_it == positional_arguments.end()) {
                    if (auto sub_it = subparser_map.find(cur); sub_it != subparser_map.end()) {
                        is_parsed = true;
                        subparser_used[cur] = true;
                        return sub_it->second->get().parse_args(Vector<String>(it, end));
                    }
                    if (positional_arguments.empty()) {
                        if (!subparser_map.empty()) {
                            throw CommandLineParserException(
                                stdx::fmt::format("Failed to parse '{}', did you mean '{}'",
                                cur,
                                get_most_similar_string(subparser_map, cur))
                            );
                        }
                        throw CommandLineParserException("Zero positional arguments expected");
                    }
                    throw CommandLineParserException(stdx::fmt::format(
                        "Maximum number of positional arguments exceeded, failed to parse '{}'", cur));
                }
                auto argument = pos_it++;
                if (argument->num_args_range.min() == 1 &&
                    argument->num_args_range.max() == UnsignedSize::MAX_VALUE &&
                    pos_it != positional_arguments.end() &&
                    Iterators::next(pos_it) == positional_arguments.end() &&
                    pos_it->num_args_range.min() == 1 && pos_it->num_args_range.max() == 1) {
                    if (Iterators::next(it) != end) {
                        pos_it->consume(Iterators::prev(end), end);
                        end = Iterators::prev(end);
                    } else {
                        throw CommandLineParserException(stdx::fmt::format("Missing {}", pos_it->names.front()));
                    }
                }
                it = argument->consume(it, end);
                continue;
            }

            if (auto ami = argument_map.find(cur); ami != argument_map.end()) {
                it = ami->second->consume(Iterators::next(it), end, ami->first);
            } else if (cur.size() > 1 && is_valid_prefix_char(cur[0]) && !is_valid_prefix_char(cur[1])) {
                ++it;
                for (usize j = 1; j < cur.size(); j++) {
                    String ha{'-', cur[j]};
                    if (auto ami2 = argument_map.find(ha); ami2 != argument_map.end()) {
                        it = ami2->second->consume(it, end, ami2->first);
                    } else {
                        throw CommandLineParserException(stdx::fmt::format("Unknown argument: {}", cur));
                    }
                }
            } else {
                throw CommandLineParserException(stdx::fmt::format("Unknown argument: {}", cur));
            }
        }
        is_parsed = true;
    }

    Vector<String> parse_known_args_internal(Span<const String> raw_arguments) {
        Vector<String> arguments = preprocess_arguments(raw_arguments);
        Vector<String> unknown;
        if (prog_name.empty() && !arguments.empty()) {
            prog_name = arguments.front();
        }
        auto end = arguments.end();
        auto pos_it = positional_arguments.begin();

        for (auto it = Iterators::next(arguments.begin()); it != end;) {
            const String& cur = *it;
            if (Argument::check_positional(cur, prefix_chars)) {
                if (pos_it == positional_arguments.end()) {
                    auto sub_it = subparser_map.find(cur);
                    if (sub_it != subparser_map.end()) {
                        is_parsed = true;
                        subparser_used[cur] = true;
                        return sub_it->second->get()
                            .parse_known_args_internal(Vector<String>(it, end));
                    }
                    unknown.push_back(cur);
                    ++it;
                } else {
                    auto argument = pos_it++;
                    it = argument->consume(it, end);
                }
                continue;
            }
            if (auto ami = argument_map.find(cur); ami != argument_map.end()) {
                it = ami->second->consume(Iterators::next(it), end, ami->first);
            } else if (cur.size() > 1 && is_valid_prefix_char(cur[0]) && !is_valid_prefix_char(cur[1])) {
                ++it;
                for (usize j = 1; j < cur.size(); j++) {
                    String ha{'-', cur[j]};
                    if (auto ami2 = argument_map.find(ha); ami2 != argument_map.end()) {
                        it = ami2->second->consume(it, end, ami2->first);
                    } else {
                        unknown.push_back(cur);
                        break;
                    }
                }
            } else {
                unknown.push_back(cur);
                ++it;
            }
        }
        is_parsed = true;
        return unknown;
    }

    [[nodiscard]]
    usize length_of_longest_argument() const {
        if (argument_map.empty()) {
            return 0;
        }
        usize max_size = 0;
        for (const auto& [_, arg]: argument_map) {
            max_size = Math::max<usize>(max_size, arg->get_arguments_length());
        }
        for (const auto& [cmd, _]: subparser_map) {
            max_size = Math::max<usize>(max_size, cmd.size());
        }
        return max_size;
    }

    void index_argument(ArgumentIterator it) {
        for (const String& name: it->names) {
            argument_map.insert_or_assign(name, it);
        }
    }
public:
    explicit ArgumentParser(
        String program_name = {},
        String version = "1.0",
        DefaultArguments add_args = DefaultArguments::ALL,
        bool exit_on_default_arguments = true,
        OutputStream& os = Cout
    ):
        prog_name{Ops::move(program_name)}, ver{Ops::move(version)},
        exit_on_default_arguments{exit_on_default_arguments}, parser_path{prog_name} {
        if ((add_args & DefaultArguments::HELP) == DefaultArguments::HELP) {
            add_argument("-h", "--help")
                .action(
                    [&](const auto& _) -> void {
                        os << help().str();
                        if (exit_on_default_arguments) {
                            Environment::exit(0);
                        }
                    }
                )
                .default_value(false).help("shows help message and exits").implicit_value(true).nargs(0uz);
        }
        if ((add_args & DefaultArguments::VERSION) == DefaultArguments::VERSION) {
            add_argument("-v", "--version")
                .action([&](const auto& _) -> void {
                    os << stdx::fmt::format("{}\n", ver);
                    if (exit_on_default_arguments) {
                        Environment::exit(0);
                    }
                })
                .default_value(false)
                .help("prints version information and exits")
                .implicit_value(true)
                .nargs(0uz);
        }
    }

    ~ArgumentParser() = default;
    ArgumentParser(const ArgumentParser&) = delete;
    ArgumentParser& operator=(const ArgumentParser&) = delete;
    ArgumentParser(ArgumentParser&&) noexcept = delete;
    ArgumentParser& operator=(ArgumentParser&&) = delete;

    explicit operator bool() const {
        bool arg_used = stdx::util::any_of(
            argument_map.cbegin(),
            argument_map.cend(),
            [](auto& it) -> bool { return it.second->is_used; }
        );
        bool sub_used = stdx::util::any_of(
            subparser_used.cbegin(),
            subparser_used.cend(),
            [](auto& it) -> const bool { return it.second; }
        );
        return is_parsed && (arg_used || sub_used);
    }

    template <typename... Targs>
    Argument& add_argument(Targs... f_args) {
        using ArrayOfSv = Array<StringView, sizeof...(Targs)>;
        auto argument = optional_arguments.emplace(optional_arguments.cend(), prefix_chars, ArrayOfSv{f_args...});
        if (!argument->is_optional) {
            positional_arguments.splice(positional_arguments.cend(), optional_arguments, argument);
        }
        argument->set_usage_newline_counter(usage_newline_counter);
        argument->set_group_index(group_names.size());
        index_argument(argument);
        return *argument;
    }

    MutuallyExclusiveGroup& add_mutually_exclusive_group(bool required = false) {
        mutually_exclusive_groups.emplace_back(*this, required);
        return mutually_exclusive_groups.back();
    }

    ArgumentParser& add_usage_newline() {
        ++usage_newline_counter;
        return *this;
    }

    ArgumentParser& add_group(String group_name) {
        group_names.emplace_back(Ops::move(group_name));
        return *this;
    }

    ArgumentParser& add_description(String desc) {
        desc = Ops::move(desc);
        return *this;
    }

    ArgumentParser& add_epilog(String ep) {
        epilog = Ops::move(ep);
        return *this;
    }

    ArgumentParser& add_hidden_alias_for(Argument& arg, StringView alias) throws (LogicException) {
        for (auto it = optional_arguments.begin(); it != optional_arguments.end(); ++it) {
            if (&(*it) == &arg) {
                argument_map.insert_or_assign(String(alias), it);
                return *this;
            }
        }
        throw LogicException("Argument is not an optional argument of this parser");
    }

    template <typename T = Argument>
    [[nodiscard]]
    T& at(StringView name) throws (LogicException) {
        if constexpr (IsSameValue<T, Argument>) {
            return (*this)[name];
        } else {
            String str_name(name);
            if (auto it = subparser_map.find(str_name); it != subparser_map.end()) {
                return it->second->get();
            }
            throw LogicException(stdx::fmt::format("No such subparser: {}", str_name));
        }
    }

    ArgumentParser& set_prefix_chars(String pc) {
        prefix_chars = Ops::move(pc);
        return *this;
    }

    ArgumentParser& set_assign_chars(String ac) {
        assign_chars = Ops::move(ac);
        return *this;
    }

    void parse_args(Span<const String> arguments) throws (CommandLineParserException) {
        parse_args_internal(arguments);
        for (const auto& [_, argument]: argument_map) {
            argument->validate();
        }
        for (const MutuallyExclusiveGroup& group: mutually_exclusive_groups) {
            bool mutex_used = false;
            Argument* mutex_arg = nullptr;
            for (Argument* arg: group.elems) {
                if (!mutex_used && arg->is_used) {
                    mutex_used = true;
                    mutex_arg = arg;
                } else if (mutex_used && arg->is_used) {
                    throw CommandLineParserException(
                        stdx::fmt::format("Argument '{}' not allowed with '{}'",
                        arg->get_usage_full(),
                        mutex_arg->get_usage_full())
                    );
                }
            }
            if (!mutex_used && group.required) {
                String names;
                usize i = 0;
                for (Argument* arg: group.elems) {
                    names += stdx::fmt::format("'{}' ", arg->get_usage_full());
                    if (i + 1 != group.elems.size()) {
                        names += "or ";
                    }
                    ++i;
                }
                throw CommandLineParserException(stdx::fmt::format("One of the arguments {} is required", names));
            }
        }
    }

    void parse_args(Span<const StringView> arguments) throws (CommandLineParserException) {
        parse_args(Vector<String>(arguments.begin(), arguments.end()));
    }

    void parse_args(InitializerList<StringView> arguments) throws (CommandLineParserException) {
        parse_args(Vector<String>(arguments.begin(), arguments.end()));
    }

    void parse_args(ConvertibleTo<StringView> auto&&... arguments) throws (CommandLineParserException)
        requires (sizeof...(arguments) >= 1) {
        parse_args(Vector<String>{String(StringView(arguments))...});
    }

    [[nodiscard]]
    Vector<String> parse_known_args(Span<const String> arguments) throws (CommandLineParserException) {
        Vector<String> unknown = parse_known_args_internal(arguments);
        for (const auto& [_, argument]: argument_map) {
            argument->validate();
        }
        return unknown;
    }

    [[nodiscard]]
    Vector<String> parse_known_args(Span<const StringView> arguments) throws (CommandLineParserException) {
        return parse_known_args(Vector<String>(arguments.begin(), arguments.end()));
    }

    [[nodiscard]]
    Vector<String> parse_known_args(InitializerList<StringView> arguments) throws (CommandLineParserException) {
        return parse_known_args(Vector<String>(arguments.begin(), arguments.end()));
    }

    [[nodiscard]]
    Vector<String> parse_known_args(ConvertibleTo<StringView> auto&&... arguments) throws (CommandLineParserException)
        requires (sizeof...(arguments) >= 1) {
        return parse_known_args(Vector<String>{String(StringView(arguments))...});
    }

    void parse_args(i32 argc, const char* argv[]) throws (CommandLineParserException) {
        parse_args(Vector<String>(argv, argv + argc));
    }

    void parse_args(i32 argc, char* argv[]) throws (CommandLineParserException) {
        parse_args(Vector<String>(argv, argv + argc));
    }

    #ifdef __cpp_lib_reflection
    /**
     * @brief Reflect over the annotated struct @p T and parse argc/argv into a
     * fresh instance - the declarative counterpart to parse_args().
     *
     * Describe the arguments as the fields of @p T, optionally decorating them
     * with the ShortName / Description / Env annotations; see
     * argparse.annotations.inl for the field-to-flag and annotation conventions.
     *
     * @tparam T A reflectable, default-constructible struct describing the args.
     * @throws CommandLineParserException on a missing required argument, a
     * missing value, or an option given in both its long and short forms.
     * @throws InvalidArgumentException, InvalidRangeException if a value fails to
     * convert to a field's type.
     */
    template <ReflectableClass T>
    [[nodiscard]]
    [[=Throws<CommandLineParserException, InvalidArgumentException, InvalidRangeException>()]]
    static T parse(int argc, char* argv[]);

    /**
     * @brief Render usage text for the annotated struct @p T, one block per
     * field (flags, Description, friendly type, and default/optional markers).
     * @tparam T A reflectable, default-constructible struct describing the args.
     */
    template <ReflectableClass T>
    [[nodiscard]]
    static String help(int argc, char* argv[]);
    #endif

    [[nodiscard]]
    Vector<String> parse_known_args(i32 argc, const char* argv[]) throws (CommandLineParserException) {
        return parse_known_args(Vector<String>(argv, argv + argc));
    }

    [[nodiscard]]
    Vector<String> parse_known_args(i32 argc, char* argv[]) throws (CommandLineParserException) {
        return parse_known_args(Vector<String>(argv, argv + argc));
    }

    template <typename T = String>
    [[nodiscard]]
    T get(StringView arg_name) const throws (LogicException) {
        if (!is_parsed) {
            throw LogicException("Nothing parsed, no arguments are available.");
        }
        return (*this)[arg_name].template get<T>();
    }

    template <typename T = String>
    Optional<T> present(StringView arg_name) const {
        return (*this)[arg_name].template present<T>();
    }

    [[nodiscard]]
    bool is_used(StringView arg_name) const {
        return (*this)[arg_name].is_used;
    }

    [[nodiscard]]
    bool is_subcommand_used(StringView name) const {
        return subparser_used.at(String(name));
    }

    [[nodiscard]]
    bool is_subcommand_used(const ArgumentParser& sub) const {
        return is_subcommand_used(sub.prog_name);
    }

    Argument& operator[](StringView arg_name) const {
        String name(arg_name);
        auto it = argument_map.find(name);
        if (it != argument_map.end()) {
            return *(it->second);
        }
        if (!is_valid_prefix_char(arg_name.front())) {
            String prefix = String(1, get_any_valid_prefix_char());
            name = prefix + name;
            it = argument_map.find(name);
            if (it != argument_map.end()) {
                return *(it->second);
            }
            name = prefix + name;
            it = argument_map.find(name);
            if (it != argument_map.end()) {
                return *(it->second);
            }
        }
        throw LogicException(stdx::fmt::format("No such argument: {}", arg_name));
    }

    friend OutputStream& operator<<(OutputStream& stream, const ArgumentParser& parser) {
        stream.setf(FormatFlags::LEFT);
        usize longest = parser.length_of_longest_argument();
        stream << stdx::fmt::format("{}\n\n", parser.usage());
        if (!parser.desc.empty()) {
            stream << stdx::fmt::format("{}\n\n", parser.desc);
        }

        bool has_vis_pos = stdx::util::find_if(
            parser.positional_arguments.begin(),
            parser.positional_arguments.end(), [](const Argument& a) -> bool { return !a.is_hidden; }
        ) != parser.positional_arguments.end();
        if (has_vis_pos) {
            stream << "Positional arguments:\n";
        }
        for (const Argument& a: parser.positional_arguments) {
            if (!a.is_hidden) {
                stream.width(static_cast<long>(longest));
                stream << a;
            }
        }

        if (!parser.optional_arguments.empty()) {
            stream << stdx::fmt::format("{}{}", !has_vis_pos ? "" : "\n", "Optional arguments:\n");
        }
        for (const Argument& a: parser.optional_arguments) {
            if (a.group_index == 0 && !a.is_hidden) {
                stream.width(static_cast<long>(longest));
                stream << a;
            }
        }

        for (usize ig = 0; ig < parser.group_names.size(); ++ig) {
            stream << stdx::fmt::format("\n{} (detailed usage):\n", parser.group_names[ig]);
            for (const Argument& a: parser.optional_arguments) {
                if (a.group_index == ig + 1 && !a.is_hidden) {
                    stream.width(static_cast<long>(longest));
                    stream << a;
                }
            }
        }

        if (stdx::util::any_of(parser.subparser_map.begin(), parser.subparser_map.end(),
                [](auto& p) -> bool { return !p.second->get().suppress; })) {
            stream << stdx::fmt::format("{}{}", 
                parser.positional_arguments.empty() ? parser.optional_arguments.empty() ? "" : "\n" : "\n", 
                "Subcommands:\n"
            );
            for (const auto& [cmd, sub]: parser.subparser_map) {
                if (sub->get().suppress) {
                    continue;
                }
                stream << "  ";
                stream.width(static_cast<long>(longest - 2));
                stream << stdx::fmt::format("{} {}\n", cmd, sub->get().desc);
            }
        }
        if (!parser.epilog.empty()) {
            stream << stdx::fmt::format("\n{}\n\n", parser.epilog);
        }
        return stream;
    }

    [[nodiscard]]
    StringStream help() const {
        StringStream out;
        out << *this;
        return out;
    }

    ArgumentParser& set_usage_max_line_width(usize w) {
        usage_max_line_width = w;
        return *this;
    }

    ArgumentParser& set_usage_break_on_mutex() {
        usage_break_on_mutex = true;
        return *this;
    }

    [[nodiscard]]
    String usage() const {
        StringStream stream;
        String curline("Usage: ");
        curline += parser_path;
        const bool multiline = usage_max_line_width < UnsignedSize::MAX_VALUE;
        const usize indent = curline.size();

        const auto deal_with_group = [&](usize group_idx) -> bool {
            bool found = false;
            const MutuallyExclusiveGroup* cur_mutex = nullptr;
            i32 nl_counter = -1;
            for (const Argument& arg: optional_arguments) {
                if (arg.is_hidden) {
                    continue;
                }
                if (multiline) {
                    if (arg.group_index != group_idx) {
                        continue;
                    }
                    if (nl_counter != static_cast<i32>(arg.usage_newline_counter)) {
                        if (nl_counter >= 0 && curline.size() > indent) {
                            stream << stdx::fmt::format("{}\n", curline);
                            curline = String(indent, ' ');
                        }
                        nl_counter = static_cast<i32>(arg.usage_newline_counter);
                    }
                }
                found = true;
                const String iu = arg.get_inline_usage();
                const MutuallyExclusiveGroup* am = get_belonging_mutex(&arg);
                if (cur_mutex && !am) {
                    curline += ']';
                    if (usage_break_on_mutex) {
                        stream << stdx::fmt::format("{}\n", curline);
                        curline = String(indent, ' ');
                    }
                } else if (!cur_mutex && am) {
                    if ((usage_break_on_mutex && curline.size() > indent) || curline.size() + 3 + iu.size() > usage_max_line_width) {
                        stream << stdx::fmt::format("{}\n", curline);
                        curline = String(indent, ' ');
                    }
                    curline += " [";
                } else if (cur_mutex && am) {
                    if (cur_mutex != am) {
                        curline += ']';
                        if (usage_break_on_mutex || curline.size() + 3 + iu.size() > usage_max_line_width) {
                            stream << stdx::fmt::format("{}\n", curline);
                            curline = String(indent, ' ');
                        }
                        curline += " [";
                    } else {
                        curline += '|';
                    }
                }
                cur_mutex = am;
                if (curline.size() != indent && curline.size() + 1 + iu.size() > usage_max_line_width) {
                    stream << stdx::fmt::format("{}\n", curline);
                    curline = String(indent, ' ');
                    curline += " ";
                } else if (!cur_mutex) {
                    curline += " ";
                }
                curline += iu;
            }
            if (cur_mutex) {
                curline += ']';
            }
            return found;
        };

        const bool found = deal_with_group(0);
        if (found && multiline && !positional_arguments.empty()) {
            stream << stdx::fmt::format("{}\n", curline);
            curline = String(indent, ' ');
        }
        for (const Argument& arg: positional_arguments) {
            if (arg.is_hidden) {
                continue;
            }
            const String pa = !arg.meta_variable.empty() ? arg.meta_variable : arg.names.front();
            if (curline.size() + 1 + pa.size() > usage_max_line_width) {
                stream << stdx::fmt::format("{}\n", curline);
                curline = String(indent, ' ');
            }
            curline += " ";
            if (arg.num_args_range.min() == 0 && !arg.num_args_range.is_right_bounded()) {
                curline += stdx::fmt::format("[{}]...", pa);
            } else if (arg.num_args_range.min() == 1 && !arg.num_args_range.is_right_bounded()) {
                curline += stdx::fmt::format("{}...", pa);
            } else {
                curline += pa;
            }
        }
        if (multiline) {
            for (usize i = 0; i < group_names.size(); ++i) {
                stream << stdx::fmt::format("{}\n\n{}:\n", curline, group_names[i]);
                curline = String(indent, ' ');
                deal_with_group(i + 1);
            }
        }
        stream << curline;
        if (!subparser_map.empty()) {
            stream << " {";
            usize i = 0;
            for (const auto& [cmd, sub]: subparser_map) {
                if (sub->get().suppress) {
                    continue;
                }
                stream << stdx::fmt::format("{}{}", i == 0 ? "" : ",", cmd);
                ++i;
            }
            stream << "}";
        }
        return stream.str();
    }

    void add_subparser(ArgumentParser& parser) {
        parser.parser_path = stdx::fmt::format("{} {}", prog_name, parser.prog_name);
        auto it = subparsers.emplace(subparsers.cend(), parser);
        subparser_map.insert_or_assign(parser.prog_name, it);
        subparser_used.insert_or_assign(parser.prog_name, false);
    }

    void set_suppress(bool suppress) {
        this->suppress = suppress;
    }
};

}
