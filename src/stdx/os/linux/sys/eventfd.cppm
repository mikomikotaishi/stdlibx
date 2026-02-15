/**
 * @file eventfd.cppm
 * @module stdx:os.linux.sys.eventfd
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/eventfd.h>.
 */

module;

#ifdef __linux__
#include <sys/eventfd.h>
#endif

export module stdx:os.linux.sys.eventfd;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::eventfd;
    using ::eventfd_read;
    using ::eventfd_write;
    #endif
}
