/**
 * @file atomic.cppm
 * @module std:sync.atomic
 * @brief Module file for standard library atomic operations.
 *
 * This file contains the implementation of the atomic operations in the standard library.
 */

module;

#include <atomic>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:sync.atomic;
#else
export module stdlib:sync.atomic;
#endif

/**
 * @namespace std::sync
 * @brief Wrapper namespace for standard library synchronisation operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::sync {
#else 
export namespace stdlib::sync {
#endif
    // Atomic template class
    template <typename T>
    using Atomic = std::atomic<T>;
    
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    // Atomic reference template class
    template <typename T>
    using AtomicRef = std::atomic_ref<T>;
    #endif

    // Atomic reference template class
    template <typename T>
    using AtomicReference = std::atomic_ref<T>;
    
    // Memory order enum
    using MemoryOrder = std::memory_order;
    
    // Memory order constants
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    inline constexpr MemoryOrder MEMORY_ORDER_ACQ_REL = std::memory_order_acq_rel;
    inline constexpr MemoryOrder MEMORY_ORDER_SEQ_CST = std::memory_order_seq_cst;
    #endif

    inline constexpr MemoryOrder MEMORY_ORDER_RELAXED = std::memory_order_relaxed;
    inline constexpr MemoryOrder MEMORY_ORDER_CONSUME = std::memory_order_consume;
    inline constexpr MemoryOrder MEMORY_ORDER_ACQUIRE = std::memory_order_acquire;
    inline constexpr MemoryOrder MEMORY_ORDER_RELEASE = std::memory_order_release;
    inline constexpr MemoryOrder MEMORY_ORDER_ACQUIRE_RELEASE = std::memory_order_acq_rel;
    inline constexpr MemoryOrder MEMORY_ORDER_SEQUENTIALLY_CONSISTENT = std::memory_order_seq_cst;
    
    // Atomic flag
    using AtomicFlag = std::atomic_flag;
    
    // Basic atomic typedefs
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using AtomicBool = std::atomic_bool;
    using AtomicChar = std::atomic_char;
    using AtomicSChar = std::atomic_schar;
    using AtomicUChar = std::atomic_uchar;
    using AtomicShort = std::atomic_short;
    using AtomicUShort = std::atomic_ushort;
    using AtomicInt = std::atomic_int;
    using AtomicUInt = std::atomic_uint;
    using AtomicLong = std::atomic_long;
    using AtomicULong = std::atomic_ulong;
    using AtomicLLong = std::atomic_llong;
    using AtomicULLong = std::atomic_ullong;
    using AtomicChar8_t = std::atomic_char8_t;
    using AtomicChar16_t = std::atomic_char16_t;
    using AtomicChar32_t = std::atomic_char32_t;
    using AtomicWChar_t = std::atomic_wchar_t;

    using AtomicCharacter8_t = std::atomic_char8_t;
    using AtomicCharacter16_t = std::atomic_char16_t;
    using AtomicCharacter32_t = std::atomic_char32_t;
    using AtomicWideCharacter_t = std::atomic_wchar_t;
    #endif

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
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using AtomicInt8_t = std::atomic_int8_t;
    using AtomicUInt8_t = std::atomic_uint8_t;
    using AtomicInt16_t = std::atomic_int16_t;
    using AtomicUInt16_t = std::atomic_uint16_t;
    using AtomicInt32_t = std::atomic_int32_t;
    using AtomicUInt32_t = std::atomic_uint32_t;
    using AtomicInt64_t = std::atomic_int64_t;
    using AtomicUInt64_t = std::atomic_uint64_t;

    using AtomicInteger8_t = std::atomic_int8_t;
    using AtomicUnsignedInteger8_t = std::atomic_uint8_t;
    using AtomicInteger16_t = std::atomic_int16_t;
    using AtomicUnsignedInteger16_t = std::atomic_uint16_t;
    using AtomicInteger32_t = std::atomic_int32_t;
    using AtomicUnsignedInteger32_t = std::atomic_uint32_t;
    using AtomicInteger64_t = std::atomic_int64_t;
    using AtomicUnsignedInteger64_t = std::atomic_uint64_t;
    #endif

    using AtomicInteger8 = std::atomic_int8_t;
    using AtomicUnsignedInteger8 = std::atomic_uint8_t;
    using AtomicInteger16 = std::atomic_int16_t;
    using AtomicUnsignedInteger16 = std::atomic_uint16_t;
    using AtomicInteger32 = std::atomic_int32_t;
    using AtomicUnsignedInteger32 = std::atomic_uint32_t;
    using AtomicInteger64 = std::atomic_int64_t;
    using AtomicUnsignedInteger64 = std::atomic_uint64_t;
    
    // Least/fast atomic integer types
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using AtomicIntLeast8_t = std::atomic_int_least8_t;
    using AtomicUIntLeast8_t = std::atomic_uint_least8_t;
    using AtomicIntLeast16_t = std::atomic_int_least16_t;
    using AtomicUIntLeast16_t = std::atomic_uint_least16_t;
    using AtomicIntLeast32_t = std::atomic_int_least32_t;
    using AtomicUIntLeast32_t = std::atomic_uint_least32_t;
    using AtomicIntLeast64_t = std::atomic_int_least64_t;
    using AtomicUIntLeast64_t = std::atomic_uint_least64_t;
    
    using AtomicIntFast8_t = std::atomic_int_fast8_t;
    using AtomicUIntFast8_t = std::atomic_uint_fast8_t;
    using AtomicIntFast16_t = std::atomic_int_fast16_t;
    using AtomicUIntFast16_t = std::atomic_uint_fast16_t;
    using AtomicIntFast32_t = std::atomic_int_fast32_t;
    using AtomicUIntFast32_t = std::atomic_uint_fast32_t;
    using AtomicIntFast64_t = std::atomic_int_fast64_t;
    using AtomicUIntFast64_t = std::atomic_uint_fast64_t;

    using AtomicIntegerLeast8_t = std::atomic_int_least8_t;
    using AtomicUnsignedIntegerLeast8_t = std::atomic_uint_least8_t;
    using AtomicIntegerLeast16_t = std::atomic_int_least16_t;
    using AtomicUnsignedIntegerLeast16_t = std::atomic_uint_least16_t;
    using AtomicIntegerLeast32_t = std::atomic_int_least32_t;
    using AtomicUnsignedIntegerLeast32_t = std::atomic_uint_least32_t;
    using AtomicIntegerLeast64_t = std::atomic_int_least64_t;
    using AtomicUnsignedIntegerLeast64_t = std::atomic_uint_least64_t;
    
    using AtomicIntegerFast8_t = std::atomic_int_fast8_t;
    using AtomicUnsignedIntegerFast8_t = std::atomic_uint_fast8_t;
    using AtomicIntegerFast16_t = std::atomic_int_fast16_t;
    using AtomicUnsignedIntegerFast16_t = std::atomic_uint_fast16_t;
    using AtomicIntegerFast32_t = std::atomic_int_fast32_t;
    using AtomicUnsignedIntegerFast32_t = std::atomic_uint_fast32_t;
    using AtomicIntegerFast64_t = std::atomic_int_fast64_t;
    using AtomicUnsignedIntegerFast64_t = std::atomic_uint_fast64_t;
    #endif

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
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using AtomicIntPtr_t = std::atomic_intptr_t;
    using AtomicUIntPtr_t = std::atomic_uintptr_t;
    using AtomicSize_t = std::atomic_size_t;
    using AtomicPtrDiff_t = std::atomic_ptrdiff_t;
    using AtomicIntMax_t = std::atomic_intmax_t;
    using AtomicUIntMax_t = std::atomic_uintmax_t;

    using AtomicIntegerPointer_t = std::atomic_intptr_t;
    using AtomicUnsignedIntegerPointer_t = std::atomic_uintptr_t;
    using AtomicSize_t = std::atomic_size_t;
    using AtomicPointerDifference_t = std::atomic_ptrdiff_t;
    using AtomicIntegerMaximum_t = std::atomic_intmax_t;
    using AtomicUnsignedIntegerMaximum_t = std::atomic_uintmax_t;

    using AtomicUSize_t = std::atomic_size_t;
    using AtomicUSize = std::atomic_size_t;
    #endif

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
