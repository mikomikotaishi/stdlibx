/**
 * @file single_threaded.cppm
 * @module stdx:os.linux.sys.single_threaded
 * @brief Module file for Unix single-threaded optimisation support.
 *
 * This file contains the implementation of the Unix single-threaded optimisation support from the POSIX libraries,
 * located in <sys/single_threaded.h>.
 */

module;

#ifdef __linux__
#include <sys/single_threaded.h>
#endif

export module stdx:os.linux.sys.single_threaded;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::__libc_single_threaded;
    #endif
}
