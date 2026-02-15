/**
 * @file ptrace.cppm
 * @module stdx:os.linux.sys.ptrace
 * @brief Module file for Unix ptrace debugger interface operations.
 *
 * This file contains the implementation of the Unix ptrace debugger interface operations from the POSIX libraries,
 * located in <sys/ptrace.h>.
 */

module;

#ifdef __linux__
#include <sys/ptrace.h>
#endif

export module stdx:os.linux.sys.ptrace;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::ptrace;
    #endif
}
