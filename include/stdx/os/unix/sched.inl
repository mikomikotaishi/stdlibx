#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using ::sched_setparam;
    using ::sched_getparam;
    using ::sched_setscheduler;
    using ::sched_getscheduler;
    using ::sched_yield;
    using ::sched_get_priority_max;
    using ::sched_get_priority_min;
    using ::sched_rr_get_interval;
    using ::sched_setaffinity;
    using ::sched_getaffinity;
    #endif
}
