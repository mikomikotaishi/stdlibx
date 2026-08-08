#pragma once

/**
 * @namespace stdx::os::darwin::netinet
 * @brief ICMP for IPv6 - <netinet/icmp6.h>.
 */
export namespace stdx::os::darwin::netinet {
    #if defined(__APPLE__) && __has_include(<netinet/icmp6.h>)
    using Icmp6Header = ::icmp6_hdr;
    using Icmp6Filter = ::icmp6_filter;
    using NeighbourDiscoveryRouterSolicit = ::nd_router_solicit;
    using NeighbourDiscoveryRouterAdvert = ::nd_router_advert;
    using NeighbourDiscoveryNeighbourSolicit = ::nd_neighbor_solicit;
    using NeighbourDiscoveryNeighbourAdvert = ::nd_neighbor_advert;
    #endif
}
