/**
 * @file semaphore.cppm
 * @module stdx:sync.semaphore
 * @brief Module file for standard library semaphore operations.
 *
 * This file contains the implementation of the semaphore operations in the standard library.
 */

module;

#if __has_include(<semaphore>)
#include <semaphore>
#endif

export module stdx:sync.semaphore;

/**
 * @namespace stdx::sync
 * @brief Wrapper namespace for standard library synchronisation operations.
 */
export namespace stdx::sync {
    #if __has_include(<semaphore>)
    using CountingSemaphore = std::counting_semaphore<>;

    using BinarySemaphore = std::binary_semaphore;
    #endif
}
