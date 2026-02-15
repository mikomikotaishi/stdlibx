/**
 * @file perm.cppm
 * @module stdx:os.linux.sys.perm
 * @brief Module file for Unix I/O privilege permission modifications.
 *
 * This file contains the implementation of the Unix operations on I/O privilege permissions,
 * located in <sys/perm.h>.
 */

module;

#ifdef __linux__
#include <sys/perm.h>
#endif

export module stdx:os.linux.sys.perm;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::ioperm;
    using ::iopl;
    #endif
}
