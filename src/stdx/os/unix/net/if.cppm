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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.net.if_header;
#else
export module stdlibx:os.unix.net.if_header;
#endif

/**
 * @namespace stdx::os::unix::net
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix::net {
#else 
export namespace stdlibx::os::unix::net {
#endif
    #ifdef __unix__
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using IfAddr = ::ifaddr;
    using IfMap = ::ifmap;
    using IfReq = ::ifreq;
    using IfConf = ::ifconf;
    #endif

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
