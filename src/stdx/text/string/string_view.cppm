/**
 * @file string_view.cppm
 * @module stdx:text.string.string_view
 * @brief Module file for standard library string view operations.
 *
 * This file contains the implementation of the string view operations in the standard library.
 */

module;

export module stdx:text.string.string_view;

import core;

/**
 * @namespace stdx::text::string
 * @brief Wrapper namespace for standard library string operations.
 */
export namespace stdx::text::string {
    using core::text::string::CharTraits;
    using core::text::string::BasicStringView;
    using core::text::string::StringView;
    using core::text::string::Utf8StringView;
    using core::text::string::Utf16StringView;
    using core::text::string::Utf32StringView;
    using core::text::string::WideStringView;

    using core::text::string::operator==;
    using core::text::string::operator<=>;
    using core::text::string::operator<<;

    using core::text::string::swap;
    using core::text::string::begin;
    using core::text::string::cbegin;
    using core::text::string::end;
    using core::text::string::cend;
    using core::text::string::rbegin;
    using core::text::string::crbegin;
    using core::text::string::rend;
    using core::text::string::crend;
    using core::text::string::size;
    using core::text::string::ssize;
    using core::text::string::empty;
    using core::text::string::data;
}

/**
 * @namespace stdx::literals::string_view_literals
 * @brief String view literals namespace
 */
export namespace stdx::inline literals::inline string_view_literals {
    using core::literals::string_view_literals::operator""sv;
}

#ifdef __GLIBCXX__
export namespace __gnu_cxx {
    using __gnu_cxx::__normal_iterator;
    using __gnu_cxx::operator==;
    using __gnu_cxx::operator<=>;
}
#endif
