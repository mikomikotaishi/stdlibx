/**
 * @file timeb.cppm
 * @module stdx:os.linux.sys.timeb
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/timeb.h>.
 */

module;

#ifdef __linux__
#include <sys/timeb.h>
#endif

export module stdx:os.linux.sys.timeb;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::ftime;
    #endif
}
