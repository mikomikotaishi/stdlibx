/**
 * @file ethernet.cppm
 * @module stdx:os.linux.net.ethernet
 * @brief Module file for Unix local network interfacing operations.
 *
 * This file contains the implementation of the Unix local networking interfacing operations from the POSIX libraries,
 * located in <net/ethernet.h>.
 */

module;

#ifdef __linux__
#include <net/ethernet.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.net.ethernet;
#else
export module stdlibx:os.linux.net.ethernet;
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
    using EtherAddr = ::ether_addr;
    using EtherHeader = ::ether_header;
    #endif

    using EthernetAddress = ::ether_addr;
    using EthernetHeader = ::ether_header;
    #endif
}
