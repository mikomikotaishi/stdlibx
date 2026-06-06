#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix {
    [[nodiscard]]
    inline auto& errnov() noexcept {
        return errno;
    }
}
