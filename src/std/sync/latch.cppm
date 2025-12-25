/**
 * @file latch.cppm
 * @module std:sync.latch
 * @brief Module file for standard library latch operations.
 *
 * This file contains the implementation of the latch operations in the standard library.
 */

module;

#if __has_include(<latch>)
#include <latch>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:sync.latch;
#else
export module stdlib:sync.latch;
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
    #if __has_include(<latch>)
    using Latch = std::latch;
    #endif
}
