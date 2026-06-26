#pragma once

using stdx::meta::UnderlyingTypeType;

/**
 * @namespace stdx::text::regex
 * @brief Standard library regular expressions operations.
 */
export namespace stdx::text::regex {
    template <typename T>
    using RegexTraits = std::regex_traits<T>;

    template <typename Char, typename RxTraits = RegexTraits<Char>>
        requires IsSameValue<Char, typename RxTraits::char_type>
    using BasicRegex = std::basic_regex<Char, RxTraits>;

    using Regex = std::regex;

    using WideRegex = std::wregex;

    template <typename T>
    using SubexpressionMatch = std::sub_match<T>;

    using CSubexpressionMatch = std::csub_match;
    using WideCSubexpressionMatch = std::wcsub_match;
    using StringSubexpressionMatch = std::ssub_match;
    using WideStringSubexpressionMatch = std::wssub_match;

    template <typename BidirIt, typename Alloc = Allocator<SubexpressionMatch<BidirIt>>>
    using MatchResults = std::match_results<BidirIt, Alloc>;

    using CMatch = std::cmatch;


    using WideCMatch = std::wcmatch;
    using StringMatch = std::smatch;
    using WideStringMatch = std::wsmatch;

    template <typename BidirIt, typename Char = IteratorTraits<BidirIt>::value_type, typename RxTraits = RegexTraits<Char>>
    using RegexIterator = std::regex_iterator<BidirIt, Char, RxTraits>;

    using CRegexIterator = std::cregex_iterator;


    using WideCRegexIterator = std::wcregex_iterator;
    using StringRegexIterator = std::sregex_iterator;
    using WideStringRegexIterator = std::wsregex_iterator;

    template <typename T>
    using RegexTokenIterator = std::regex_token_iterator<T>;

    using CRegexTokenIterator = std::cregex_token_iterator;

    using WideCRegexTokenIterator = std::wcregex_token_iterator;
    using StringRegexTokenIterator = std::sregex_token_iterator;
    using WideStringRegexTokenIterator = std::wsregex_token_iterator;

    using RegexException = std::regex_error;

    using std::regex_match;
    using std::regex_search;
    using std::regex_replace;

    class [[nodiscard]] SyntaxOption final {
    public:
        using Self = std::regex_constants::syntax_option_type;

        static constexpr Self NONE = std::regex_constants::syntax_option_type{};
        static constexpr Self ICASE = std::regex_constants::icase;
        static constexpr Self NOSUBS = std::regex_constants::nosubs;
        static constexpr Self OPTIMIZE = std::regex_constants::optimize;
        static constexpr Self COLLATE = std::regex_constants::collate;
        static constexpr Self ECMASCRIPT = std::regex_constants::ECMAScript;
        static constexpr Self BASIC = std::regex_constants::basic;
        static constexpr Self EXTENDED = std::regex_constants::extended;
        static constexpr Self AWK = std::regex_constants::awk;
        static constexpr Self GREP = std::regex_constants::grep;
        static constexpr Self EGREP = std::regex_constants::egrep;
        #ifdef __GNUC__
        static constexpr Self POLYNOMIAL = std::regex_constants::_S_polynomial;
        #endif
        static constexpr Self MULTILINE = std::regex_constants::multiline;
    private:
        // Intentionally not const to allow bitwise operators to modify it.
        Self value = NONE;
    public:
        constexpr SyntaxOption() noexcept = default;

        constexpr SyntaxOption(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }

        [[nodiscard]]
        constexpr SyntaxOption operator&(SyntaxOption other) const noexcept {
            using Underlying = UnderlyingTypeType<Self>;
            return SyntaxOption(static_cast<Self>(
                static_cast<Underlying>(value) & static_cast<Underlying>(other.value)
            ));
        }

        [[nodiscard]]
        constexpr SyntaxOption operator|(SyntaxOption other) const noexcept {
            using Underlying = UnderlyingTypeType<Self>;
            return SyntaxOption(static_cast<Self>(
                static_cast<Underlying>(value) | static_cast<Underlying>(other.value)
            ));
        }

        [[nodiscard]]
        constexpr SyntaxOption operator^(SyntaxOption other) const noexcept {
            using Underlying = UnderlyingTypeType<Self>;
            return SyntaxOption(static_cast<Self>(
                static_cast<Underlying>(value) ^ static_cast<Underlying>(other.value)
            ));
        }

        [[nodiscard]]
        constexpr SyntaxOption operator~() const noexcept {
            using Underlying = UnderlyingTypeType<Self>;
            return SyntaxOption(static_cast<Self>(~static_cast<Underlying>(value)));
        }

        constexpr SyntaxOption& operator&=(SyntaxOption other) noexcept {
            using Underlying = UnderlyingTypeType<Self>;
            value = static_cast<Self>(
                static_cast<Underlying>(value) & static_cast<Underlying>(other.value)
            );
            return *this;
        }

        constexpr SyntaxOption& operator|=(SyntaxOption other) noexcept {
            using Underlying = UnderlyingTypeType<Self>;
            value = static_cast<Self>(
                static_cast<Underlying>(value) | static_cast<Underlying>(other.value)
            );
            return *this;
        }

