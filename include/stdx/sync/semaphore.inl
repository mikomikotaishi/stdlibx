#pragma once

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
