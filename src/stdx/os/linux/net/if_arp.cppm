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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.net.if_arp;
#else
export module stdlibx:os.linux.net.if_arp;
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
    using ArpHdr = ::arphdr;
    using ArpReq = ::arpreq;
    using ArpReqOld = ::arpreq_old;
    using ArpDRequest = ::arpd_request;
    #endif

    using ArpHeader = ::arphdr;
    using ArpRequest = ::arpreq;
    using ArpRequestOld = ::arpreq_old;
    using ArpDaemonRequest = ::arpd_request;
    #endif
}
