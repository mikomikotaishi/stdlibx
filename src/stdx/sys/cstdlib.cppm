/**
 * @file cstdlib.cppm
 * @module stdx:sys.cstdlib
 * @brief Module file for standard library C-style system operations.
 *
 * This file contains the implementation of the C-style system operations in the standard library.
 */

module;

export module stdx:sys.cstdlib;

import core;

/**
 * @namespace stdx::sys
 * @brief Wrapper namespace for standard library system operations.
 */
export namespace stdx::sys {
    using core::sys::abort;
    using core::sys::exit;
    using core::sys::quick_exit;
    using core::sys::_Exit;
    using core::sys::atexit;
    using core::sys::at_quick_exit;
    using core::sys::system;
    using core::sys::getenv;
}
