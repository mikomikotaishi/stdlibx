#pragma once

/**
 * @namespace stdx::os::darwin::netinet
 * @brief The IPv4 header - <netinet/ip.h>.
 */
export namespace stdx::os::darwin::netinet {
    #if defined(__APPLE__) && __has_include(<netinet/ip.h>)
    using IpHeader = ::ip;
    using IpOptionTimestamp = ::ip_timestamp;
    #endif
}
