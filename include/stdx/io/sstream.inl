#pragma once

using stdx::alloc::Allocator;
using stdx::text::CharTraits;

/**
 * @namespace stdx::io
 * @brief Standard library input/output operations.
 */
export namespace stdx::io {
    template <typename Char, typename Traits = CharTraits<Char>, typename Alloc = Allocator<Char>>
    using BasicStringBuffer = std::basic_stringbuf<Char, Traits, Alloc>;

    template <typename Char, typename Traits = CharTraits<Char>, typename Alloc = Allocator<Char>>
    using BasicStringStream = std::basic_stringstream<Char, Traits, Alloc>;

    template <typename Char, typename Traits = CharTraits<Char>, typename Alloc = Allocator<Char>>
    using BasicInputStringStream = std::basic_istringstream<Char, Traits, Alloc>;

    template <typename Char, typename Traits = CharTraits<Char>, typename Alloc = Allocator<Char>>
    using BasicOutputStringStream = std::basic_ostringstream<Char, Traits, Alloc>;

    using StringBuffer = std::stringbuf;
    using WideStringBuffer = std::wstringbuf;

    using StringStream = std::stringstream;

    using WideStringStream = std::wstringstream;
    using InputStringStream = std::istringstream;
    using WideInputStringStream = std::wistringstream;
    using OutputStringStream = std::ostringstream;
    using WideOutputStringStream = std::wostringstream;
}
