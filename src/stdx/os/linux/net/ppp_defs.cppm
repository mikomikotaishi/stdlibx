/**
 * @file ppp_defs.cppm
 * @module stdx:os.linux.net.ppp_defs
 * @brief Module file for Unix local network interfacing operations.
 *
 * This file contains the implementation of the Unix local networking interfacing operations from the POSIX libraries,
 * located in <net/ppp_defs.h>.
 */

module;

#ifdef __linux__
#include <net/ppp_defs.h>
#endif

export module stdx:os.linux.net.ppp_defs;

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
