#pragma once

/**
 * @namespace stdx::os::darwin::netinet
 * @brief The UDP header - <netinet/udp.h>.
 */
export namespace stdx::os::darwin::netinet {
    #if defined(__APPLE__) && __has_include(<netinet/udp.h>)
    using UdpHeader = ::udphdr;
    #endif
}
