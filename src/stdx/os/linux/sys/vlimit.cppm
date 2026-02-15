/**
 * @file vlimit.cppm
 * @module stdx:os.linux.sys.vlimit
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/vlimit.h>.
 */

module;

#ifdef __linux__
#include <sys/vlimit.h>
#endif

export module stdx:os.linux.sys.vlimit;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::vlimit;
    #endif
}
