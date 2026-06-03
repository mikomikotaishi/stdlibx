#pragma once

/**
 * @namespace stdx::alloc
 * @brief Wrapper namespace for standard library allocation operations.
 */
export namespace stdx::alloc {
    template <typename Ptr>
    using PointerTraits = std::pointer_traits<Ptr>;

    template <typename Ptr>
    using AllocatorTraits = std::allocator_traits<Ptr>;

    #ifdef __cpp_lib_allocate_at_least
    template <typename Ptr, typename Size = usize>
    using AllocationResult = std::allocation_result<Ptr, Size>;
    #endif

    template <typename Ptr, typename Alloc>
    using UsesAllocator = std::uses_allocator<Ptr, Alloc>;

    template <typename T>
    using DefaultDelete = std::default_delete<T>;

    using AllocatorArgumentTag = std::allocator_arg_t;
    inline constexpr AllocatorArgumentTag AllocatorArgument = std::allocator_arg;

    using std::uses_allocator_construction_args;
    using std::make_obj_using_allocator;
    using std::uninitialized_construct_using_allocator;
}
