/**
 * @file new_header.cppm
 * @module std:mem.new_header
 * @brief Module file for standard library dynamic memory management operations.
 *
 * This file contains the implementation of the dynamic memory management operations in the standard library.
 * It is named new_header.cppm rather than new.cppm due to C++ keyword limitations.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:mem.new_header;
#else
export module stdlib:mem.new_header;
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
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using alloc::mem::AlignVal_t;
    using alloc::mem::AlignValue_t;
    using alloc::mem::NoThrow_t;
    using alloc::mem::DestroyingDelete_t;
    #endif

    using alloc::mem::AlignValue;
    using alloc::mem::NewHandler;
    using alloc::mem::NoThrow;
    using alloc::mem::DestroyingDelete;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using alloc::mem::BadAllocException;
    #endif

    using alloc::mem::HARDWARE_CONSTRUCTIVE_INTERFERENCE_SIZE;
    using alloc::mem::HARDWARE_DESTRUCTIVE_INTERFERENCE_SIZE;

    using alloc::mem::BadAllocationException;
    using alloc::mem::BadArrayNewLengthException;

    using alloc::mem::get_new_handler;
    using alloc::mem::set_new_handler;
    
    using alloc::mem::launder;
}
