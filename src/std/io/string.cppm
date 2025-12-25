/**
 * @file string.cppm
 * @module std:io.string
 * @brief Module file for standard library string operations.
 *
 * This file contains the implementation of the string operations in the standard library.
 */

module;

#include <string>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:io.string;
#else
export module stdlib:io.string;
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
    using std::getline;
}
