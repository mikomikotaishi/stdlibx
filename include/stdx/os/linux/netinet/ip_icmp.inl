#pragma once

/**
 * @namespace stdx::os::linux::netinet
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::netinet {
    #if defined(__linux__) && __has_include(<netinet/ip_icmp.h>)
    using IcmpHeader = ::icmphdr;
    using IcmpRouterAdvertisementHeader = ::icmp_ra_addr;
    using Icmp = ::icmp;
    #endif
}
