/**
 * @file route.cppm
 * @module stdx:os.linux.net.route
 * @brief Module file for Unix local network interfacing operations.
 *
 * This file contains the implementation of the Unix local networking interfacing operations from the POSIX libraries,
 * located in <net/route.h>.
 */

module;

#ifdef __linux__
#include <net/route.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.net.route;
#else
export module stdlibx:os.linux.net.route;
#endif

/**
 * @namespace stdx::os::linux::net
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::net {
#else 
export namespace stdlibx::os::linux::net {
#endif
    #ifdef __linux__
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using RtEntry = ::rtentry;
    using In6RtMsg = ::in6_rtmsg;
    #endif

    using RouteEntry = ::rtentry;
    using Internet6RouteMessage = ::in6_rtmsg;
    #endif
}
