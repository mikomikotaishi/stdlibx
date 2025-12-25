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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sys.un;
#else
export module stdlibx:os.unix.sys.un;
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
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using SockAddrUn = ::sockaddr_un;
    #endif

    using UnixSocketAddress = ::sockaddr_un;
    #endif
}
