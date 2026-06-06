#pragma once

/**
 * @namespace stdx::os::linux::net
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::net {
    #if defined(__linux__) && __has_include(<net/if_packet.h>)
    using SocketAddressPacket = ::sockaddr_pkt;
    #endif
}
