/**
 * @file shared_mutex.cppm
 * @module stdx:sync.shared_mutex
 * @brief Module file for standard library shared mutex operations.
 *
 * This file contains the implementation of the shared mutex operations in the standard library.
 */

module;

#include <shared_mutex>

export module stdx:sync.shared_mutex;

/**
 * @namespace stdx::sync
 * @brief Wrapper namespace for standard library synchronisation operations.
 */
export namespace stdx::sync {
    using SharedMutex = std::shared_mutex;
    using SharedTimedMutex = std::shared_timed_mutex;

    template <typename T>
    using SharedLock = std::shared_lock<T>;

    using std::swap;
}
