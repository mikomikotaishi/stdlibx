#pragma once

/**
 * @namespace stdx::os::darwin::net
 * @brief Ethernet framing - <net/ethernet.h>.
 */
export namespace stdx::os::darwin::net {
    #if defined(__APPLE__) && __has_include(<net/ethernet.h>)
    using EthernetAddress = ::ether_addr;
    using EthernetHeader = ::ether_header;

    using ::ether_ntoa;
    using ::ether_aton;
    using ::ether_ntohost;
    using ::ether_hostton;
    using ::ether_line;
    #endif
}
