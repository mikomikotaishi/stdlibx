/**
 * @file cstdlib.cppm
 * @module stdx:mem.cstdlib
 * @brief Module file for standard library C-style memory operations.
 *
 * This file contains the implementation of the C-style memory operations in the standard library.
 */

module;

export module stdx:mem.cstdlib;

import alloc;

/**
 * @namespace stdx::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace stdx::mem {
    using alloc::mem::malloc;
    using alloc::mem::aligned_alloc;
    using alloc::mem::calloc;
    using alloc::mem::realloc;
    using alloc::mem::free;
}
