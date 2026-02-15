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

export module stdx:os.linux.netinet.udp;

/**
 * @namespace stdx::os::linux::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::posix::netinet {
    #ifdef __linux__
    using UdpHeader = ::udphdr;
    #endif
}
