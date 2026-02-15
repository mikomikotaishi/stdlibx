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

export module stdx:os.linux.netinet.if_tr;

/**
 * @namespace stdx::os::linux::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::netinet {
    #ifdef __linux__
    using TokenRingFrameHeader = ::trh_hdr;
    using TokenRingLlc = ::trllc;
    using TokenRingStatistics = ::tr_statistics;
    using TokenRingNHeader = ::trn_hdr;
    #endif
}
