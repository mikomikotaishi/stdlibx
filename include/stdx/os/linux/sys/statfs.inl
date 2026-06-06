#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/statfs.h>)
    using ::statfs;
    using ::statfs64;
    using ::fstatfs;
    using ::fstatfs64;
    #endif
}
