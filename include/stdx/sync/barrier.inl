#pragma once

/**
 * @namespace stdx::sync
 * @brief Standard library synchronization operations.
 */
export namespace stdx::sync {
    struct EmptyCompletion {
        void operator()() const noexcept {}
    };

    template <typename CompletionFunction = EmptyCompletion>
    using Barrier = std::barrier<CompletionFunction>;
}
