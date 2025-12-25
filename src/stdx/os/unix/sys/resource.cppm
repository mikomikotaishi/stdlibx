/**
 * @file x86.cppm
 * @module stdx:os.unix.sys.resource
 * @brief Module file for Unix resource usage operations.
 *
 * This file contains the implementation of the Unix resource usage operations from the POSIX libraries,
 * located in <sys/resource.h>.
 */

module;

#ifdef __unix__
#include <sys/resource.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sys.resource;
#else
export module stdlibx:os.unix.sys.resource;
#endif

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
    using ::getrlimit;
    using ::getrlimit64;
    using ::setrlimit;
    using ::setrlimit64;
    using ::getrusage;
    using ::getpriority;
    using ::setpriority;
    #endif
}
