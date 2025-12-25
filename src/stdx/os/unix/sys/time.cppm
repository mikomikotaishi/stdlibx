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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sys.time;
#else
export module stdlibx:os.unix.sys.time;
#endif

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix::sys {
#else 
export namespace stdlibx::os::unix::sys {
#endif
    #ifdef __unix__
    using TimeZone = ::timezone;

    using ::gettimeofday;
    using ::settimeofday;
    using ::adjtime;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using ITimerVal = ::itimerval;
    #endif

    using IntervalTimerValue = ::itimerval;

    using ::getitimer;
    using ::setitimer;
    using ::utimes;
    using ::lutimes;
    using ::futimes;
    using ::futimesat;
    #endif
}
