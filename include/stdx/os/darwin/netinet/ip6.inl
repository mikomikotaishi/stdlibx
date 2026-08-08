#pragma once

/**
 * @namespace stdx::os::darwin::netinet
 * @brief The IPv6 header - <netinet/ip6.h>.
 */
export namespace stdx::os::darwin::netinet {
    #if defined(__APPLE__) && __has_include(<netinet/ip6.h>)
    using Ip6Header = ::ip6_hdr;
    using Ip6ExtensionHeader = ::ip6_ext;
    using Ip6HopByHopHeader = ::ip6_hbh;
    using Ip6DestinationHeader = ::ip6_dest;
    using Ip6RoutingHeader = ::ip6_rthdr;
    using Ip6FragmentHeader = ::ip6_frag;
    #endif
}
