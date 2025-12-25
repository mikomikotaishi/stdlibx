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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.eventfd;
#else
export module stdlibx:os.linux.sys.eventfd;
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
    using ::eventfd;
    using ::eventfd_read;
    using ::eventfd_write;
    #endif
}
