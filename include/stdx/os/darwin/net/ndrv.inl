#pragma once

/**
 * @namespace stdx::os::darwin::net
 * @brief Raw network driver access, the AF_NDRV family - <net/ndrv.h>.
 */
export namespace stdx::os::darwin::net {
    #if defined(__APPLE__) && __has_include(<net/ndrv.h>)
    using SocketAddressNdrv = ::sockaddr_ndrv;
    using NdrvProtocolDescription = ::ndrv_protocol_desc;
    using NdrvDemuxDescription = ::ndrv_demux_desc;
    #endif
}
