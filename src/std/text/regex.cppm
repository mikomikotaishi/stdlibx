/**
 * @file regex.cppm
 * @module std:text.regex
 * @brief Module file for standard library regular expression operations.
 *
 * This file contains the implementation of the regular expression operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:text.regex;
#else
export module stdlib:text.regex;
#endif

import alloc;

/**
 * @namespace std::text
 * @brief Wrapper namespace for standard library text operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::text {
#else 
export namespace stdlib::text {
#endif
    using alloc::text::BasicRegex;
    using alloc::text::Regex;    
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using alloc::text::WRegex;
    #endif
    using alloc::text::WideRegex;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using alloc::text::SubMatch;
    #endif
    using alloc::text::SubexpressionMatch;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using alloc::text::CSubMatch;
    using alloc::text::WCSubMatch;
    using alloc::text::SSubMatch;
    using alloc::text::WSSubMatch;
    #endif
    using alloc::text::CSubexpressionMatch;
    using alloc::text::WideCSubexpressionMatch;
    using alloc::text::StringSubexpressionMatch;
    using alloc::text::WideStringSubexpressionMatch;
    using alloc::text::MatchResults;
    using alloc::text::CMatch;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using alloc::text::WCMatch;
    using alloc::text::SMatch;
    using alloc::text::WSMatch;
    #endif
    using alloc::text::WideCMatch;
    using alloc::text::StringMatch;
    using alloc::text::WideStringMatch;

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        using alloc::text::pmr::CMatch;
        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        using alloc::text::pmr::WCMatch;
        using alloc::text::pmr::SMatch;
        using alloc::text::pmr::WSMatch;
        #endif
        using alloc::text::pmr::WideCMatch;
        using alloc::text::pmr::StringMatch;
        using alloc::text::pmr::WideStringMatch;
    }

    using alloc::text::RegexIterator;
    using alloc::text::CRegexIterator;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using alloc::text::WCRegexIterator;
    using alloc::text::SRegexIterator;
    using alloc::text::WSRegexIterator;
    #endif
    using alloc::text::WideCRegexIterator;
    using alloc::text::StringRegexIterator;
    using alloc::text::WideStringRegexIterator;
    using alloc::text::RegexTokenIterator;
    using alloc::text::CRegexTokenIterator;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using alloc::text::WCRegexTokenIterator;
    using alloc::text::SRegexTokenIterator;
    using alloc::text::WideStringRegexTokenIterator;
    #endif
    using alloc::text::RegexTraits;
    using alloc::text::RegexException;

    using alloc::text::regex_match;
    using alloc::text::regex_search;
    using alloc::text::regex_replace;

    using alloc::text::swap;

    using alloc::text::begin;
    using alloc::text::cbegin;
    using alloc::text::end;
    using alloc::text::cend;
    using alloc::text::rbegin;
    using alloc::text::crbegin;
    using alloc::text::rend;
    using alloc::text::crend;
    using alloc::text::size;
    using alloc::text::ssize;
    using alloc::text::empty;
    using alloc::text::data;

    using alloc::text::operator==;
    using alloc::text::operator<=>;
    using alloc::text::operator<<;

    /**
     * @namespace regex_constants
     * @brief Namespace containing all constants used by standard library regex objects.
     */
    namespace regex_constants {
        using alloc::text::regex_constants::SyntaxOptionType;
        using alloc::text::regex_constants::ICASE;
        using alloc::text::regex_constants::NOSUBS;
        using alloc::text::regex_constants::OPTIMIZE;
        using alloc::text::regex_constants::COLLATE;
        using alloc::text::regex_constants::ECMASCRIPT;
        using alloc::text::regex_constants::BASIC;
        using alloc::text::regex_constants::EXTENDED;
        using alloc::text::regex_constants::AWK;
        using alloc::text::regex_constants::GREP;
        using alloc::text::regex_constants::EGREP;
        using alloc::text::regex_constants::POLYNOMIAL;
        using alloc::text::regex_constants::MULTILINE;

        using alloc::text::regex_constants::MatchFlagType;
        using alloc::text::regex_constants::MATCH_DEFAULT;
        using alloc::text::regex_constants::MATCH_NOT_BOL;
        using alloc::text::regex_constants::MATCH_NOT_EOL;
        using alloc::text::regex_constants::MATCH_NOT_BOW;
        using alloc::text::regex_constants::MATCH_ANY;
        using alloc::text::regex_constants::MATCH_NOT_NULL;
        using alloc::text::regex_constants::MATCH_CONTINUOUS;
        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        using alloc::text::regex_constants::MATCH_PREV_AVAIL;
        #endif
        using alloc::text::regex_constants::MATCH_PREVIOUS_AVAILABLE;
        using alloc::text::regex_constants::FORMAT_DEFAULT;
        using alloc::text::regex_constants::FORMAT_SED;
        using alloc::text::regex_constants::FORMAT_NO_COPY;
        using alloc::text::regex_constants::FORMAT_FIRST_ONLY;

        using alloc::text::regex_constants::ErrorType;
        using alloc::text::regex_constants::ERROR_COLLATE;
        using alloc::text::regex_constants::ERROR_CTYPE;
        using alloc::text::regex_constants::ERROR_ESCAPE;
        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        using alloc::text::regex_constants::ERROR_BACKREF;
        using alloc::text::regex_constants::ERROR_BRACK;
        using alloc::text::regex_constants::ERROR_PAREN;
        #endif
        using alloc::text::regex_constants::ERROR_BACKREFERENCE;
        using alloc::text::regex_constants::ERROR_BRACKETS;
        using alloc::text::regex_constants::ERROR_PARENTHESES;
        using alloc::text::regex_constants::ERROR_BRACE;
        using alloc::text::regex_constants::ERROR_BADBRACE;
        using alloc::text::regex_constants::ERROR_RANGE;
        using alloc::text::regex_constants::ERROR_SPACE;
        using alloc::text::regex_constants::ERROR_BADREPEAT;
        using alloc::text::regex_constants::ERROR_COMPLEXITY;
        using alloc::text::regex_constants::ERROR_STACK;
    }
}
