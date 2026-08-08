#pragma once

/**
 * @namespace stdx::os::unix::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #if (defined(__unix__) || defined(__APPLE__)) && __has_include(<sys/time.h>)
    using TimeZone = struct timezone;

    using TimeValue = ::timeval;

    using ::gettimeofday;
    using ::settimeofday;
    using ::adjtime;

    using IntervalTimerValue = ::itimerval;

    using ::getitimer;
    using ::setitimer;
    using ::utimes;
    using ::lutimes;
    using ::futimes;

    // Darwin has no futimesat.
    #ifndef __APPLE__
    using ::futimesat;
    #endif
    #endif
}
