#pragma once

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
