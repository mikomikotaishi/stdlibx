#pragma once

/**
 * @namespace stdx::text::regex
 * @brief Wrapper namespace for standard library regular expressions operations.
 */
export namespace stdx::text::regex {
    using ::alloc::text::regex::BasicRegex;
    using ::alloc::text::regex::Regex;    
    using ::alloc::text::regex::WideRegex;
    using ::alloc::text::regex::SubexpressionMatch;
    using ::alloc::text::regex::CSubexpressionMatch;
    using ::alloc::text::regex::WideCSubexpressionMatch;
    using ::alloc::text::regex::StringSubexpressionMatch;
    using ::alloc::text::regex::WideStringSubexpressionMatch;
    using ::alloc::text::regex::MatchResults;
    using ::alloc::text::regex::CMatch;
    using ::alloc::text::regex::WideCMatch;
    using ::alloc::text::regex::StringMatch;
    using ::alloc::text::regex::WideStringMatch;

    using ::alloc::text::regex::RegexIterator;
    using ::alloc::text::regex::CRegexIterator;
    using ::alloc::text::regex::WideCRegexIterator;
    using ::alloc::text::regex::StringRegexIterator;
    using ::alloc::text::regex::WideStringRegexIterator;
    using ::alloc::text::regex::RegexTokenIterator;
    using ::alloc::text::regex::CRegexTokenIterator;
    using ::alloc::text::regex::RegexTraits;
    using ::alloc::text::regex::RegexException;

    using ::alloc::text::regex::regex_match;
    using ::alloc::text::regex::regex_search;
    using ::alloc::text::regex::regex_replace;

    using ::alloc::text::regex::swap;

    using ::alloc::text::regex::begin;
    using ::alloc::text::regex::cbegin;
    using ::alloc::text::regex::end;
    using ::alloc::text::regex::cend;
    using ::alloc::text::regex::rbegin;
    using ::alloc::text::regex::crbegin;
    using ::alloc::text::regex::rend;
    using ::alloc::text::regex::crend;
    using ::alloc::text::regex::size;
    using ::alloc::text::regex::ssize;
    using ::alloc::text::regex::empty;
    using ::alloc::text::regex::data;

    using ::alloc::text::regex::operator==;
    using ::alloc::text::regex::operator<=>;
    using ::alloc::text::regex::operator<<;

    using ::alloc::text::regex::SyntaxOption;
    using ::alloc::text::regex::MatchFlag;
    using ::alloc::text::regex::RegexParseError;
}
