/**
 * @file semaphore.cppm
 * @module std:sync.semaphore
 * @brief Module file for standard library semaphore operations.
 *
 * This file contains the implementation of the semaphore operations in the standard library.
 */

module;

#if __has_include(<semaphore>)
#include <semaphore>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:sync.semaphore;
#else
export module stdlib:sync.semaphore;
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
    #if __has_include(<semaphore>)
    using CountingSemaphore = std::counting_semaphore<>;

    using BinarySemaphore = std::binary_semaphore;
    #endif
}
