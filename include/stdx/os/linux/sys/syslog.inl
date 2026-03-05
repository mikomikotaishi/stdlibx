#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::closelog;
    using ::openlog;
    using ::setlogmask;
    using ::syslog;
    using ::vsyslog;
    #endif
}
