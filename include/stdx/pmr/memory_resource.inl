#pragma once

/**
 * @namespace stdx::pmr
 * @brief Namespace for operations on polymorphic memory resources.
 */
export namespace stdx::pmr {
    using ::alloc::pmr::MemoryResource;
    using ::alloc::pmr::PoolOptions;
    using ::alloc::pmr::SynchronizedPoolResource;
    using ::alloc::pmr::UnsynchronizedPoolResource;
    using ::alloc::pmr::MonotoneBufferResource;

    using ::alloc::pmr::new_delete_resource;
    using ::alloc::pmr::null_memory_resource;
    using ::alloc::pmr::get_default_resource;
    using ::alloc::pmr::set_default_resource;

    using ::alloc::pmr::PolymorphicAllocator;
}
