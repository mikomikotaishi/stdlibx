#pragma once

using alloc::mem::Allocator;

/**
 * @namespace alloc::text::string
 * @brief Wrapper namespace for standard library string operations.
 */
export namespace alloc::text::string {
    using String = std::string;

    template <typename CharT>
    using CharTraits = std::char_traits<CharT>;

    template <typename CharT, typename Traits = CharTraits<CharT>, typename Alloc = Allocator<CharT>>
    using BasicString = std::basic_string<CharT, Traits, Alloc>;

    using Utf8String = std::u8string;
    using Utf16String = std::u16string;
    using Utf32String = std::u32string;
    using WideString = std::wstring;

    /**
        * @namespace pmr
        * @brief Namespace for operations on polymorphic memory resources.
        */
    namespace pmr {
        template <typename CharT, typename Traits = CharTraits<CharT>>
        using BasicString = std::pmr::basic_string<CharT, Traits>;

        using String = std::pmr::string;
        using Utf8String = std::pmr::u8string;
        using Utf16String = std::pmr::u16string;
        using Utf32String = std::pmr::u32string;
        using WideString = std::pmr::wstring;
    }

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
 * @namespace alloc::literals::string_literals
 * @brief String literals namespace
 */
export namespace alloc::inline literals::inline string_literals {
    using std::operator""s;
}

#ifdef __GLIBCXX__
export namespace __gnu_cxx {
    using __gnu_cxx::__normal_iterator;
    using __gnu_cxx::operator==;
    using __gnu_cxx::operator<=>;
}
#endif
