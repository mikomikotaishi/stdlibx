#pragma once

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
