/**
 * @file string.cppm
 * @module stdx:core.string
 * @brief Module file for importing the String class from the standard library.
 * 
 * This file exports the String class operations in the standard library.
 */

module;

#include "Macros.hpp"

export module stdx:core.string;

import alloc;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using alloc::CharTraits;
    using alloc::BasicString;
    using alloc::String;
    using alloc::Utf8String;
    using alloc::Utf16String;
    using alloc::Utf32String;
    using alloc::WideString;

    /**
     * @namespace pmr
     * @brief Namespace for operations on polymorphic memory resources.
     */
    namespace pmr {
        using alloc::pmr::BasicString;
        using alloc::pmr::String;
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
 * @namespace stdx::literals::string_literals
 * @brief String literals namespace
 */
export namespace stdx::inline literals::inline string_literals {
    using alloc::literals::string_literals::operator""s;
}

#ifdef __GLIBCXX__
export namespace __gnu_cxx {
    using __gnu_cxx::__normal_iterator;
    using __gnu_cxx::operator==;
    using __gnu_cxx::operator<=>;
}
#endif
