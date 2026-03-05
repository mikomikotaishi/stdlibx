#pragma once

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
