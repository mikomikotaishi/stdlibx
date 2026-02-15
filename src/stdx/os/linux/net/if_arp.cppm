/**
 * @file if_arp.cppm
 * @module stdx:os.linux.net.if_arp
 * @brief Module file for Unix local network interfacing operations.
 *
 * This file contains the implementation of the Unix local networking interfacing operations from the POSIX libraries,
 * located in <net/if_arp.h>.
 */

module;

#ifdef __linux__
#include <net/if_arp.h>
#endif

export module stdx:os.linux.net.if_arp;

/**
 * @namespace stdx::os::linux::net
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::net {
    #ifdef __linux__
    using ArpHeader = ::arphdr;
    using ArpRequest = ::arpreq;
    using ArpRequestOld = ::arpreq_old;
    using ArpDaemonRequest = ::arpd_request;
    #endif
}
