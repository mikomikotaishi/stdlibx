/**
 * @file memory_resource.cppm
 * @module stdx:mem.memory_resource
 * @brief Module file for standard library memory resource operations.
 *
 * This file contains the implementation of the memory resource operations in the standard library.
 */

module;

export module stdx:mem.memory_resource;

import alloc;

/**
 * @namespace stdx::mem::pmr
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace stdx::mem::pmr {
    using alloc::mem::pmr::PolymorphicAllocator;

    using alloc::mem::pmr::IMemoryResource;
    using alloc::mem::pmr::PoolOptions;
    using alloc::mem::pmr::SynchronizedPoolResource;
    using alloc::mem::pmr::UnsynchronizedPoolResource;
    using alloc::mem::pmr::MonotoneBufferResource;

    using alloc::mem::pmr::new_delete_resource;
    using alloc::mem::pmr::null_memory_resource;
    using alloc::mem::pmr::get_default_resource;
    using alloc::mem::pmr::set_default_resource;
}
