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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sys.ioctl;
#else
export module stdlibx:os.unix.sys.ioctl;
#endif

export import :os.unix.sys.ioctl.constants;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix::sys {
#else 
export namespace stdlibx::os::unix::sys {
#endif
    #ifdef __unix__
    using ::ioctl;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using WinSize = ::winsize;
    // using TermIO = ::termio;
    #endif

    using WindowSize = ::winsize;
    // using TerminalIO = ::termio;
    #endif
}
