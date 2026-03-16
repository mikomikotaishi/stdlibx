#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    [[nodiscard]]
    inline i32& errnov() noexcept {
        return errno;
    }
}
