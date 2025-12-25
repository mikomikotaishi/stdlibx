/**
 * @file cstdlib.cppm
 * @module std:mem.cstdlib
 * @brief Module file for standard library C-style memory operations.
 *
 * This file contains the implementation of the C-style memory operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:mem.cstdlib;
#else
export module stdlib:mem.cstdlib;
#endif

import alloc;

/**
 * @namespace std::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::mem {
#else 
export namespace stdlib::mem {
#endif
    using alloc::mem::malloc;
    using alloc::mem::aligned_alloc;
    using alloc::mem::calloc;
    using alloc::mem::realloc;
    using alloc::mem::free;
}
