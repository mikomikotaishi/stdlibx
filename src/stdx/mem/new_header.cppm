/**
 * @file new_header.cppm
 * @module stdx:mem.new_header
 * @brief Module file for standard library dynamic memory management operations.
 *
 * This file contains the implementation of the dynamic memory management operations in the standard library.
 * It is named new_header.cppm rather than new.cppm due to C++ keyword limitations.
 */

module;

export module stdx:mem.new_header;

import alloc;

/**
 * @namespace stdx::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace stdx::mem {
    using alloc::mem::AlignValue;
    using alloc::mem::NewHandler;
    using alloc::mem::NoThrow;
    using alloc::mem::DestroyingDelete;

    using alloc::mem::HARDWARE_CONSTRUCTIVE_INTERFERENCE_SIZE;
    using alloc::mem::HARDWARE_DESTRUCTIVE_INTERFERENCE_SIZE;

    using alloc::mem::BadAllocationException;
    using alloc::mem::BadArrayNewLengthException;

    using alloc::mem::get_new_handler;
    using alloc::mem::set_new_handler;
    
    using alloc::mem::launder;
}
