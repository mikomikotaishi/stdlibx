/**
 * @file time.cppm
 * @module stdx:os.unix.sys.time
 * @brief Module file for Unix time and date operations.
 *
 * This file contains the implementation of the Unix time and date operations from the POSIX libraries,
 * located in <sys/time.h>.
 */

module;

#ifdef __unix__
#include <sys/time.h>
#endif

export module stdx:os.unix.sys.time;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using TimeZone = ::timezone;

    using ::gettimeofday;
    using ::settimeofday;
    using ::adjtime;

    using IntervalTimerValue = ::itimerval;

    using ::getitimer;
    using ::setitimer;
    using ::utimes;
    using ::lutimes;
    using ::futimes;
    using ::futimesat;
    #endif
}
