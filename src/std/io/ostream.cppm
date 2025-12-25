/**
 * @file ostream.cppm
 * @module std:io.ostream
 * @brief Module file for standard library output stream operations.
 *
 * This file contains the implementation of the output stream operations in the standard library.
 */

module;

#include <ostream>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:io.ostream;
#else
export module stdlib:io.ostream;
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
    using BasicOutputStream = std::basic_ostream<CharT>;

    using OStream = std::ostream;
    using WOStream = std::wostream;
    #endif

    template <typename CharT>
    using BasicOutputStream = std::basic_ostream<CharT>;

    using OutputStream = std::ostream;
    using WideOutputStream = std::wostream;

    using std::operator<<;

    using std::print;
    using std::println;
    using std::vprint_unicode;
    using std::vprint_nonunicode;

    using std::endl;
    using std::ends;
    using std::flush;
    using std::emit_on_flush;
    using std::noemit_on_flush;
    using std::flush_emit;
}
