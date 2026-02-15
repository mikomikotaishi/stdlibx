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

export module stdx:os.linux.net.if_ppp;

/**
 * @namespace stdx::os::linux::net
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::net {
    #ifdef __linux__
    using NetworkProtocolIOControl = ::npioctl;
    using PppOptionData = ::ppp_option_data;
    using IfPppStatsReq = ::ifpppstatsreq;
    using IfPppCStatsReq = ::ifpppcstatsreq;
    #endif
}
