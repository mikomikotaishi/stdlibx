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

export module stdx:os.linux.net.route;

/**
 * @namespace stdx::os::linux::net
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::net {
    #ifdef __linux__
    using RouteEntry = ::rtentry;
    using Internet6RouteMessage = ::in6_rtmsg;
    #endif
}
