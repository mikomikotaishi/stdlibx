/**
 * @file if_ppp.cppm
 * @module stdx:os.linux.net.if_ppp
 * @brief Module file for Unix local network interfacing operations.
 *
 * This file contains the implementation of the Unix local networking interfacing operations from the POSIX libraries,
 * located in <net/if_ppp.h>.
 */

module;

#ifdef __linux__
#include <net/if_ppp.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.net.if_ppp;
#else
export module stdlibx:os.linux.net.if_ppp;
#endif

/**
 * @namespace stdx::os::linux::net
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::net {
#else 
export namespace stdlibx::os::linux::net {
#endif
    #ifdef __linux__
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using NpIOCtl = ::npioctl;
    using PppOptionData = ::ppp_option_data;
    using IfPppStatsReq = ::ifpppstatsreq;
    using IfPppCStatsReq = ::ifpppcstatsreq;
    #endif

    using NetworkProtocolIOControl = ::npioctl;
    using PppOptionData = ::ppp_option_data;
    using IfPppStatsReq = ::ifpppstatsreq;
    using IfPppCStatsReq = ::ifpppcstatsreq;
    #endif
}
