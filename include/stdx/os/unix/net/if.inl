#pragma once

/**
 * @namespace stdx::os::unix::net
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::net {
    #ifdef __unix__
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
