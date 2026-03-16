#pragma once

using alloc::text::string::CharTraits;

/**
 * @namespace aloc::pmr::text::string
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace alloc::pmr::text::string {
    template <typename Char, typename Traits = CharTraits<Char>>
    using BasicString = std::pmr::basic_string<Char, Traits>;

    using String = std::pmr::string;
    using Utf8String = std::pmr::u8string;
    using Utf16String = std::pmr::u16string;
    using Utf32String = std::pmr::u32string;
    using WideString = std::pmr::wstring;
}

/**
 * @namespace alloc::pmr
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace alloc::pmr {
    using ::alloc::pmr::text::string::BasicString;
    using ::alloc::pmr::text::string::String;
    using ::alloc::pmr::text::string::Utf8String;
    using ::alloc::pmr::text::string::Utf16String;
    using ::alloc::pmr::text::string::Utf32String;
    using ::alloc::pmr::text::string::WideString;
}
