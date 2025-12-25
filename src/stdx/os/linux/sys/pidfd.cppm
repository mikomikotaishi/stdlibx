/**
 * @file pidfd.cppm
 * @module stdx:os.linux.sys.pidfd
 * @brief Module file for Unix process function file descriptor operations.
 *
 * This file contains the implementation of the Unix process function file descriptor operations from the POSIX libraries,
 * located in <sys/pidfd.h>.
 */

module;

#ifdef __linux__
#include <sys/pidfd.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.pidfd;
#else
export module stdlibx:os.linux.sys.pidfd;
#endif

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::sys {
#else 
export namespace stdlibx::os::linux::sys {
#endif
    #ifdef __linux__
    using ::pidfd_open;
    using ::pidfd_getfd;
    using ::pidfd_send_signal;
    using ::pidfd_getpid;
    #endif
}
