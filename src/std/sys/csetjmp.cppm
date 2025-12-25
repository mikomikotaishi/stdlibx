/**
 * @file csetjmp.cppm
 * @module std:sys.csetjmp
 * @brief Module file for standard library C-style jumping operations.
 *
 * This file contains the implementation of the C-style jumping operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:sys.csetjmp;
#else
export module stdlib:sys.csetjmp;
#endif

import core;

/**
 * @namespace std::sys
 * @brief Wrapper namespace for standard library system operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::sys {
#else 
export namespace stdlib::sys {
#endif
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::sys::JmpBuf;
    #endif

    using core::sys::JumpBuffer;
    
    using core::sys::longjmp;
}
