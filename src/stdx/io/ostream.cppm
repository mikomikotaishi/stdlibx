/**
 * @file ostream.cppm
 * @module stdx:io.ostream
 * @brief Module file for standard library output stream operations.
 *
 * This file contains the implementation of the output stream operations in the standard library.
 */

module;

#include <ostream>

export module stdx:io.ostream;

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
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
