#pragma once

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
