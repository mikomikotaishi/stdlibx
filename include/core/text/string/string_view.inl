#pragma once

/**
 * @namespace core::text::string
 * @brief Wrapper namespace for standard library string operations.
 */
export namespace core::text::string {
    template <typename Char>
    using CharTraits = std::char_traits<Char>;

    template <typename Char, typename Traits = CharTraits<Char>>
    using BasicStringView = std::basic_string_view<Char>;

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
