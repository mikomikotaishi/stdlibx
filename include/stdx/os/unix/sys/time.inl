#pragma once

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using TimeZone = struct timezone;

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
