/**
 * @file udp.cppm
 * @module stdx:os.linux.netinet.udp
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <netinet/udp.h>.
 */

module;

#ifdef __linux__
#include <netinet/udp.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.netinet.udp;
#else
export module stdlibx:os.linux.netinet.udp;
#endif

/**
 * @namespace stdx::os::linux::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::posix::netinet {
#else 
export namespace stdlibx::os::posix::netinet {
#endif
    #ifdef __linux__
    using UDPHdr = ::udphdr;
    #endif
}
