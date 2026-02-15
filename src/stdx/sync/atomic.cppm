/**
 * @file atomic.cppm
 * @module stdx:sync.atomic
 * @brief Module file for standard library atomic operations.
 *
 * This file contains the implementation of the atomic operations in the standard library.
 */

module;

#include <atomic>

export module stdx:sync.atomic;

/**
 * @namespace stdx::sync
 * @brief Wrapper namespace for standard library synchronisation operations.
 */
export namespace stdx::sync {
    // Atomic template class
    template <typename T>
    using Atomic = std::atomic<T>;

    // Atomic reference template class
    template <typename T>
    using AtomicReference = std::atomic_ref<T>;
    
    // Memory order enum
    using MemoryOrder = std::memory_order;
    
    // Memory order constants

    inline constexpr MemoryOrder MEMORY_ORDER_RELAXED = std::memory_order_relaxed;
    inline constexpr MemoryOrder MEMORY_ORDER_CONSUME = std::memory_order_consume;
    inline constexpr MemoryOrder MEMORY_ORDER_ACQUIRE = std::memory_order_acquire;
    inline constexpr MemoryOrder MEMORY_ORDER_RELEASE = std::memory_order_release;
    inline constexpr MemoryOrder MEMORY_ORDER_ACQUIRE_RELEASE = std::memory_order_acq_rel;
    inline constexpr MemoryOrder MEMORY_ORDER_SEQUENTIALLY_CONSISTENT = std::memory_order_seq_cst;
    
    // Atomic flag
    using AtomicFlag = std::atomic_flag;
    
    // Basic atomic typedefs

    using AtomicBoolean = std::atomic_bool;
    using AtomicCharacter = std::atomic_char;
    using AtomicSignedChararacter = std::atomic_schar;
    using AtomicUnsignedCharacter = std::atomic_uchar;
    using AtomicShort = std::atomic_short;
    using AtomicUnsignedShort = std::atomic_ushort;
    using AtomicInteger = std::atomic_int;
    using AtomicUnsignedInteger = std::atomic_uint;
    using AtomicLong = std::atomic_long;
    using AtomicUnsignedLong = std::atomic_ulong;
    using AtomicLongLong = std::atomic_llong;
    using AtomicUnsignedLongLong = std::atomic_ullong;
    using AtomicCharacter8 = std::atomic_char8_t;
    using AtomicCharacter16 = std::atomic_char16_t;
    using AtomicCharacter32 = std::atomic_char32_t;
    using AtomicWideCharacter = std::atomic_wchar_t;
    
    // Integer type atomics

    using AtomicInteger8 = std::atomic_int8_t;
    using AtomicUnsignedInteger8 = std::atomic_uint8_t;
    using AtomicInteger16 = std::atomic_int16_t;
    using AtomicUnsignedInteger16 = std::atomic_uint16_t;
    using AtomicInteger32 = std::atomic_int32_t;
    using AtomicUnsignedInteger32 = std::atomic_uint32_t;
    using AtomicInteger64 = std::atomic_int64_t;
    using AtomicUnsignedInteger64 = std::atomic_uint64_t;
    
    // Least/fast atomic integer types

    using AtomicIntegerLeast8 = std::atomic_int_least8_t;
    using AtomicUnsignedIntegerLeast8 = std::atomic_uint_least8_t;
    using AtomicIntegerLeast16 = std::atomic_int_least16_t;
    using AtomicUnsignedIntegerLeast16 = std::atomic_uint_least16_t;
    using AtomicIntegerLeast32 = std::atomic_int_least32_t;
    using AtomicUnsignedIntegerLeast32 = std::atomic_uint_least32_t;
    using AtomicIntegerLeast64 = std::atomic_int_least64_t;
    using AtomicUnsignedIntegerLeast64 = std::atomic_uint_least64_t;
    
    using AtomicIntegerFast8 = std::atomic_int_fast8_t;
    using AtomicUnsignedIntegerFast8 = std::atomic_uint_fast8_t;
    using AtomicIntegerFast16 = std::atomic_int_fast16_t;
    using AtomicUnsignedIntegerFast16 = std::atomic_uint_fast16_t;
    using AtomicIntegerFast32 = std::atomic_int_fast32_t;
    using AtomicUnsignedIntegerFast32 = std::atomic_uint_fast32_t;
    using AtomicIntegerFast64 = std::atomic_int_fast64_t;
    using AtomicUnsignedIntegerFast64 = std::atomic_uint_fast64_t;
    
    // Other atomic integer types

    using AtomicIntegerPointer = std::atomic_intptr_t;
    using AtomicUnsignedIntegerPointer = std::atomic_uintptr_t;
    using AtomicSize = std::atomic_size_t;
    using AtomicPointerDifference = std::atomic_ptrdiff_t;
    using AtomicIntegerMaximum = std::atomic_intmax_t;
    using AtomicUnsignedIntegerMaximum = std::atomic_uintmax_t;
    
    // C++20 lock-free type aliases
    using AtomicSignedLockFree = std::atomic_signed_lock_free;
    using AtomicUnsignedLockFree = std::atomic_unsigned_lock_free;
    
    // Atomic flag functions
    using std::atomic_flag_test_and_set;
    using std::atomic_flag_test_and_set_explicit;
    using std::atomic_flag_clear;
    using std::atomic_flag_clear_explicit;
    using std::atomic_flag_test;
    using std::atomic_flag_test_explicit;

    using std::atomic_flag_wait;
    using std::atomic_flag_wait_explicit;
    using std::atomic_flag_notify_one;
    using std::atomic_flag_notify_all;
    using std::atomic_wait;
    using std::atomic_wait_explicit;
    using std::atomic_notify_one;
    using std::atomic_notify_all;
    
    // Atomic operations
    using std::atomic_is_lock_free;
    using std::atomic_init;
    using std::atomic_store;
    using std::atomic_store_explicit;
    using std::atomic_load;
    using std::atomic_load_explicit;
    using std::atomic_exchange;
    using std::atomic_exchange_explicit;
    using std::atomic_compare_exchange_weak;
    using std::atomic_compare_exchange_weak_explicit;
    using std::atomic_compare_exchange_strong;
    using std::atomic_compare_exchange_strong_explicit;
    
    // Atomic arithmetic operations
    using std::atomic_fetch_add;
    using std::atomic_fetch_add_explicit;
    using std::atomic_fetch_sub;
    using std::atomic_fetch_sub_explicit;
    using std::atomic_fetch_and;
    using std::atomic_fetch_and_explicit;
    using std::atomic_fetch_or;
    using std::atomic_fetch_or_explicit;
    using std::atomic_fetch_xor;
    using std::atomic_fetch_xor_explicit;
    
    // Fence functions
    using std::atomic_thread_fence;
    using std::atomic_signal_fence;
    
    // Hash function
    using std::hash;
    using std::swap;
}
