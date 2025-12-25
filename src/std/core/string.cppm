/**
 * @file string.cppm
 * @module std.core.string
 * @brief Module file for importing the String class from the standard library.
 * 
 * This file exports the String class operations in the standard library.
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:core.string;
#else
export module stdlib:core.string;
#endif

import alloc;

/**
 * @namespace std::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::core {
#else 
export namespace stdlib::core {
#endif
    using alloc::CharTraits;
    using alloc::BasicString;
    using alloc::String;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using alloc::U8String;
    using alloc::U16String;
    using alloc::U32String;
    #endif
    using alloc::Utf8String;
    using alloc::Utf16String;
    using alloc::Utf32String;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using alloc::WString;
    #endif
    using alloc::WideString;

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        using alloc::pmr::BasicString;
        using alloc::pmr::String;
        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        using alloc::pmr::U8String;
        using alloc::pmr::U16String;
        using alloc::pmr::U32String;
        using alloc::pmr::WString;
        #endif
        using alloc::pmr::Utf8String;
        using alloc::pmr::Utf16String;
        using alloc::pmr::Utf32String;
        using alloc::pmr::WideString;
    }
    
    using alloc::stoi;
    using alloc::stol;
    using alloc::stoll;
    using alloc::stoul;
    using alloc::stoull;
    using alloc::stof;
    using alloc::stod;
    using alloc::stold;
    using alloc::to_string;
    using alloc::to_wstring;

    using alloc::operator+;
    using alloc::operator==;
    using alloc::operator<=>;

    using alloc::erase;
    using alloc::erase_if;

    using alloc::begin;
    using alloc::cbegin;
    using alloc::end;
    using alloc::cend;
    using alloc::rbegin;
    using alloc::crbegin;
    using alloc::rend;
    using alloc::crend;
    using alloc::size;
    using alloc::ssize;
    using alloc::empty;
    using alloc::data;

    using alloc::hash;
    using alloc::swap;
}

/**
 * @namespace std::literals::string_literals
 * @brief String literals namespace
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::inline literals::inline string_literals {
#else 
export namespace stdlib::inline literals::inline string_literals {
#endif
    using alloc::literals::string_literals::operator""s;
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
