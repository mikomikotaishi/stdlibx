/**
 * @file syslog.cppm
 * @module stdx:os.linux.sys.syslog
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/syslog.h>.
 */

module;

#ifdef __linux__
#include <sys/syslog.h>
#endif

export module stdx:os.linux.sys.syslog;

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
