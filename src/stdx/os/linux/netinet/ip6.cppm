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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.netinet.ip6;
#else
export module stdlibx:os.linux.netinet.ip6;
#endif

/**
 * @namespace stdx::os::linux::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::netinet {
#else 
export namespace stdlibx::os::linux::netinet {
#endif
    #ifdef __linux__
    using IP6Hdr = ::ip6_hdr;
    using IP6Ext = ::ip6_ext;
    using IP6HBH = ::ip6_hbh;
    using IP6Dest = ::ip6_dest;
    using IP6RtHdr = ::ip6_rthdr;
    using IP6RtHdr0 = ::ip6_rthdr0;
    using IP6Frag = ::ip6_frag;
    using IP6Opt = ::ip6_opt;
    using IP6OptJumbo = ::ip6_opt_jumbo;
    using IP6OptNSAP = ::ip6_opt_nsap;
    using IP6OptTunnel = ::ip6_opt_tunnel;
    using IP6OptRouter = ::ip6_opt_router;
    #endif
}
