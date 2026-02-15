/**
 * @file times.cppm
 * @module stdx:os.unix.sys.times
 * @brief Module file for Unix file access/modification times operations.
 *
 * This file contains the implementation of the Unix file access/modification times operations from the POSIX libraries,
 * located in <sys/times.h>.
 */

module;

#ifdef __unix__
#include <sys/times.h>
#endif

export module stdx:os.unix.sys.times;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using Times = ::tms;

    using ::times;
    #endif
}
