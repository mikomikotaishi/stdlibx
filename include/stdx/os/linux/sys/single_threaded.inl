#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/single_threaded.h>)
    using ::__libc_single_threaded;
    #endif
}
