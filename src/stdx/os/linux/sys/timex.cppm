/**
 * @file timex.cppm
 * @module stdx:os.linux.sys.timex
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/timex.h>.
 */

module;

#ifdef __linux__
#include <sys/timex.h>
#endif

export module stdx:os.linux.sys.timex;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using NtpTimeValue = ::ntptimeval;

    using ::adjtimex;
    using ::ntp_gettimex;
    using ::ntp_adjtime;
    #endif
}
