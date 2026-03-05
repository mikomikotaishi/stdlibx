#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::pidfd_open;
    using ::pidfd_getfd;
    using ::pidfd_send_signal;
    using ::pidfd_getpid;
    #endif
}
