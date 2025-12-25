/**
 * @file regex.cppm
 * @module alloc:text.regex
 * @brief Module file for standard library regular expression operations.
 *
 * This file contains the implementation of the regular expression operations in the standard library.
 */

module;

#include <regex>

export module alloc:text.regex;

import core;

import :mem.memory;

using core::iter::IteratorTraits;
using core::meta::IsSameValue;
using core::meta::UnderlyingTypeType;

using alloc::mem::Allocator;

/**
 * @namespace alloc::text
 * @brief Wrapper namespace for standard library text operations.
 */
export namespace alloc::text {
    template <typename T>
    using RegexTraits = std::regex_traits<T>;

    template <typename CharT, typename RxTraits = RegexTraits<CharT>>
        requires IsSameValue<CharT, typename RxTraits::char_type>
    using BasicRegex = std::basic_regex<CharT, RxTraits>;

    using Regex = std::regex;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using WRegex = std::wregex;
    #endif

    using WideRegex = std::wregex;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename T>
    using SubMatch = std::sub_match<T>;
    #endif

    template <typename T>
    using SubexpressionMatch = std::sub_match<T>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using CSubMatch = std::csub_match;
    using WCSubMatch = std::wcsub_match;
    using SSubMatch = std::ssub_match;
    using WSSubMatch = std::wssub_match;
    #endif

    using CSubexpressionMatch = std::csub_match;
    using WideCSubexpressionMatch = std::wcsub_match;
    using StringSubexpressionMatch = std::ssub_match;
    using WideStringSubexpressionMatch = std::wssub_match;

    template <typename BidirIt, typename Alloc = Allocator<SubexpressionMatch<BidirIt>>>
    using MatchResults = std::match_results<BidirIt, Alloc>;

    using CMatch = std::cmatch;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using WCMatch = std::wcmatch;
    using SMatch = std::smatch;
    using WSMatch = std::wsmatch;
    #endif

    using WideCMatch = std::wcmatch;
    using StringMatch = std::smatch;
    using WideStringMatch = std::wsmatch;

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        using CMatch = std::pmr::cmatch;

        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        using WCMatch = std::pmr::wcmatch;
        using SMatch = std::pmr::smatch;
        using WSMatch = std::pmr::wsmatch;
        #endif

