/**
 * @file sstream.cppm
 * @module stdx:io.sstream
 * @brief Module file for standard library string stream operations.
 *
 * This file contains the implementation of the string stream operations in the standard library.
 */

module;

#include <sstream>

export module stdx:io.sstream;

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    template <typename CharT>
    using BasicStringBuffer = std::basic_stringbuf<CharT>;

    template <typename CharT>
    using BasicStringStream = std::basic_stringstream<CharT>;

    template <typename CharT>
    using BasicInputStringStream = std::basic_istringstream<CharT>;

    template <typename CharT>
    using BasicOutputStringStream = std::basic_ostringstream<CharT>;

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
