#pragma once

/**
 * @internal
 * @namespace stdx::os::unix::netinet::captured
 * @brief The numeric values of the <netinet/in.h> macros, read where they exist.
 *
 * The IPPROTO_* numbers are IANA's and agree everywhere. The IPV6_* option
 * numbers do not: IPV6_V6ONLY is 26 on Linux and 27 on Darwin, and the multicast
 * and hop-limit options differ too. IPV6_V6ONLY is the one that matters here,
 * since TcpListener uses it to serve both families from one socket.
 *
 * Note that Darwin declares the IPV6_* options in <netinet6/in6.h>, which
 * <netinet/in.h> includes, so reading them here needs no extra include.
 */
namespace stdx::os::unix::netinet::captured {
    #if (defined(__unix__) || defined(__APPLE__)) && __has_include(<netinet/in.h>)
    constexpr auto IPPROTO_IP_VALUE = IPPROTO_IP;
    constexpr auto IPPROTO_ICMP_VALUE = IPPROTO_ICMP;
    constexpr auto IPPROTO_TCP_VALUE = IPPROTO_TCP;
    constexpr auto IPPROTO_UDP_VALUE = IPPROTO_UDP;
    constexpr auto IPPROTO_IPV6_VALUE = IPPROTO_IPV6;
    constexpr auto INADDR_ANY_VALUE = INADDR_ANY;
    constexpr auto INADDR_LOOPBACK_VALUE = INADDR_LOOPBACK;
    constexpr auto INADDR_BROADCAST_VALUE = INADDR_BROADCAST;
    constexpr auto IPV6_V6ONLY_VALUE = IPV6_V6ONLY;
    constexpr auto IPV6_JOIN_GROUP_VALUE = IPV6_JOIN_GROUP;
    constexpr auto IPV6_LEAVE_GROUP_VALUE = IPV6_LEAVE_GROUP;
    constexpr auto IPV6_MULTICAST_HOPS_VALUE = IPV6_MULTICAST_HOPS;
    constexpr auto IPV6_MULTICAST_IF_VALUE = IPV6_MULTICAST_IF;
    constexpr auto IPV6_UNICAST_HOPS_VALUE = IPV6_UNICAST_HOPS;
    #endif
}

/**
 * @namespace stdx::os::unix::netinet
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix::netinet {
    #if (defined(__unix__) || defined(__APPLE__)) && __has_include(<netinet/in.h>)
    using InternetAddress = ::in_addr;
    using Internet6Address = ::in6_addr;

    using InternetSocketAddress = ::sockaddr_in;
    using Internet6SocketAddress = ::sockaddr_in6;

    using ::in6addr_any;
    using ::in6addr_loopback;

    using IpMulticastRequest = ::ip_mreq;
    using IpMulticastRequestN = ::ip_mreqn;
    using IpMulticastRequestSource = ::ip_mreq_source;
    using IpV6MulticastRequets = ::ipv6_mreq;
    using GroupRequest = ::group_req;
    using GroupSourceRequest = ::group_source_req;

    using ::bindresvport;

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

    /**
     * @brief Not found in the macOS SDK headers to hand.
     *
     * ip_msfilter and group_filter are RFC 3678 multicast source-filter types and
     * bindresvport6 is glibc's IPv6 counterpart to bindresvport. None appears in
     * the Darwin <netinet/in.h> or <netinet6/in6.h> checked here, so they are
     * guarded rather than assumed. If a Mac build shows they exist elsewhere in
     * the SDK, widen this - the guard is a statement about what was verified, not
     * a claim that Darwin lacks them.
     */
    #ifdef __linux__
    using IpMsFilter = ::ip_msfilter;
    using GroupFilter = ::group_filter;

    using ::bindresvport6;
    #endif
    #endif
}
