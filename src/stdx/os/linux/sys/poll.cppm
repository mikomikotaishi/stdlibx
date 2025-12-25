/**
 * @file poll.cppm
 * @module stdx:os.linux.sys.poll
 * @brief Module file for Unix System V poll interface compatibility operations.
 *
 * This file contains the implementation of the Unix System V poll interface compatibility operations from the POSIX libraries,
 * located in <sys/poll.h>.
 */

module;

#ifdef __linux__
#include <sys/poll.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.poll;
#else
export module stdlibx:os.linux.sys.poll;
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
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using PollFd = ::pollfd;
    #endif

    using PollFileDescriptor = ::pollfd;

    using ::poll;
    using ::ppoll;
    #endif
}
