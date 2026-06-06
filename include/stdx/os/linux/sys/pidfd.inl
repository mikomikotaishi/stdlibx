#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/pidfd.h>)
    using ::pidfd_open;
    using ::pidfd_getfd;
    using ::pidfd_send_signal;
    using ::pidfd_getpid;
    #endif
}
