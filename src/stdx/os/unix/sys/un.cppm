/**
 * @file un.cppm
 * @module stdx:os.unix.sys.un
 * @brief Module file for Unix domain socket operations.
 *
 * This file contains the implementation of the Unix domain socket operations from the POSIX libraries,
 * located in <sys/un.h>.
 */

module;

#ifdef __unix__
#include <sys/un.h>
#endif

export module stdx:os.unix.sys.un;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using UnixSocketAddress = ::sockaddr_un;
    #endif
}
