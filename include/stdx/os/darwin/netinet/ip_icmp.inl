#pragma once

/**
 * @namespace stdx::os::darwin::netinet
 * @brief ICMP for IPv4 - <netinet/ip_icmp.h>.
 */
export namespace stdx::os::darwin::netinet {
    #if defined(__APPLE__) && __has_include(<netinet/ip_icmp.h>)
    using IcmpHeader = ::icmp;
    #endif
}
