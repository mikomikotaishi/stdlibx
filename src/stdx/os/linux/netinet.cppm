/**
 * @file netinet.cppm
 * @module stdx:os.linux.netinet
 * @brief Module file for Unix network internet operations.
 *
 * This file contains the implementation of the Unix network internet operations from the POSIX libraries,
 * located in all <netinet/*.h> files.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.netinet;
#else
export module stdlibx:os.linux.netinet;
#endif

export import :os.linux.netinet.ether;
export import :os.linux.netinet.icmp6;
export import :os.linux.netinet.if_ether;
export import :os.linux.netinet.if_fddi;
export import :os.linux.netinet.if_tr;
export import :os.linux.netinet.igmp;
export import :os.linux.netinet.ip;
export import :os.linux.netinet.ip_icmp;
export import :os.linux.netinet.ip6;
export import :os.linux.netinet.udp;
