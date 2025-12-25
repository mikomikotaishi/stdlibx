/**
 * @file sstream.cppm
 * @module std:io.sstream
 * @brief Module file for standard library string stream operations.
 *
 * This file contains the implementation of the string stream operations in the standard library.
 */

module;

#include <sstream>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:io.sstream;
#else
export module stdlib:io.sstream;
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
    using BasicStringBuf = std::basic_stringbuf<CharT>;
    #endif

    template <typename CharT>
    using BasicStringBuffer = std::basic_stringbuf<CharT>;

    template <typename CharT>
    using BasicStringStream = std::basic_stringstream<CharT>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename CharT>
    using BasicIStringStream = std::basic_istringstream<CharT>;

    template <typename CharT>
    using BasicOStringStream = std::basic_ostringstream<CharT>;

    using StringBuf = std::stringbuf;
    using WStringBuf = std::wstringbuf;
    #endif

    template <typename CharT>
    using BasicInputStringStream = std::basic_istringstream<CharT>;

    template <typename CharT>
    using BasicOutputStringStream = std::basic_ostringstream<CharT>;

    using StringBuffer = std::stringbuf;
    using WideStringBuffer = std::wstringbuf;

    using StringStream = std::stringstream;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using WStringStream = std::wstringstream;
    using IStringStream = std::istringstream;
    using WIStringStream = std::wistringstream;
    using OStringStream = std::ostringstream;
    using WOStringStream = std::wostringstream;
    #endif

    using WideStringStream = std::wstringstream;
    using InputStringStream = std::istringstream;
    using WideInputStringStream = std::wistringstream;
    using OutputStringStream = std::ostringstream;
    using WideOutputStringStream = std::wostringstream;

    using std::swap;
}
