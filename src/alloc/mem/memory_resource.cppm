/**
 * @file memory_resource.cppm
 * @module alloc:mem.memory_resource
 * @brief Module file for standard library memory resource operations.
 *
 * This file contains the implementation of the memory resource operations in the standard library.
 */

module;

#include <memory_resource>

export module alloc:mem.memory_resource;

/**
 * @namespace alloc::mem
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace alloc::mem::pmr {
    template <typename T>
    using PolymorphicAllocator = std::pmr::polymorphic_allocator<T>;

    using IMemoryResource = std::pmr::memory_resource;
    using PoolOptions = std::pmr::pool_options;
    using SynchronizedPoolResource = std::pmr::synchronized_pool_resource;
    using UnsynchronizedPoolResource = std::pmr::unsynchronized_pool_resource;
    using MonotoneBufferResource = std::pmr::monotonic_buffer_resource;

    using std::pmr::new_delete_resource;
    using std::pmr::null_memory_resource;
    using std::pmr::get_default_resource;
    using std::pmr::set_default_resource;
}
