/**
 * @file sched.cppm
 * @module stdx:os.unix.sched
 * @brief Module file for Unix execution scheduling operations.
 *
 * This file contains the implementation of the Unix executing scheduling operations from the POSIX libraries,
 * located in <sched.h>.
 */

module;

#ifdef __unix__
#include <sched.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sched;
#else
export module stdlibx:os.unix.sched;
#endif

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix {
#else 
export namespace stdlibx::os::unix {
#endif
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
