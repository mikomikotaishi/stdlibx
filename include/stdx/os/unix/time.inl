#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #if (defined(__unix__) || defined(__APPLE__)) && __has_include(<time.h>)
    using ClockId = ::clockid_t;

    /**
     * @brief A duration or deadline in nanoseconds.
     *
     * Spelled out here because it is the only shape kqueue accepts for a wait
     * deadline, and stdx::net::Poller needs one on Darwin.
     */
    using TimeSpec = ::timespec;

    using ::clock_getres;
    using ::clock_gettime;
    using ::clock_settime;

    /**
     * @brief Linux-only clock adjustment.
     *
     * adjtimex and its struct are a Linux extension; Darwin offers neither, and
     * re-exporting them unconditionally made this header Linux-only in a file
     * whose whole point is POSIX.
     */
    #ifdef __linux__
    using Timex = ::timex;

    using ::clock_adjtime;
    #endif
    #endif
}
