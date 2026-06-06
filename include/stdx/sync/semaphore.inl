#pragma once

/**
 * @namespace stdx::sync
 * @brief Standard library synchronisation operations.
 */
export namespace stdx::sync {
    template <ptrdiff LeastMaxValue = std::counting_semaphore<>::max()>
    using CountingSemaphore = std::counting_semaphore<LeastMaxValue>;
    using BinarySemaphore = std::binary_semaphore;
}
