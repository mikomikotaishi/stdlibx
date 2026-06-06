#pragma once

/**
 * @namespace stdx::os::unix::net
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix::net {
    #if defined(__unix__) && __has_include(<net/if.h>)
    using InterfaceAddress = ::ifaddr;
    using InterfaceMap = ::ifmap;
    using InterfaceRequest = ::ifreq;
    using InterfaceConfig = ::ifconf;
    using InterfaceNameIndex = struct if_nameindex;

    using ::if_nametoindex;
    using ::if_indextoname;
    using ::if_nameindex;
    using ::if_freenameindex;
    #endif
}
