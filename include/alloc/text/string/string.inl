#pragma once

/**
 * @namespace alloc::text
 * @brief Wrapper namespace for standard library text operations.
 */
export namespace alloc::text {
    template <typename Char>
    using CharTraits = std::char_traits<Char>;
}

/**
 * @namespace alloc::text::string
 * @brief Wrapper namespace for standard library string operations.
 */
export namespace alloc::text::string {
    using String = std::string;

    template <typename Char, typename Traits = CharTraits<Char>, typename Alloc = Allocator<Char>>
    using BasicString = std::basic_string<Char, Traits, Alloc>;

    using Utf8String = std::u8string;
    using Utf16String = std::u16string;
    using Utf32String = std::u32string;
    using WideString = std::wstring;

    using std::stoi;
    using std::stol;
    using std::stoll;
    using std::stoul;
    using std::stoull;
    using std::stof;
    using std::stod;
    using std::stold;
    using std::to_string;
    using std::to_wstring;

    using std::operator+;
    using std::operator==;
    using std::operator<=>;

    using std::erase;
    using std::erase_if;

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
    using std::swap;
}

/**
 * @namespace core::literals::string_literals
 * @brief String literals namespace
 */
export namespace core::inline literals::inline string_literals {
    using std::operator""s;
}
