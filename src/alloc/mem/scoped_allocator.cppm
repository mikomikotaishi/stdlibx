/**
 * @file scoped_allocator.cppm
 * @module alloc:mem.scoped_allocator
 * @brief Module file for standard library scoped allocator operations.
 *
 * This file contains the implementation of the scoped allocator operations in the standard library.
 */

module;

#include <scoped_allocator>

export module alloc:mem.scoped_allocator;

/**
 * @namespace alloc::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace alloc::mem {
    template <typename OuterAlloc, typename... InnerAllocs>
    using ScopedAllocatorAdaptor = std::scoped_allocator_adaptor<OuterAlloc, InnerAllocs...>;
}