        constexpr SyntaxOption& operator^=(SyntaxOption other) noexcept {
            using Underlying = UnderlyingTypeType<Self>;
            value = static_cast<Self>(
                static_cast<Underlying>(value) ^ static_cast<Underlying>(other.value)
            );
            return *this;
        }
    };

    class [[nodiscard]] MatchFlag final {
    public:
        using Self = std::regex_constants::match_flag_type;

        static constexpr Self DEFAULT = std::regex_constants::match_default;
        static constexpr Self NOT_BOL = std::regex_constants::match_not_bol;
        static constexpr Self NOT_EOL = std::regex_constants::match_not_eol;
        static constexpr Self NOT_BOW = std::regex_constants::match_not_bow;
        static constexpr Self ANY = std::regex_constants::match_any;
        static constexpr Self NOT_NULL = std::regex_constants::match_not_null;
        static constexpr Self CONTINUOUS = std::regex_constants::match_continuous;
        static constexpr Self PREVIOUS_AVAILABLE = std::regex_constants::match_prev_avail;
        static constexpr Self FORMAT_DEFAULT = std::regex_constants::format_default;
        static constexpr Self FORMAT_SED = std::regex_constants::format_sed;
        static constexpr Self FORMAT_NO_COPY = std::regex_constants::format_no_copy;
        static constexpr Self FORMAT_FIRST_ONLY = std::regex_constants::format_first_only;
    private:
        // Intentionally not const to allow bitwise operators to modify it.
        Self value = DEFAULT;
    public:
        constexpr MatchFlag() noexcept = default;

        constexpr MatchFlag(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }

        [[nodiscard]]
        constexpr MatchFlag operator&(MatchFlag other) const noexcept {
            using Underlying = UnderlyingTypeType<Self>;
            return MatchFlag(static_cast<Self>(
                static_cast<Underlying>(value) & static_cast<Underlying>(other.value)
            ));
        }

        [[nodiscard]]
        constexpr MatchFlag operator|(MatchFlag other) const noexcept {
            using Underlying = UnderlyingTypeType<Self>;
            return MatchFlag(static_cast<Self>(
                static_cast<Underlying>(value) | static_cast<Underlying>(other.value)
            ));
        }

        [[nodiscard]]
        constexpr MatchFlag operator^(MatchFlag other) const noexcept {
            using Underlying = UnderlyingTypeType<Self>;
            return MatchFlag(static_cast<Self>(
                static_cast<Underlying>(value) ^ static_cast<Underlying>(other.value)
            ));
        }

        [[nodiscard]]
        constexpr MatchFlag operator~() const noexcept {
            using Underlying = UnderlyingTypeType<Self>;
            return MatchFlag(static_cast<Self>(~static_cast<Underlying>(value)));
        }

        constexpr MatchFlag& operator&=(MatchFlag other) noexcept {
            using Underlying = UnderlyingTypeType<Self>;
            value = static_cast<Self>(
                static_cast<Underlying>(value) & static_cast<Underlying>(other.value)
            );
            return *this;
        }

        constexpr MatchFlag& operator|=(MatchFlag other) noexcept {
            using Underlying = UnderlyingTypeType<Self>;
            value = static_cast<Self>(
                static_cast<Underlying>(value) | static_cast<Underlying>(other.value)
            );
            return *this;
        }

        constexpr MatchFlag& operator^=(MatchFlag other) noexcept {
            using Underlying = UnderlyingTypeType<Self>;
            value = static_cast<Self>(
                static_cast<Underlying>(value) ^ static_cast<Underlying>(other.value)
            );
            return *this;
        }
    };

    class [[nodiscard]] RegexParseError final {
    public:
        using Self = std::regex_constants::error_type;

        static constexpr Self NONE = std::regex_constants::error_type{};
        static constexpr Self COLLATE = std::regex_constants::error_collate;
        static constexpr Self CTYPE = std::regex_constants::error_ctype;
        static constexpr Self ESCAPE = std::regex_constants::error_escape;
        static constexpr Self BACKREFERENCE = std::regex_constants::error_backref;
        static constexpr Self BRACKETS = std::regex_constants::error_brack;
        static constexpr Self PARENTHESES = std::regex_constants::error_paren;
        static constexpr Self BRACE = std::regex_constants::error_brace;
        static constexpr Self BAD_BRACE = std::regex_constants::error_badbrace;
        static constexpr Self RANGE = std::regex_constants::error_range;
        static constexpr Self SPACE = std::regex_constants::error_space;
        static constexpr Self BAD_REPEAT = std::regex_constants::error_badrepeat;
        static constexpr Self COMPLEXITY = std::regex_constants::error_complexity;
        static constexpr Self STACK = std::regex_constants::error_stack;
    private:
        const Self value = NONE;
    public:
        constexpr RegexParseError() noexcept = default;

        constexpr RegexParseError(Self value) noexcept:
            value{value} {}

        constexpr operator Self() const noexcept {
            return value;
        }
    };
}
