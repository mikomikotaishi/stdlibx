#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/syslog.h>)
    using ::closelog;
    using ::openlog;
    using ::setlogmask;
    using ::syslog;
    using ::vsyslog;
    #endif
}
