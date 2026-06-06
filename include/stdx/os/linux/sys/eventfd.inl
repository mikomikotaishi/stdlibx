#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/eventfd.h>)
    using ::eventfd;
    using ::eventfd_read;
    using ::eventfd_write;
    #endif
}
