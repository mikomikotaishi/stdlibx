/**
 * @file new_header.cppm
 * @module alloc:mem.new_header
 * @brief Module file for standard library dynamic memory management operations.
 *
 * This file contains the implementation of the dynamic memory management operations in the standard library.
 * It is named new_header.cppm rather than new.cppm due to C++ keyword limitations.
 */

module;

#include <new>

export module alloc:mem.new_header;

/**
 * @namespace alloc::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace alloc::mem {
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using AlignVal_t = std::align_val_t;
    using AlignValue_t = std::align_val_t;
    using NoThrow_t = std::nothrow_t;
    using DestroyingDelete_t = std::destroying_delete_t;
    #endif

    using AlignValue = std::align_val_t;
    using NewHandler = std::new_handler;
    using NoThrowTag = std::nothrow_t;
    using DestroyingDeleteTag = std::destroying_delete_t;
    inline constexpr NoThrowTag NoThrow = std::nothrow;
    inline constexpr DestroyingDeleteTag DestroyingDelete = std::destroying_delete;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using BadAllocException = std::bad_alloc;
    #endif

    inline constexpr std::size_t HARDWARE_CONSTRUCTIVE_INTERFERENCE_SIZE = std::hardware_constructive_interference_size;
    inline constexpr std::size_t HARDWARE_DESTRUCTIVE_INTERFERENCE_SIZE = std::hardware_destructive_interference_size;

    using BadAllocationException = std::bad_alloc;
    using BadArrayNewLengthException = std::bad_array_new_length;

    using std::get_new_handler;
    using std::set_new_handler;
    
    using std::launder;
}
