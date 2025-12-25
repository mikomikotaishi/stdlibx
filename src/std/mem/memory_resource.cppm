/**
 * @file memory_resource.cppm
 * @module std:mem.memory_resource
 * @brief Module file for standard library memory resource operations.
 *
 * This file contains the implementation of the memory resource operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:mem.memory_resource;
#else
export module stdlib:mem.memory_resource;
#endif

import alloc;

/**
 * @namespace std::mem::pmr
 * @brief Namespace for operations on polymorphic memory resources.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::mem {
#else 
export namespace stdlib::mem {
#endif
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
