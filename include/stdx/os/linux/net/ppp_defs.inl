#pragma once

/**
 * @namespace stdx::os::linux::net
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::net {
    #ifdef __linux__
    using NetworkProtocolMode = ::NPmode;
    using PppStat = ::pppstat;
    using VjStat = ::vjstat;
    using CompStat = ::compstat;
    using PppStats = ::ppp_stats;
    using PppCompStats = ::ppp_comp_stats;
    using PppIdle = ::ppp_idle;
    using PppIdle32 = ::ppp_idle32;
    using PppIdle64 = ::ppp_idle64;
    #endif
}
