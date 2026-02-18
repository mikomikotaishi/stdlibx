#pragma once

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
