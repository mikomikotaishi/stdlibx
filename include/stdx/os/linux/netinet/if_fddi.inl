#pragma once

/**
 * @namespace stdx::os::linux::netinet
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::netinet {
    #if defined(__linux__) && __has_include(<netinet/if_fddi.h>)
    using FddiHeader = ::fddi_header;
    #endif
}
