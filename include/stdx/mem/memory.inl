#pragma once

/**
 * @namespace stdx::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace stdx::mem {
    using ::alloc::mem::UniquePointer;
    using ::alloc::mem::SharedPointer;
    using ::alloc::mem::WeakPointer;
    using ::alloc::mem::OwnerLess;
    using ::alloc::mem::EnableSharedFromThis;

    using ::alloc::mem::hash;

    #ifdef __cpp_lib_out_ptr
    using ::alloc::mem::OutputPointer;
    using ::alloc::mem::InOutPointer;
    #endif

    using ::alloc::mem::BadWeakPointerException;

    #ifdef __cpp_lib_indirect
    using ::alloc::mem::Indirect;
    #endif

    #ifdef __cpp_lib_polymorphic
    using ::alloc::mem::Polymorphic;
    #endif

    using ::alloc::mem::to_address;
    using ::alloc::mem::addressof;
    using ::alloc::mem::align;
    using ::alloc::mem::assume_aligned;
    #ifdef __cpp_lib_is_sufficiently_aligned
    using ::alloc::mem::is_sufficiently_aligned;
    #endif

    #ifdef __cpp_lib_start_lifetime_as
    using ::alloc::mem::start_lifetime_as;
    using ::alloc::mem::start_lifetime_as_array;
    #endif

    using ::alloc::mem::uninitialized_copy;
    using ::alloc::mem::uninitialized_copy_n;
    using ::alloc::mem::uninitialized_fill;
    using ::alloc::mem::uninitialized_fill_n;
    using ::alloc::mem::uninitialized_move;
    using ::alloc::mem::uninitialized_move_n;
    using ::alloc::mem::uninitialized_default_construct;
    using ::alloc::mem::uninitialized_default_construct_n;
    using ::alloc::mem::uninitialized_value_construct;
    using ::alloc::mem::uninitialized_value_construct_n;
    using ::alloc::mem::construct_at;
    using ::alloc::mem::destroy_at;
    using ::alloc::mem::destroy;
    using ::alloc::mem::destroy_n;

    using ::alloc::mem::make_unique;
    using ::alloc::mem::make_unique_for_overwrite;
    using ::alloc::mem::make_shared;
    using ::alloc::mem::make_shared_for_overwrite;
    using ::alloc::mem::allocate_shared;
    using ::alloc::mem::allocate_shared_for_overwrite;
    using ::alloc::mem::static_pointer_cast;
    using ::alloc::mem::dynamic_pointer_cast;
    using ::alloc::mem::const_pointer_cast;
    using ::alloc::mem::reinterpret_pointer_cast;
    using ::alloc::mem::get_deleter;

    using ::alloc::mem::swap;

    #ifdef __cpp_lib_out_ptr
    using ::alloc::mem::out_ptr;
    using ::alloc::mem::inout_ptr;
    #endif

    /**
     * @namespace ranges
     * @brief Wrapper namespace for standard library memory operations over ranges.
     */
    namespace ranges {
        using ::alloc::mem::ranges::uninitialized_copy;
        using ::alloc::mem::ranges::uninitialized_copy_n;
        using ::alloc::mem::ranges::uninitialized_fill;
        using ::alloc::mem::ranges::uninitialized_fill_n;
        using ::alloc::mem::ranges::uninitialized_move;
        using ::alloc::mem::ranges::uninitialized_move_n;
        using ::alloc::mem::ranges::uninitialized_default_construct;
        using ::alloc::mem::ranges::uninitialized_default_construct_n;
        using ::alloc::mem::ranges::uninitialized_value_construct;
        using ::alloc::mem::ranges::uninitialized_value_construct_n;
        using ::alloc::mem::ranges::destroy;
        using ::alloc::mem::ranges::destroy_n;
        using ::alloc::mem::ranges::destroy_at;
        using ::alloc::mem::ranges::construct_at;
    }

    using ::alloc::mem::Pointers;
}
