/**
 * @file istream.cppm
 * @module std:io.istream
 * @brief Module file for standard library input stream operations.
 *
 * This file contains the implementation of the input stream operations in the standard library.
 */

module;

#include <istream>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:io.istream;
#else
export module stdlib:io.istream;
#endif

/**
 * @namespace std::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::io {
#else 
export namespace stdlib::io {
#endif
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename CharT>
    using BasicIStream = std::basic_istream<CharT>;

    using IStream = std::istream;
    using WIStream = std::wistream;
    using IOStream = std::iostream;
    using WIOStream = std::wiostream;
    #endif

    template <typename CharT>
    using BasicInputStream = std::basic_istream<CharT>;

    using InputStream = std::istream;
    using WideStream = std::wistream;

    template <typename CharT>
    using BasicIOStream = std::basic_iostream<CharT>;
    
    using IOStream = std::iostream;
    using WideIOStream = std::wiostream;
}
