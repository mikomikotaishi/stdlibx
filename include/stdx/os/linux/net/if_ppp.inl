#pragma once

/**
 * @namespace stdx::os::linux::net
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::net {
    #if defined(__linux__) && __has_include(<net/if_ppp.h>)
    using NetworkProtocolIOControl = ::npioctl;
    using PppOptionData = ::ppp_option_data;
    using IfPppStatsReq = ::ifpppstatsreq;
    using IfPppCStatsReq = ::ifpppcstatsreq;
    #endif
}
