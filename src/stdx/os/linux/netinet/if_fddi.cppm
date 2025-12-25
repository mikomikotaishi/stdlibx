/**
 * @file if_fddi.cppm
 * @module stdx:os.linux.netinet.if_fddi
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <netinet/if_fddi.h>.
 */

module;

#ifdef __linux__
#include <netinet/if_fddi.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.netinet.if_fddi;
#else
export module stdlibx:os.linux.netinet.if_fddi;
#endif

/**
 * @namespace stdx::os::linux::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::netinet {
#else 
export namespace stdlibx::os::linux::netinet {
#endif
    #ifdef __linux__
    using FddiHeader = ::fddi_header;
    #endif
}
