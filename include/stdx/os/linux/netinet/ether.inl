#pragma once

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