        using WideCMatch = std::pmr::wcmatch;
        using StringMatch = std::pmr::smatch;
        using WideStringMatch = std::pmr::wsmatch;
    }

    template <typename BidirIt, typename CharT = IteratorTraits<BidirIt>::value_type, typename RxTraits = RegexTraits<CharT>>
    using RegexIterator = std::regex_iterator<BidirIt, CharT, RxTraits>;

    using CRegexIterator = std::cregex_iterator;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using WCRegexIterator = std::wcregex_iterator;
    using SRegexIterator = std::sregex_iterator;
    using WSRegexIterator = std::wsregex_iterator;
    #endif

    using WideCRegexIterator = std::wcregex_iterator;
    using StringRegexIterator = std::sregex_iterator;
    using WideStringRegexIterator = std::wsregex_iterator;

    template <typename T>
    using RegexTokenIterator = std::regex_token_iterator<T>;

    using CRegexTokenIterator = std::cregex_token_iterator;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using WCRegexTokenIterator = std::wcregex_token_iterator;
    using SRegexTokenIterator = std::sregex_token_iterator;
    using WSRegexTokenIterator = std::wsregex_token_iterator;
    #endif

    using WideCRegexTokenIterator = std::wcregex_token_iterator;
    using StringRegexTokenIterator = std::sregex_token_iterator;
    using WideStringRegexTokenIterator = std::wsregex_token_iterator;

    using RegexException = std::regex_error;

    using std::regex_match;
    using std::regex_search;
    using std::regex_replace;

    using std::swap;

    using std::begin;
    using std::cbegin;
    using std::end;
    using std::cend;
    using std::rbegin;
    using std::crbegin;
    using std::rend;
    using std::crend;
    using std::size;
    using std::ssize;
    using std::empty;
    using std::data;

    using std::operator==;
    using std::operator<=>;
    using std::operator<<;

    /**
     * @namespace regex_constants
     * @brief Namespace containing all constants used by standard library regex objects.
     */
    namespace regex_constants {
        using SyntaxOptionType = std::regex_constants::syntax_option_type;

        inline constexpr SyntaxOptionType ICASE = std::regex_constants::icase;
        inline constexpr SyntaxOptionType NOSUBS = std::regex_constants::nosubs;
        inline constexpr SyntaxOptionType OPTIMIZE = std::regex_constants::optimize;
        inline constexpr SyntaxOptionType COLLATE = std::regex_constants::collate;
        inline constexpr SyntaxOptionType ECMASCRIPT = std::regex_constants::ECMAScript;
        inline constexpr SyntaxOptionType BASIC = std::regex_constants::basic;
        inline constexpr SyntaxOptionType EXTENDED = std::regex_constants::extended;
        inline constexpr SyntaxOptionType AWK = std::regex_constants::awk;
        inline constexpr SyntaxOptionType GREP = std::regex_constants::grep;
        inline constexpr SyntaxOptionType EGREP = std::regex_constants::egrep;
        #ifdef __GNUC__
        inline constexpr SyntaxOptionType POLYNOMIAL = std::regex_constants::__polynomial;
        #endif
        inline constexpr SyntaxOptionType MULTILINE = std::regex_constants::multiline;

        [[nodiscard]]
        constexpr SyntaxOptionType operator&(SyntaxOptionType a, SyntaxOptionType b) noexcept {
            using Underlying = UnderlyingTypeType<std::regex_constants::syntax_option_type>;
            return static_cast<SyntaxOptionType>(
                static_cast<Underlying>(a) & static_cast<Underlying>(b)
            );
        }

        [[nodiscard]]
        constexpr SyntaxOptionType operator|(SyntaxOptionType a, SyntaxOptionType b) noexcept {
            using Underlying = UnderlyingTypeType<std::regex_constants::syntax_option_type>;
            return static_cast<SyntaxOptionType>(
                static_cast<Underlying>(a) | static_cast<Underlying>(b)
            );
        }

        [[nodiscard]]
        constexpr SyntaxOptionType operator^(SyntaxOptionType a, SyntaxOptionType b) noexcept {
            using Underlying = UnderlyingTypeType<std::regex_constants::syntax_option_type>;
            return static_cast<SyntaxOptionType>(
                static_cast<Underlying>(a) ^ static_cast<Underlying>(b)
            );
        }

        [[nodiscard]]
        constexpr SyntaxOptionType operator~(SyntaxOptionType a) noexcept {
            using Underlying = UnderlyingTypeType<std::regex_constants::syntax_option_type>;
            return static_cast<SyntaxOptionType>(~static_cast<Underlying>(a));
        }

        constexpr SyntaxOptionType& operator&=(SyntaxOptionType& a, SyntaxOptionType b) noexcept {
            using Underlying = UnderlyingTypeType<std::regex_constants::syntax_option_type>;
            return a = static_cast<SyntaxOptionType>(
                static_cast<Underlying>(a) & static_cast<Underlying>(b)
            );
        }

        constexpr SyntaxOptionType& operator|=(SyntaxOptionType& a, SyntaxOptionType b) noexcept {
            using Underlying = UnderlyingTypeType<std::regex_constants::syntax_option_type>;
            return a = static_cast<SyntaxOptionType>(
                static_cast<Underlying>(a) | static_cast<Underlying>(b)
            );
        }

        constexpr SyntaxOptionType& operator^=(SyntaxOptionType& a, SyntaxOptionType b) noexcept {
            using Underlying = UnderlyingTypeType<std::regex_constants::syntax_option_type>;
            return a = static_cast<SyntaxOptionType>(
                static_cast<Underlying>(a) ^ static_cast<Underlying>(b)
            );
        }

        using MatchFlagType = std::regex_constants::match_flag_type;

        inline constexpr MatchFlagType MATCH_DEFAULT = std::regex_constants::match_default;
        inline constexpr MatchFlagType MATCH_NOT_BOL = std::regex_constants::match_not_bol;
        inline constexpr MatchFlagType MATCH_NOT_EOL = std::regex_constants::match_not_eol;
        inline constexpr MatchFlagType MATCH_NOT_BOW = std::regex_constants::match_not_bow;
        inline constexpr MatchFlagType MATCH_ANY = std::regex_constants::match_any;
        inline constexpr MatchFlagType MATCH_NOT_NULL = std::regex_constants::match_not_null;
        inline constexpr MatchFlagType MATCH_CONTINUOUS = std::regex_constants::match_continuous;
        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        inline constexpr MatchFlagType MATCH_PREV_AVAIL = std::regex_constants::match_prev_avail;
        #endif
        inline constexpr MatchFlagType MATCH_PREVIOUS_AVAILABLE = std::regex_constants::match_prev_avail;
        inline constexpr MatchFlagType FORMAT_DEFAULT = std::regex_constants::format_default;
        inline constexpr MatchFlagType FORMAT_SED = std::regex_constants::format_sed;
        inline constexpr MatchFlagType FORMAT_NO_COPY = std::regex_constants::format_no_copy;
        inline constexpr MatchFlagType FORMAT_FIRST_ONLY = std::regex_constants::format_first_only;

        [[nodiscard]]
        constexpr MatchFlagType operator&(MatchFlagType a, MatchFlagType b) noexcept {
            using Underlying = UnderlyingTypeType<std::regex_constants::match_flag_type>;
            return static_cast<MatchFlagType>(
                static_cast<Underlying>(a) & static_cast<Underlying>(b)
            );
        }

        [[nodiscard]]
        constexpr MatchFlagType operator|(MatchFlagType a, MatchFlagType b) noexcept {
            using Underlying = UnderlyingTypeType<std::regex_constants::match_flag_type>;
            return static_cast<MatchFlagType>(
                static_cast<Underlying>(a) | static_cast<Underlying>(b)
            );
        }

        [[nodiscard]]
        constexpr MatchFlagType operator^(MatchFlagType a, MatchFlagType b) noexcept {
            using Underlying = UnderlyingTypeType<std::regex_constants::match_flag_type>;
            return static_cast<MatchFlagType>(
                static_cast<Underlying>(a) ^ static_cast<Underlying>(b)
            );
        }

        [[nodiscard]]
        constexpr MatchFlagType operator~(MatchFlagType a) noexcept {
            using Underlying = UnderlyingTypeType<std::regex_constants::match_flag_type>;
            return static_cast<MatchFlagType>(~static_cast<Underlying>(a));
        }

        constexpr MatchFlagType& operator&=(MatchFlagType& a, MatchFlagType b) noexcept {
            using Underlying = UnderlyingTypeType<std::regex_constants::match_flag_type>;
            return a = static_cast<MatchFlagType>(
                static_cast<Underlying>(a) & static_cast<Underlying>(b)
            );
        }

        constexpr MatchFlagType& operator|=(MatchFlagType& a, MatchFlagType b) noexcept {
            using Underlying = UnderlyingTypeType<std::regex_constants::match_flag_type>;
            return a = static_cast<MatchFlagType>(
                static_cast<Underlying>(a) | static_cast<Underlying>(b)
            );
        }

        constexpr MatchFlagType& operator^=(MatchFlagType& a, MatchFlagType b) noexcept {
            using Underlying = UnderlyingTypeType<std::regex_constants::match_flag_type>;
            return a = static_cast<MatchFlagType>(
                static_cast<Underlying>(a) ^ static_cast<Underlying>(b)
            );
        }

        using ErrorType = std::regex_constants::error_type;

        inline constexpr ErrorType ERROR_COLLATE = std::regex_constants::error_collate;
        inline constexpr ErrorType ERROR_CTYPE = std::regex_constants::error_ctype;
        inline constexpr ErrorType ERROR_ESCAPE = std::regex_constants::error_escape;
        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        inline constexpr ErrorType ERROR_BACKREF = std::regex_constants::error_backref;
        inline constexpr ErrorType ERROR_BRACK = std::regex_constants::error_brack;
        inline constexpr ErrorType ERROR_PAREN = std::regex_constants::error_paren;
        #endif
        inline constexpr ErrorType ERROR_BACKREFERENCE = std::regex_constants::error_backref;
        inline constexpr ErrorType ERROR_BRACKETS = std::regex_constants::error_brack;
        inline constexpr ErrorType ERROR_PARENTHESES = std::regex_constants::error_paren;
        inline constexpr ErrorType ERROR_BRACE = std::regex_constants::error_brace;
        inline constexpr ErrorType ERROR_BADBRACE = std::regex_constants::error_badbrace;
        inline constexpr ErrorType ERROR_RANGE = std::regex_constants::error_range;
        inline constexpr ErrorType ERROR_SPACE = std::regex_constants::error_space;
        inline constexpr ErrorType ERROR_BADREPEAT = std::regex_constants::error_badrepeat;
        inline constexpr ErrorType ERROR_COMPLEXITY = std::regex_constants::error_complexity;
        inline constexpr ErrorType ERROR_STACK = std::regex_constants::error_stack;
    }
}
