#pragma once

/**
 * @namespace aloc::pmr::text::string
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace stdx::pmr::text::string {
    template <typename Char, typename Traits = CharTraits<Char>>
    using BasicString = std::pmr::basic_string<Char, Traits>;

    using String = std::pmr::string;
    using Utf8String = std::pmr::u8string;
    using Utf16String = std::pmr::u16string;
    using Utf32String = std::pmr::u32string;
    using WideString = std::pmr::wstring;
}

/**
 * @namespace stdx::pmr
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace stdx::pmr {
    using ::stdx::pmr::text::string::BasicString;
    using ::stdx::pmr::text::string::String;
    using ::stdx::pmr::text::string::Utf8String;
    using ::stdx::pmr::text::string::Utf16String;
    using ::stdx::pmr::text::string::Utf32String;
    using ::stdx::pmr::text::string::WideString;
}
