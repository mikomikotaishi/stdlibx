#pragma once

/**
 * @namespace stdx::os::linux::net
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::net {
    #if defined(__linux__) && __has_include(<net/ppp_defs.h>)
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
