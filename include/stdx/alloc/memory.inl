#pragma once

/**
 * @namespace stdx::alloc
 * @brief Wrapper namespace for standard library allocation operations.
 */
export namespace stdx::alloc {
    using ::alloc::PointerTraits;
    using ::alloc::Allocator;
    using ::alloc::AllocatorTraits;
    using ::alloc::UsesAllocator;
    using ::alloc::DefaultDelete;

    using ::alloc::AllocatorArgumentTag;
    using ::alloc::AllocatorArgument;

    using ::alloc::uses_allocator_construction_args;
    using ::alloc::make_obj_using_allocator;
    using ::alloc::uninitialized_construct_using_allocator;
}
