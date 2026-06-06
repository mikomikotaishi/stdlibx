#pragma once

/**
 * @namespace stdx::os::linux::net
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::net {
    #if defined(__linux__) && __has_include(<net/ethernet.h>)
    using EthernetAddress = ::ether_addr;
    using EthernetHeader = ::ether_header;
    #endif
}
