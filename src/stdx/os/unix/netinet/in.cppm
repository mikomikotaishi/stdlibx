/**
 * @file in.cppm
 * @module stdx:os.unix.netinet.in
 * @brief Module file for Unix Internet protocol and address operations.
 *
 * This file contains the implementation of the Unix Internet protocol and address operations from the POSIX libraries,
 * located in <netinnet/in.h>.
 */

module;

#ifdef __unix__
#include <netinet/in.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.netinet.in;
#else
export module stdlibx:os.unix.netinet.in;
#endif

/**
 * @namespace stdx::os::unix::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix::netinet {
#else 
export namespace stdlibx::os::unix::netinet {
#endif
    #ifdef __unix__
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using InAddr = ::in_addr;
    using In6Addr = ::in6_addr;
    #endif

    using InternetAddress = ::in_addr;
    using Internet6Address = ::in6_addr;

    using ::in6addr_any;
    using ::in6addr_loopback;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using IpMReq = ::ip_mreq;
    using IpMReqN = ::ip_mreqn;
    using IpMReqSource = ::ip_mreq_source;
    using IpV6MReq = ::ipv6_mreq;
    using GroupReq = ::group_req;
    using GroupSourceReq = ::group_source_req;
    #endif

    using IpMulticastRequest = ::ip_mreq;
    using IpMulticastRequestN = ::ip_mreqn;
    using IpMulticastRequestSource = ::ip_mreq_source;
    using IpV6MulticastRequets = ::ipv6_mreq;
    using GroupRequest = ::group_req;
    using GroupSourceRequest = ::group_source_req;
    using IpMsFilter = ::ip_msfilter;
    using GroupFilter = ::group_filter;

    using ::bindresvport;
    using ::bindresvport6;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using CMsgHdr = ::cmsghdr;

    using In6PktInfo = ::in6_pktinfo;
    using Ip6MtuInfo = ::ip6_mtuinfo;
    #endif

    using ControlMessageHeader = ::cmsghdr;

    using Internet6PacketInfo = ::in6_pktinfo;
    using IpV6MtuInfo = ::ip6_mtuinfo;

    using ::inet6_option_space;
    using ::inet6_option_init;
    using ::inet6_option_append;
    using ::inet6_option_alloc;
    using ::inet6_option_next;
    using ::inet6_option_find;

    using ::inet6_opt_init;
    using ::inet6_opt_append;
    using ::inet6_opt_finish;
    using ::inet6_opt_set_val;
    using ::inet6_opt_next;
    using ::inet6_opt_find;
    using ::inet6_opt_get_val;

    using ::inet6_rth_space;
    using ::inet6_rth_init;
    using ::inet6_rth_add;
    using ::inet6_rth_reverse;
    using ::inet6_rth_segments;
    using ::inet6_rth_getaddr;

    using ::getipv4sourcefilter;
    using ::setipv4sourcefilter;
    using ::getsourcefilter;
    using ::setsourcefilter;
    #endif
}
