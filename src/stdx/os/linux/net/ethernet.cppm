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

export module stdx:os.linux.net.ethernet;

/**
 * @namespace stdx::os::linux::net
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::net {
    #ifdef __linux__
    using EthernetAddress = ::ether_addr;
    using EthernetHeader = ::ether_header;
    #endif
}
