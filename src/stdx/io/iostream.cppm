/**
 * @file iostream.cppm
 * @module stdx:io.iostream
 * @brief Module file for standard library input/output stream operations.
 *
 * This file contains the implementation of the input/output stream operations in the standard library.
 */

module;

#include <iostream>

export module stdx:io.iostream;

import :io.istream;
import :io.ostream;

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    inline OutputStream& Cout = std::cout;
    inline InputStream& Cin = std::cin;
    inline OutputStream& Cerr = std::cerr;
    inline OutputStream& Clog = std::clog;

    inline WideOutputStream& WideCout = std::wcout;
    inline WideInputStream& WideCin = std::wcin;
    inline WideOutputStream& WideCerr = std::wcerr;
    inline WideOutputStream& WideClog = std::wclog;
}
