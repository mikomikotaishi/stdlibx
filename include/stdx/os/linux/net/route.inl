#pragma once

/**
 * @namespace stdx::os::linux::net
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::net {
    #if defined(__linux__) && __has_include(<net/route.h>)
    using RouteEntry = ::rtentry;
    using Internet6RouteMessage = ::in6_rtmsg;
    #endif
}
