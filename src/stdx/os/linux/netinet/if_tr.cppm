/**
 * @file if_tr.cppm
 * @module stdx:os.linux.netinet.if_tr
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <netinet/if_tr.h>.
 */

module;

#ifdef __linux__
#include <netinet/if_tr.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.netinet.if_tr;
#else
export module stdlibx:os.linux.netinet.if_tr;
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
    using TRHHdr = ::trh_hdr;
    using TRLLC = ::trllc;
    using TRStatistics = ::tr_statistics;
    using TRNHdr = ::trn_hdr;
    #endif
}
