/**
 * @file barrier.cppm
 * @module std:sync.barrier
 * @brief Module file for standard library barrier operations.
 *
 * This file contains the implementation of the barrier operations in the standard library.
 */

module;

#if __has_include(<barrier>)
#include <barrier>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:sync.barrier;
#else
export module stdlib.sync.barrier;
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
    #if __has_include(<barrier>)
    struct EmptyCompletion {
        void operator()() noexcept {

        }
    };

    template <typename CompletionFunction = EmptyCompletion>
    using Barrier = std::barrier<CompletionFunction>;
    #endif
}
