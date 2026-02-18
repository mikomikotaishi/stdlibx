#pragma once

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
