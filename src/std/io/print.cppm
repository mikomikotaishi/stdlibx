/**
 * @file print.cppm
 * @module std:io.print
 * @brief Module file for standard library print operations.
 *
 * This file contains the implementation of the print operations in the standard library.
 */

module;

#if __has_include(<print>)
#include <print>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:io.print;
#else
export module stdlib:io.print;
#endif

export import :io.cstdio;

/**
 * @namespace std::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::io {
#else 
export namespace stdlib::io {
#endif
    #if __has_include(<print>)
    using std::print;
    using std::println;
    using std::vprint_unicode;
    using std::vprint_nonunicode;
    #endif
}
