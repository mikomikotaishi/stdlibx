/**
 * @file uio.cppm
 * @module stdx:os.unix.sys.uio
 * @brief Module file for Unix vectored I/O operations.
 *
 * This file contains the implementation of the Unix vectored I/O operations from the POSIX libraries,
 * located in <sys/uio.h>.
 */

module;

#ifdef __unix__
#include <sys/uio.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sys.uio;
#else
export module stdlibx:os.unix.sys.uio;
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
    using ::readv;
    using ::writev;
    using ::preadv;
    using ::pwritev;
    using ::preadv64;
    using ::pwritev64;
    using ::preadv2;
    using ::pwritev2;
    using ::preadv64v2;
    using ::pwritev64v2;
    #endif
}
