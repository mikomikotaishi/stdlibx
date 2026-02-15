/**
 * @file ip6.cppm
 * @module stdx:os.linux.netinet.ip6
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <netinet/ip6.h>.
 */

module;

#ifdef __linux__
#include <netinet/ip6.h>
#endif

export module stdx:os.linux.netinet.ip6;

/**
 * @namespace stdx::os::linux::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::netinet {
    #ifdef __linux__
    using Ip6Header = ::ip6_hdr;
    using Ip6Extension = ::ip6_ext;
    using Ip6HopByHop = ::ip6_hbh;
    using Ip6Destination = ::ip6_dest;
    using Ip6RoutingHeader = ::ip6_rthdr;
    using Ip6Type0RoutingHeader = ::ip6_rthdr0;
    using Ip6Fragment = ::ip6_frag;
    using Ip6Options = ::ip6_opt;
    using Ip6OptionJumbo = ::ip6_opt_jumbo;
    using Ip6OptionNsap = ::ip6_opt_nsap;
    using Ip6OptionTunnel = ::ip6_opt_tunnel;
    using Ip6OptionRouter = ::ip6_opt_router;
    #endif
}
