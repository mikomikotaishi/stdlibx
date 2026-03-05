#pragma once

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
