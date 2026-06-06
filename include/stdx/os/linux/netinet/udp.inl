#pragma once

/**
 * @namespace stdx::os::linux::netinet
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::posix::netinet {
    #if defined(__linux__) && __has_include(<netinet/udp.h>)
    using UdpHeader = ::udphdr;
    #endif
}
