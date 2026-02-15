/**
 * @file latch.cppm
 * @module stdx:sync.latch
 * @brief Module file for standard library latch operations.
 *
 * This file contains the implementation of the latch operations in the standard library.
 */

module;

#if __has_include(<latch>)
#include <latch>
#endif

export module stdx:sync.latch;

/**
 * @namespace stdx::sync
 * @brief Wrapper namespace for standard library synchronisation operations.
 */
export namespace stdx::sync {
    #if __has_include(<latch>)
    using Latch = std::latch;
    #endif
}
