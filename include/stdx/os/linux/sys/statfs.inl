#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::statfs;
    using ::statfs64;
    using ::fstatfs;
    using ::fstatfs64;
    #endif
}
