/**
 * @file string_view.cppm
 * @module std:text.string.string_view
 * @brief Module file for standard library string view operations.
 *
 * This file contains the implementation of the string view operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:text.string.string_view;
#else
export module stdlib:text.string.string_view;
#endif

import core;

/**
 * @namespace std::text::string
 * @brief Wrapper namespace for standard library string operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::text::string {
#else 
export namespace stdlib::text::string {
#endif
    using core::text::string::CharTraits;
    using core::text::string::BasicStringView;
    using core::text::string::StringView;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::text::string::U8StringView;
    using core::text::string::U16StringView;
    using core::text::string::U32StringView;
    using core::text::string::WStringView;
    #endif
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
 * @namespace std::literals::string_view_literals
 * @brief String view literals namespace
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::inline literals::inline string_view_literals {
#else 
export namespace stdlib::inline literals::inline string_view_literals {
#endif
    using core::literals::string_view_literals::operator""sv;
}

#ifdef __GLIBCXX__
export namespace __gnu_cxx {
    using __gnu_cxx::__normal_iterator;
    using __gnu_cxx::operator==;
    using __gnu_cxx::operator<=>;
}
#endif
