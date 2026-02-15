/**
 * @file timerfd.cppm
 * @module stdx:os.linux.sys.timerfd
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/timerfd.h>.
 */

module;

#ifdef __linux__
#include <sys/timerfd.h>
#endif

export module stdx:os.linux.sys.timerfd;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::timerfd_create;
    using ::timerfd_settime;
    using ::timerfd_gettime;
    #endif
}
