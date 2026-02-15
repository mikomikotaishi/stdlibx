/**
 * @file print.cppm
 * @module stdx:io.print
 * @brief Module file for standard library print operations.
 *
 * This file contains the implementation of the print operations in the standard library.
 */

module;

#if __has_include(<print>)
#include <print>
#endif

export module stdx:io.print;

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    #if __has_include(<print>)
    using std::print;
    using std::println;
    using std::vprint_unicode;
    using std::vprint_nonunicode;
    #endif
}
