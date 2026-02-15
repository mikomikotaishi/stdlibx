/**
 * @file ipc.cppm
 * @module stdx:os.unix.sys.ipc
 * @brief Module file for Unix inter-process communication operations.
 *
 * This file contains the implementation of the Unix inter-process communication operations from the POSIX libraries,
 * located in <sys/ipc.h>.
 */

module;

#ifdef __unix__
#include <sys/ipc.h>
#endif

export module stdx:os.unix.sys.ipc;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using ::ftok;
    #endif
}
