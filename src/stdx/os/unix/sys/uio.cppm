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

export module stdx:os.unix.sys.uio;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
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
