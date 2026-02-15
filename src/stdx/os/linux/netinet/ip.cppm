/**
 * @file ip.cppm
 * @module stdx:os.linux.netinet.ip
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <netinet/ip.h>.
 */

module;

#ifdef __linux__
#include <netinet/ip.h>
#endif

export module stdx:os.linux.netinet.ip;

/**
 * @namespace stdx::os::linux::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::netinet {
    #ifdef __linux__
    using TimeStamp = ::timestamp;
    using IpHeader = ::iphdr;
    using Ip = ::ip;
    using IpTimeStamp = ::ip_timestamp;
    #endif
}
