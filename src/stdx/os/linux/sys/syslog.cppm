/**
 * @file syslog.cppm
 * @module stdx:os.linux.sys.syslog
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/syslog.h>.
 */

module;

#ifdef __linux__
#include <sys/syslog.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.syslog;
#else
export module stdlibx:os.linux.sys.syslog;
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
    using ::closelog;
    using ::openlog;
    using ::setlogmask;
    using ::syslog;
    using ::vsyslog;
    #endif
}
