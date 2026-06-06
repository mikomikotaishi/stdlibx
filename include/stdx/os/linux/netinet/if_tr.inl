#pragma once

/**
 * @namespace stdx::os::linux::netinet
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::netinet {
    #if defined(__linux__) && __has_include(<netinet/if_tr.h>)
    using TokenRingFrameHeader = ::trh_hdr;
    using TokenRingLlc = ::trllc;
    using TokenRingStatistics = ::tr_statistics;
    using TokenRingNHeader = ::trn_hdr;
    #endif
}
