#pragma once

/**
 * @namespace stdx::os::linux::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::netinet {
    #ifdef __linux__
    using IcmpHeader = ::icmphdr;
    using IcmpRouterAdvertisementHeader = ::icmp_ra_addr;
    using Icmp = ::icmp;
    #endif
}
