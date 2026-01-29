/**
 * @file string_view.cppm
 * @module core:text.string.string_view
 * @brief Module file for standard library string view operations.
 *
 * This file contains the implementation of the string view operations in the standard library.
 */

module;

#include <string_view>

export module core:text.string.string_view;

/**
 * @namespace core::text::string
 * @brief Wrapper namespace for standard library string operations.
 */
export namespace core::text::string {
    template <typename CharT>
    using CharTraits = std::char_traits<CharT>;

    template <typename CharT, typename Traits = CharTraits<CharT>>
    using BasicStringView = std::basic_string_view<CharT>;

    using StringView = std::string_view;

    using Utf8StringView = std::u8string_view;
    using Utf16StringView = std::u16string_view;
    using Utf32StringView = std::u32string_view;


    using WideStringView = std::wstring_view;

    using std::operator==;
    using std::operator<=>;
    using std::operator<<;
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

    using std::hash;
}

/**
 * @namespace core::literals::string_view_literals
 * @brief String view literals namespace
 */
export namespace core::inline literals::inline string_view_literals {
    using std::literals::string_view_literals::operator""sv;
}

#ifdef __GLIBCXX__
export namespace __gnu_cxx {
    using __gnu_cxx::__normal_iterator;
    using __gnu_cxx::operator==;
    using __gnu_cxx::operator<=>;
}
#endif
