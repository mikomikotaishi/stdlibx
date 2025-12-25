/**
 * @file streambuf.cppm
 * @module core:io.streambuf
 * @brief Module file for standard library stream buffer operations.
 *
 * This file contains the implementation of the stream buffer operations in the standard library.
 */

module;

#include <streambuf>

export module core:io.streambuf;

/**
 * @namespace core::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace core::io {
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename CharT>
    using BasicStreamBuf = std::basic_streambuf<CharT>;

    using StreamBuf = std::streambuf;
    using WStreamBuf = std::wstreambuf;
    #endif

    template <typename CharT>
    using BasicStreamBuffer = std::basic_streambuf<CharT>;

    using StreamBuffer = std::streambuf;
    using WideStreamBuffer = std::wstreambuf;

    using std::swap;
}
