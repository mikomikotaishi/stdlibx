/**
 * @file net.cppm
 * @module stdx:os.linux.net
 * @brief Module file for Unix networking operations.
 *
 * This file contains the implementation of the Unix networking operations from the POSIX libraries,
 * located in all <net/*.h> files.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.net;
#else
export module stdlibx:os.linux.net;
#endif

export import :os.linux.net.ethernet;
export import :os.linux.net.if_arp;
export import :os.linux.net.if_packet;
export import :os.linux.net.if_ppp;
export import :os.linux.net.if_shaper;
export import :os.linux.net.ppp_defs;
export import :os.linux.net.route;
