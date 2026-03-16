#pragma once

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    template <typename Char>
    using BasicStringBuffer = std::basic_stringbuf<Char>;

    template <typename Char>
    using BasicStringStream = std::basic_stringstream<Char>;

    template <typename Char>
    using BasicInputStringStream = std::basic_istringstream<Char>;

    template <typename Char>
    using BasicOutputStringStream = std::basic_ostringstream<Char>;

    using StringBuffer = std::stringbuf;
    using WideStringBuffer = std::wstringbuf;

    using StringStream = std::stringstream;

    using WideStringStream = std::wstringstream;
    using InputStringStream = std::istringstream;
    using WideInputStringStream = std::wistringstream;
    using OutputStringStream = std::ostringstream;
    using WideOutputStringStream = std::wostringstream;

    using std::swap;
}
