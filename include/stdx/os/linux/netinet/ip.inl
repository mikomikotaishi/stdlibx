#pragma once

/**
 * @namespace stdx::os::linux::netinet
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::netinet {
    #if defined(__linux__) && __has_include(<netinet/ip.h>)
    using TimeStamp = ::timestamp;
    using IpHeader = ::iphdr;
    using Ip = ::ip;
    using IpTimeStamp = ::ip_timestamp;
    #endif
}
