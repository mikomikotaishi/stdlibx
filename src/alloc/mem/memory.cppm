/**
 * @file memory.cppm
 * @module alloc:mem.memory
 * @brief Module file for standard library memory operations.
 *
 * This file contains the implementation of the memory operations in the standard library.
 */

module;

#include <memory>

export module alloc:mem.memory;

/**
 * @namespace alloc::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace alloc::mem {
    template <typename Ptr>
    using PointerTraits = std::pointer_traits<Ptr>;

    template <typename Ptr>
    using Allocator = std::allocator<Ptr>;

    template <typename Ptr>
    using AllocatorTraits = std::allocator_traits<Ptr>;

    template <typename Ptr, typename Alloc>
    using UsesAllocator = std::uses_allocator<Ptr, Alloc>;

    template <typename T>
    using DefaultDelete = std::default_delete<T>;

    using AllocatorArgumentTag = std::allocator_arg_t;
    inline constexpr AllocatorArgumentTag AllocatorArgument = std::allocator_arg;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename T, typename Deleter = DefaultDelete<T>>
    using UniquePtr = std::unique_ptr<T, Deleter>;

    template <typename T>
    using SharedPtr = std::shared_ptr<T>;

    template <typename T>
    using WeakPtr = std::weak_ptr<T>;
    #endif

    template <typename T, typename Deleter = DefaultDelete<T>>
    using UniquePointer = std::unique_ptr<T, Deleter>;

    template <typename T>
    using SharedPointer = std::shared_ptr<T>;

    template <typename T>
    using WeakPointer = std::weak_ptr<T>;

    template <typename T>
    using OwnerLess = std::owner_less<T>;

    template <typename T>
    using EnableSharedFromThis = std::enable_shared_from_this<T>;

    using std::hash;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename Smart, typename Pointer, typename... Args>
    using OutPtr_t = std::out_ptr_t<Smart, Pointer, Args...>;

    template <typename Smart, typename Pointer, typename... Args>
    using InOutPtr_t = std::inout_ptr_t<Smart, Pointer, Args...>;
    #endif

    template <typename Smart, typename Pointer, typename... Args>
    using OutputPointer = std::out_ptr_t<Smart, Pointer, Args...>;

    template <typename Smart, typename Pointer, typename... Args>
    using InOutPointer = std::inout_ptr_t<Smart, Pointer, Args...>;

    using BadWeakPointerException = std::bad_weak_ptr;

    using std::uses_allocator_construction_args;
    using std::make_obj_using_allocator;
    using std::uninitialized_construct_using_allocator;

    using std::to_address;
    using std::addressof;
    using std::align;
    using std::assume_aligned;

    using std::uninitialized_copy;
    using std::uninitialized_copy_n;
    using std::uninitialized_fill;
    using std::uninitialized_fill_n;
    using std::uninitialized_move;
    using std::uninitialized_move_n;
    using std::uninitialized_default_construct;
    using std::uninitialized_default_construct_n;
    using std::uninitialized_value_construct;
    using std::uninitialized_value_construct_n;
    using std::construct_at;
    using std::destroy_at;
    using std::destroy;
    using std::destroy_n;

    using std::make_unique;
    using std::make_unique_for_overwrite;
    using std::make_shared;
    using std::make_shared_for_overwrite;
    using std::allocate_shared;
    using std::allocate_shared_for_overwrite;
    using std::static_pointer_cast;
    using std::dynamic_pointer_cast;
    using std::const_pointer_cast;
    using std::reinterpret_pointer_cast;
    using std::get_deleter;

    using std::swap;

    using std::out_ptr;
    using std::inout_ptr;

    /**
     * @namespace ranges
     * @brief Wrapper namespace for standard library memory operations over ranges.
     */
    namespace ranges {
        using std::ranges::uninitialized_copy;
        using std::ranges::uninitialized_copy_n;
        using std::ranges::uninitialized_fill;
        using std::ranges::uninitialized_fill_n;
        using std::ranges::uninitialized_move;
        using std::ranges::uninitialized_move_n;
        using std::ranges::uninitialized_default_construct;
        using std::ranges::uninitialized_default_construct_n;
        using std::ranges::uninitialized_value_construct;
        using std::ranges::uninitialized_value_construct_n;
        using std::ranges::destroy;
        using std::ranges::destroy_n;
        using std::ranges::destroy_at;
        using std::ranges::construct_at;
    }
}
