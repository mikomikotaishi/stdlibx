/**
 * @file if_ether.cppm
 * @module stdx:os.linux.netinet.if_ether
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <netinet/if_ether.h>.
 */

module;

#ifdef __linux__
#include <netinet/if_ether.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.netinet.if_ether;
#else
export module stdlibx:os.linux.netinet.if_ether;
#endif

/**
 * @namespace stdx::os::linux::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::netinet {
#else 
export namespace stdlibx::os::linux::netinet {
#endif
    #ifdef __linux__
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using EtherArp = ::ether_arp;
    #endif

    using EthernetArp = ::ether_arp;
    #endif
}
