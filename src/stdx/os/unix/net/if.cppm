/**
 * @file if.cppm
 * @module stdx:os.unix.net.if_header
 * @brief Module file for Unix local network interfacing operations.
 *
 * This file contains the implementation of the Unix local networking interfacing operations from the POSIX libraries,
 * located in <net/if.h>.
 * It is named if_header.cppm rather than if.cppm due to C++ keyword limitations.
 */

module;

#ifdef __unix__
#include <net/if.h>
#endif

export module stdx:os.unix.net.if_header;

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

    using ::if_nametoindex;
    using ::if_indextoname;
    using ::if_nameindex;
    using ::if_freenameindex;
    #endif
}
