#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::BasicStringView;
    using ::core::StringView;
    using ::core::Utf8StringView;
    using ::core::Utf16StringView;
    using ::core::Utf32StringView;
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
 * @namespace stdx::literals::string_view_literals
 * @brief String view literals namespace
 */
export namespace stdx::inline literals::inline string_view_literals {
    using ::core::literals::string_view_literals::operator""sv;
}

#ifdef __GLIBCXX__
export namespace __gnu_cxx {
    using __gnu_cxx::__normal_iterator;
    using __gnu_cxx::operator==;
    using __gnu_cxx::operator<=>;
}
#endif
