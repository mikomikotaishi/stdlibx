/**
 * @file csetjmp.cppm
 * @module stdx:sys.csetjmp
 * @brief Module file for standard library C-style jumping operations.
 *
 * This file contains the implementation of the C-style jumping operations in the standard library.
 */

module;

export module stdx:sys.csetjmp;

import core;

/**
 * @namespace stdx::sys
 * @brief Wrapper namespace for standard library system operations.
 */
export namespace stdx::sys {
    using core::sys::JumpBuffer;
    
    using core::sys::longjmp;
}
