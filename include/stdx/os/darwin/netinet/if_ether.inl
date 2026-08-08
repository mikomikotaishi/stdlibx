#pragma once

/**
 * @namespace stdx::os::darwin::netinet
 * @brief ARP over Ethernet - <netinet/if_ether.h>.
 */
export namespace stdx::os::darwin::netinet {
    #if defined(__APPLE__) && __has_include(<netinet/if_ether.h>)
    using EthernetArp = ::ether_arp;
    #endif
}
