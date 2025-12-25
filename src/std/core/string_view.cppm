/**
 * @file string_view.cppm
 * @module std:core.string_view
 * @brief Module file for importing the StringView class from the standard library.
 * 
 * This file exports the StringView class operations in the standard library.
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:core.string_view;
#else
export module stdlib:core.string_view;
#endif

import core;

/**
 * @namespace std::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::core {
#else 
export namespace stdlib::core {
#endif
    using ::core::BasicStringView;
    using ::core::StringView;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using ::core::U8StringView;
    using ::core::U16StringView;
    using ::core::U32StringView;
    #endif
    using ::core::Utf8StringView;
    using ::core::Utf16StringView;
    using ::core::Utf32StringView;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using ::core::WStringView;
    #endif
    using ::core::WideStringView;

    using ::core::operator==;
    using ::core::operator<=>;
    using ::core::operator<<;

    using ::core::hash;
    using ::core::swap;
    
    using ::core::begin;
    using ::core::cbegin;
    using ::core::end;
    using ::core::cend;
    using ::core::rbegin;
    using ::core::crbegin;
    using ::core::rend;
    using ::core::crend;
    using ::core::size;
    using ::core::ssize;
    using ::core::empty;
    using ::core::data;
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
    using ::core::literals::string_view_literals::operator""sv;
}

#ifdef __GLIBCXX__
export namespace __gnu_cxx {
    using __gnu_cxx::__normal_iterator;
    using __gnu_cxx::operator==;
    using __gnu_cxx::operator<=>;
}
#endif

#if !defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_STD_MODULE_EXPORT_CORE();
#endif
