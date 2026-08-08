#pragma once

/**
 * @namespace stdx::os::darwin::netinet
 * @brief The network byte order integer types the IP headers are declared with
 *        - <netinet/in_systm.h>.
 */
export namespace stdx::os::darwin::netinet {
    #if defined(__APPLE__) && __has_include(<netinet/in_systm.h>)
    using NetworkShort = ::n_short;
    using NetworkLong = ::n_long;
    using NetworkTime = ::n_time;
    #endif
}
