/**
 * @file string_view.cppm
 * @module core:string_view
 * @brief Module file for importing the StringView class from the standard library.
 * 
 * This file exports the StringView class operations in the standard library.
 */

module;

#include "Macros.hpp"

export module core:string_view;

import :text.string.string_view;

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    using ::core::text::string::CharTraits;
    using ::core::text::string::BasicStringView;
    using ::core::text::string::StringView;
    using ::core::text::string::Utf8StringView;
    using ::core::text::string::Utf16StringView;
    using ::core::text::string::Utf32StringView;
    using ::core::text::string::WideStringView;
    
    using ::core::text::string::operator==;
    using ::core::text::string::operator<=>;
    using ::core::text::string::operator<<;
    using ::core::text::string::swap;
    using ::core::text::string::begin;
    using ::core::text::string::cbegin;
    using ::core::text::string::end;
    using ::core::text::string::cend;
    using ::core::text::string::rbegin;
    using ::core::text::string::crbegin;
    using ::core::text::string::rend;
    using ::core::text::string::crend;
    using ::core::text::string::size;
    using ::core::text::string::ssize;
    using ::core::text::string::empty;
    using ::core::text::string::data;

    using ::core::text::string::hash;
}

/**
 * @namespace core::literals::string_view_literals
 * @brief String view literals namespace
 */
export namespace core::inline literals::inline string_view_literals {
    using ::core::literals::string_view_literals::operator""sv;
}
