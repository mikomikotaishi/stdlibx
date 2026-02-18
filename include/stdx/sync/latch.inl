#pragma once

/**
 * @namespace stdx::sync
 * @brief Wrapper namespace for standard library synchronisation operations.
 */
export namespace stdx::sync {
    #if __has_include(<latch>)
    using Latch = std::latch;
    #endif
}
