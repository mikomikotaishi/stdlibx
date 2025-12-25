/**
 * @file cstdlib.cppm
 * @module core:sys.cstdlib
 * @brief Module file for standard library C-style system operations.
 *
 * This file contains the implementation of the C-style system operations in the standard library.
 */

module;

#include <cstdlib>

export module core:sys.cstdlib;

/**
 * @namespace core::sys
 * @brief Wrapper namespace for standard library system operations.
 */
export namespace core::sys {
    using std::abort;
    using std::exit;
    using std::quick_exit;
    using std::_Exit;
    using std::atexit;
    using std::at_quick_exit;
    using std::system;
    using std::getenv;
}
