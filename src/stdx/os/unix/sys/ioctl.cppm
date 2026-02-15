/**
 * @file ioctl.cppm
 * @module stdx:os.unix.sys.ioctl
 * @brief Module file for I/O control operations.
 *
 * This file contains the implementation of the I/O control operations in Unix systems,
 * located in <sys/ioctl.h>.
 */

module;

#ifdef __unix__
#include <sys/ioctl.h>
#endif

export module stdx:os.unix.sys.ioctl;

export import :os.unix.sys.ioctl.constants;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using ::ioctl;

    using WindowSize = ::winsize;
    // using TerminalIO = ::termio;
    #endif
}
