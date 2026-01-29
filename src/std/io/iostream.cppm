/**
 * @file iostream.cppm
 * @module std:io.iostream
 * @brief Module file for standard library input/output stream operations.
 *
 * This file contains the implementation of the input/output stream operations in the standard library.
 */

module;

#include <iostream>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:io.iostream;
#else
export module stdlib:io.iostream;
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

    inline std::ostream& Cout = std::cout;
    inline std::istream& Cin = std::cin;
    inline std::ostream& Cerr = std::cerr;
    inline std::ostream& Clog = std::clog;


    inline std::wostream& WideCout = std::wcout;
    inline std::wistream& WideCin = std::wcin;
    inline std::wostream& WideCerr = std::wcerr;
    inline std::wostream& WideClog = std::wclog;
}
