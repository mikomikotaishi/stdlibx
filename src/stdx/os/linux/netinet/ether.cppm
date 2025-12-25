/**
 * @file ether.cppm
 * @module stdx:os.linux.netinet.ether
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <netinet/ether.h>.
 */

module;

#ifdef __linux__
#include <netinet/ether.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.netinet.ether;
#else
export module stdlibx:os.linux.netinet.ether;
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
    using ::ether_ntoa;
    using ::ether_ntoa_r;
    using ::ether_aton;
    using ::ether_aton_r;
    using ::ether_ntohost;
    using ::ether_hostton;
    using ::ether_line;
    #endif
}
