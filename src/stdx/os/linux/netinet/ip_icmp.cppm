/**
 * @file ip_icmp.cppm
 * @module stdx:os.linux.netinet.ip_icmp
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <netinet/ip_icmp.h>.
 */

module;

#ifdef __linux__
#include <netinet/ip_icmp.h>
#endif

export module stdx:os.linux.netinet.ip_icmp;

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
