#pragma once

/**
 * @namespace stdx::os::darwin::net
 * @brief Link layer addresses - <net/if_dl.h>.
 *
 * getifaddrs(3) hands back AF_LINK entries in this form on Darwin, where Linux
 * would use AF_PACKET and struct sockaddr_ll.
 */
export namespace stdx::os::darwin::net {
    #if defined(__APPLE__) && __has_include(<net/if_dl.h>)
    using SocketAddressDataLink = ::sockaddr_dl;

    using ::link_addr;
    using ::link_ntoa;
    #endif
}
