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

export module stdx:os.linux.netinet.ether;

/**
 * @namespace stdx::os::linux::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::netinet {
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
