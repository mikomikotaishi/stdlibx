#pragma once

/**
 * @namespace stdx::os::darwin::netinet
 * @brief IGMP - <netinet/igmp.h>.
 */
export namespace stdx::os::darwin::netinet {
    #if defined(__APPLE__) && __has_include(<netinet/igmp.h>)
    using IgmpHeader = ::igmp;
    #endif
}
