/**
 * @file istream.cppm
 * @module stdx:io.istream
 * @brief Module file for standard library input stream operations.
 *
 * This file contains the implementation of the input stream operations in the standard library.
 */

module;

#include <istream>

export module stdx:io.istream;

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    template <typename CharT>
    using BasicInputStream = std::basic_istream<CharT>;

    using InputStream = std::istream;
    using WideInputStream = std::wistream;

    template <typename CharT>
    using BasicIOStream = std::basic_iostream<CharT>;
    
    using IOStream = std::iostream;
    using WideIOStream = std::wiostream;
}
