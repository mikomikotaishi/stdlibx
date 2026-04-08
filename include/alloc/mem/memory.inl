#pragma once

using core::meta::RemoveExtentType;

/**
 * @namespace alloc::mem
 * @brief Wrapper namespace for standard library memory operations.
 */
export namespace alloc::mem {
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

    #ifdef __cpp_lib_out_ptr
    template <typename Smart, typename Pointer, typename... Args>
    using OutputPointer = std::out_ptr_t<Smart, Pointer, Args...>;

    template <typename Smart, typename Pointer, typename... Args>
    using InOutPointer = std::inout_ptr_t<Smart, Pointer, Args...>;
    #endif

    using BadWeakPointerException = std::bad_weak_ptr;

    #ifdef __cpp_lib_indirect
    template <typename T, typename Alloc = Allocator<T>>
    using Indirect = std::indirect<T, Alloc>;
    #endif

    #ifdef __cpp_lib_polymorphic
    template <typename T, typename Alloc = Allocator<T>>
    using Polymorphic = std::polymorphic<T, Alloc>;
    #endif

    using std::to_address;
    using std::addressof;
    using std::align;
    using std::assume_aligned;
    #ifdef __cpp_lib_is_sufficiently_aligned
    using std::is_sufficiently_aligned;
    #endif

    #ifdef __cpp_lib_start_lifetime_as
    using std::start_lifetime_as;
    using std::start_lifetime_as_array;
    #endif

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

    #ifdef __cpp_lib_out_ptr
    using std::out_ptr;
    using std::inout_ptr;
    #endif

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

    class Pointers final {
    public:
        Pointers() = delete;

        template <typename T>
        [[nodiscard]]
        static constexpr T* to_address(T* p) noexcept {
            return p;
        }

