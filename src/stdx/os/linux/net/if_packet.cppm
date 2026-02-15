/**
 * @file if_packet.cppm
 * @module stdx:os.linux.net.if_packet
 * @brief Module file for Unix local network interfacing operations.
 *
 * This file contains the implementation of the Unix local networking interfacing operations from the POSIX libraries,
 * located in <net/if_packet.h>.
 */

module;

#ifdef __linux__
#include <net/if_packet.h>
#endif

export module stdx:os.linux.net.if_packet;

/**
 * @namespace stdx::os::linux::net
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::net {
    #ifdef __linux__
    using SocketAddressPacket = ::sockaddr_pkt;
    #endif
}
