#pragma once

/**
 * @namespace stdx::os::linux::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::posix::netinet {
    #ifdef __linux__
    using UdpHeader = ::udphdr;
    #endif
}
