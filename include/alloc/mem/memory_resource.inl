#pragma once

/**
 * @namespace alloc::mem
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace alloc::mem::pmr {
    using MemoryResource = std::pmr::memory_resource;
    using PoolOptions = std::pmr::pool_options;
    using SynchronizedPoolResource = std::pmr::synchronized_pool_resource;
    using UnsynchronizedPoolResource = std::pmr::unsynchronized_pool_resource;
    using MonotoneBufferResource = std::pmr::monotonic_buffer_resource;

    using std::pmr::new_delete_resource;
    using std::pmr::null_memory_resource;
    using std::pmr::get_default_resource;
    using std::pmr::set_default_resource;
}
