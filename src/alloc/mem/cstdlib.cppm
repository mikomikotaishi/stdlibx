/**
 * @file cstdlib.cppm
 * @module alloc:mem.cstdlib
 * @brief Module file for standard library C-style memory operations.
 *
 * This file contains the implementation of the C-style memory operations in the standard library.
 */

module;

#include <cstdlib>

export module alloc:mem.cstdlib;

/**
 * @namespace alloc::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace alloc::mem {
    using std::malloc;
    using std::aligned_alloc;
    using std::calloc;
    using std::realloc;
    using std::free;
}