        template <typename Ptr>
        [[nodiscard]]
        static constexpr auto to_address(const Ptr& sp) noexcept {
            return std::to_address(sp);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr T* addressof(T& arg) noexcept {
            return std::addressof(arg);
        }

        template <typename T>
        [[nodiscard]]
        static const T* addressof(T&&) = delete;

        [[nodiscard]]
        void* align(usize alignment, usize size, void*& ptr, usize& space) noexcept {
            return std::align(alignment, size, ptr, space);
        }

        template <usize N, typename T>
        [[nodiscard]]
        static constexpr T* assume_aligned(T* p) noexcept {
            return std::assume_aligned<N>(p);
        }

        #ifdef __cpp_lib_is_sufficiently_aligned
        template <usize N, typename T>
        [[nodiscard]]
        static bool is_sufficiently_aligned(T* p) noexcept {
            return std::is_sufficiently_aligned<N>(p);
        }
        #endif

        template <typename T, typename... Args>
        [[nodiscard]]
        static constexpr UniquePointer<T> unique(Args&&... args) {
            return std::make_unique<T>(core::util::forward<Args>(args)...);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr UniquePointer<T> unique(usize size) {
            return std::make_unique<T>(size);
        }

        template <typename T>
        [[nodiscard]]
        static constexpr UniquePointer<T> unique_for_overwrite() {
            return std::make_unique_for_overwrite<T>();
        }

        template <typename T>
        [[nodiscard]]
        static constexpr UniquePointer<T> unique_for_overwrite(usize size) {
            return std::make_unique_for_overwrite<T>(size);
        }

        template <typename T, typename... Args>
        [[nodiscard]]
        static SharedPointer<T> shared(Args&&... args) {
            return std::make_shared<T>(core::util::forward<Args>(args)...);
        }

        template <typename T>
        [[nodiscard]]
        static SharedPointer<T> shared(usize size) {
            return std::make_shared<T>(size);
        }

        template <typename T>
        [[nodiscard]]
        static SharedPointer<T> shared() {
            return std::make_shared_for_overwrite<T>();
        }

        template <typename T>
        [[nodiscard]]
        static SharedPointer<T> shared(usize size, const RemoveExtentType<T>& u) {
            return std::make_shared_for_overwrite<T>(size, u);
        }

        template <typename T>
        [[nodiscard]]
        static SharedPointer<T> shared(const RemoveExtentType<T>& u) {
            return std::make_shared_for_overwrite<T>(u);
        }

        template <typename T>
        [[nodiscard]]
        static SharedPointer<T> shared_for_overwrite() {
            return std::make_shared_for_overwrite<T>();
        }

        template <typename T>
        [[nodiscard]]
        static SharedPointer<T> shared_for_overwrite(usize size) {
            return std::make_shared_for_overwrite<T>(size);
        }

        template <typename T, typename Alloc, typename... Args>
        [[nodiscard]]
        static SharedPointer<T> allocate_shared(const Alloc& alloc, Args&&... args) {
            return std::allocate_shared<T>(alloc, core::util::forward<Args>(args)...);
        }

        template <typename T, typename Alloc>
        [[nodiscard]]
        static SharedPointer<T> allocate_shared(const Alloc& alloc, usize size) {
            return std::allocate_shared<T>(alloc, size);
        }

        template <typename T, typename Alloc>
        [[nodiscard]]
        static SharedPointer<T> allocate_shared(const Alloc& alloc) {
            return std::allocate_shared<T>(alloc);
        }

        template <typename T, typename Alloc>
        [[nodiscard]]
        static SharedPointer<T> allocate_shared(const Alloc& alloc, usize size, const RemoveExtentType<T>& u) {
            return std::allocate_shared<T>(alloc, size, u);
        }

        template <typename T, typename Alloc>
        [[nodiscard]]
        static SharedPointer<T> allocate_shared(const Alloc& alloc, const RemoveExtentType<T>& u) {
            return std::allocate_shared<T>(alloc, u);
        }

        template <typename T, typename Alloc>
        [[nodiscard]]
        static SharedPointer<T> allocate_shared_for_overwrite(const Alloc& alloc) {
            return std::allocate_shared_for_overwrite<T>(alloc);
        }

        template <typename T, typename Alloc>
        [[nodiscard]]
        static SharedPointer<T> allocate_shared_for_overwrite(const Alloc& alloc, usize size) {
            return std::allocate_shared_for_overwrite<T>(alloc, size);
        }

        template <typename T, typename U>
        [[nodiscard]]
        static SharedPointer<T> static_pointer_cast(const SharedPointer<U>& sp) noexcept {
            return std::static_pointer_cast<T>(sp);
        }

        template <typename T, typename U>
        [[nodiscard]]
        static SharedPointer<T> static_pointer_cast(SharedPointer<U>&& sp) noexcept {
            return std::static_pointer_cast<T>(core::util::move(sp));
        }

        template <typename T, typename U>
        [[nodiscard]]
        static SharedPointer<T> dynamic_pointer_cast(const SharedPointer<U>& sp) noexcept {
            return std::dynamic_pointer_cast<T>(sp);
        }

        template <typename T, typename U>
        [[nodiscard]]
        static SharedPointer<T> dynamic_pointer_cast(SharedPointer<U>&& sp) noexcept {
            return std::dynamic_pointer_cast<T>(core::util::move(sp));
        }

        template <typename T, typename U>
        [[nodiscard]]
        static SharedPointer<T> const_pointer_cast(const SharedPointer<U>& sp) noexcept {
            return std::const_pointer_cast<T>(sp);
        }

        template <typename T, typename U>
        [[nodiscard]]
        static SharedPointer<T> const_pointer_cast(SharedPointer<U>&& sp) noexcept {
            return std::const_pointer_cast<T>(core::util::move(sp));
        }

        template <typename T, typename U>
        [[nodiscard]]
        static SharedPointer<T> reinterpret_pointer_cast(const SharedPointer<U>& sp) noexcept {
            return std::reinterpret_pointer_cast<T>(sp);
        }

        template <typename T, typename U>
        [[nodiscard]]
        static SharedPointer<T> reinterpret_pointer_cast(SharedPointer<U>&& sp) noexcept {
            return std::reinterpret_pointer_cast<T>(core::util::move(sp));
        }

        template <typename Del, typename T>
        [[nodiscard]]
        static Del* get_deleter(const SharedPointer<T>& sp) noexcept {
            return std::get_deleter<Del>(sp);
        }

        #ifdef __cpp_lib_out_ptr
        template <typename Ptr = void, typename Smart, typename... Args>
        [[nodiscard]]
        static auto out_ptr(Smart& s, Args&&... args) {
            return std::out_ptr<Ptr>(s, core::util::forward<Args>(args)...);
        }

        template <typename Ptr = void, typename Smart, typename... Args>
        [[nodiscard]]
        static auto inout_ptr(Smart& s, Args&&... args) {
            return std::inout_ptr<Ptr>(s, core::util::forward<Args>(args)...);
        }
        #endif

        #ifdef __cpp_lib_hazard_pointer
        
        #endif
    };
}
