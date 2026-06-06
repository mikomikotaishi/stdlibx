#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #if defined(__unix__) && __has_include(<time.h>)
    using ClockId = ::clockid_t;
    using Timex = ::timex;

    using ::clock_getres;
    using ::clock_gettime;
    using ::clock_settime;
    using ::clock_adjtime;
    #endif
}
