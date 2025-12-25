/**
 * @file shared_mutex.cppm
 * @module std:sync.shared_mutex
 * @brief Module file for standard library shared mutex operations.
 *
 * This file contains the implementation of the shared mutex operations in the standard library.
 */

module;

#include <shared_mutex>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:sync.shared_mutex;
#else
export module stdlib:sync.shared_mutex;
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
    using SharedMutex = std::shared_mutex;
    using SharedTimedMutex = std::shared_timed_mutex;

    template <typename T>
    using SharedLock = std::shared_lock<T>;

    using std::swap;
}
