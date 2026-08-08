#pragma once

/**
 * @namespace stdx::os::darwin::net
 * @brief Interface media state - <net/if_media.h>.
 */
export namespace stdx::os::darwin::net {
    #if defined(__APPLE__) && __has_include(<net/if_media.h>)
    using InterfaceMediaRequest = ::ifmediareq;
    #endif
}
