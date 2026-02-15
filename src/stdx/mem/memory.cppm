/**
 * @file memory.cppm
 * @module stdx:mem.memory
 * @brief Module file for standard library memory operations.
 *
 * This file contains the implementation of the memory operations in the standard library.
 */

module;

export module stdx:mem.memory;

import alloc;

/**
 * @namespace stdx::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace stdx::mem {
    using alloc::mem::PointerTraits;
    using alloc::mem::Allocator;
    using alloc::mem::AllocatorTraits;
    using alloc::mem::UsesAllocator;

    using alloc::mem::UniquePointer;
    using alloc::mem::SharedPointer;
    using alloc::mem::WeakPointer;
    using alloc::mem::OwnerLess;
    using alloc::mem::EnableSharedFromThis;
    using alloc::mem::DefaultDelete;
    using alloc::mem::AllocatorArgumentTag;
    using alloc::mem::AllocatorArgument;

    using alloc::mem::hash;

    using alloc::mem::OutputPointer;
    using alloc::mem::InOutPointer;

    using alloc::mem::BadWeakPointerException;

    using alloc::mem::uses_allocator_construction_args;
    using alloc::mem::make_obj_using_allocator;
    using alloc::mem::uninitialized_construct_using_allocator;

    using alloc::mem::to_address;
    using alloc::mem::addressof;
    using alloc::mem::align;
    using alloc::mem::assume_aligned;

    using alloc::mem::uninitialized_copy;
    using alloc::mem::uninitialized_copy_n;
    using alloc::mem::uninitialized_fill;
    using alloc::mem::uninitialized_fill_n;
    using alloc::mem::uninitialized_move;
    using alloc::mem::uninitialized_move_n;
    using alloc::mem::uninitialized_default_construct;
    using alloc::mem::uninitialized_default_construct_n;
    using alloc::mem::uninitialized_value_construct;
    using alloc::mem::uninitialized_value_construct_n;
    using alloc::mem::construct_at;
    using alloc::mem::destroy_at;
    using alloc::mem::destroy;
    using alloc::mem::destroy_n;

    using alloc::mem::make_unique;
    using alloc::mem::make_unique_for_overwrite;
    using alloc::mem::make_shared;
    using alloc::mem::make_shared_for_overwrite;
    using alloc::mem::allocate_shared;
    using alloc::mem::allocate_shared_for_overwrite;
    using alloc::mem::static_pointer_cast;
    using alloc::mem::dynamic_pointer_cast;
    using alloc::mem::const_pointer_cast;
    using alloc::mem::reinterpret_pointer_cast;
    using alloc::mem::get_deleter;

    using alloc::mem::swap;

    using alloc::mem::out_ptr;
    using alloc::mem::inout_ptr;

    /**
     * @namespace ranges
     * @brief Wrapper namespace for standard library memory operations over ranges.
     */
    namespace ranges {
        using alloc::mem::ranges::uninitialized_copy;
        using alloc::mem::ranges::uninitialized_copy_n;
        using alloc::mem::ranges::uninitialized_fill;
        using alloc::mem::ranges::uninitialized_fill_n;
        using alloc::mem::ranges::uninitialized_move;
        using alloc::mem::ranges::uninitialized_move_n;
        using alloc::mem::ranges::uninitialized_default_construct;
        using alloc::mem::ranges::uninitialized_default_construct_n;
        using alloc::mem::ranges::uninitialized_value_construct;
        using alloc::mem::ranges::uninitialized_value_construct_n;
        using alloc::mem::ranges::destroy;
        using alloc::mem::ranges::destroy_n;
        using alloc::mem::ranges::destroy_at;
        using alloc::mem::ranges::construct_at;
    }
}
