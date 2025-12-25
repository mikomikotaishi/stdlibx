/**
 * @file csetjmp.cppm
 * @module core:sys.csetjmp
 * @brief Module file for standard library C-style jumping operations.
 *
 * This file contains the implementation of the C-style jumping operations in the standard library.
 */

module;

#include <csetjmp>

export module core:sys.csetjmp;

/**
 * @namespace core::sys
 * @brief Wrapper namespace for standard library system operations.
 */
export namespace core::sys {
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using JmpBuf = std::jmp_buf;
    #endif

    using JumpBuffer = std::jmp_buf;
    
    using std::longjmp;
}
