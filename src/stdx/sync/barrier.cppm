/**
 * @file barrier.cppm
 * @module stdx:sync.barrier
 * @brief Module file for standard library barrier operations.
 *
 * This file contains the implementation of the barrier operations in the standard library.
 */

module;

#if __has_include(<barrier>)
#include <barrier>
#endif

export module stdx:sync.barrier;

/**
 * @namespace stdx::sync
 * @brief Wrapper namespace for standard library synchronisation operations.
 */
export namespace stdx::sync {
    struct EmptyCompletion {
        void operator()() const noexcept {}
    };

    #if __has_include(<barrier>)
    template <typename CompletionFunction = EmptyCompletion>
    using Barrier = std::barrier<CompletionFunction>;
    #endif
}
