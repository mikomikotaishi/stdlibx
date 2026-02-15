/**
 * @file psx_syscall.cppm
 * @module stdx:os.linux.sys.psx_syscall
 * @brief Module file for Unix POSIX syscalls for pthread operations.
 *
 * This file contains the implementation of the Unix POSIX syscalls for pthread operations from the POSIX libraries,
 * located in <sys/psx_syscall.h>.
 */

module;

#ifdef __linux__
#include <sys/psx_syscall.h>
#endif

export module stdx:os.linux.sys.psx_syscall;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::psx_load_syscalls;
    using ::psx_set_sensitivity;
    #endif
}
