/**
 * @file scoped_allocator.cppm
 * @module stdx:mem.scoped_allocator
 * @brief Module file for standard library scoped allocator operations.
 *
 * This file contains the implementation of the scoped allocator operations in the standard library.
 */

module;

export module stdx:mem.scoped_allocator;

import alloc;

/**
 * @namespace stdx::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace stdx::mem {
    using alloc::mem::ScopedAllocatorAdaptor;
}
