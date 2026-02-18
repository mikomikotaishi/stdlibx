#pragma once

/**
 * @namespace stdx::sync
 * @brief Wrapper namespace for standard library synchronisation operations.
 */
export namespace stdx::sync {
    using Mutex = std::mutex;
    using TimedMutex = std::timed_mutex;
    using RecursiveMutex = std::recursive_mutex;
    using RecursiveTimedMutex = std::recursive_timed_mutex;

    template <typename Mtx>
    using LockGuard = std::lock_guard<Mtx>;

    template <typename Mtx>
    using UniqueLock = std::unique_lock<Mtx>;

    template <typename... MtxTypes>
    using ScopedLock = std::scoped_lock<MtxTypes...>;

    using OnceFlag = std::once_flag;
    
    using std::try_lock;
    using std::lock;
    using std::call_once;
    using std::swap;

    using DeferLockTag = std::defer_lock_t;
    using TryToLockTag = std::try_to_lock_t;
    using AdoptLockTag = std::adopt_lock_t;
    inline constexpr DeferLockTag DeferLock = std::defer_lock;
    inline constexpr TryToLockTag TryToLock = std::try_to_lock;
    inline constexpr AdoptLockTag AdoptLock = std::adopt_lock;
}
