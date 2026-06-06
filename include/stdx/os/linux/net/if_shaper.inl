#pragma once

/**
 * @namespace stdx::os::linux::net
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::net {
    #if defined(__linux__) && __has_include(<net/if_shaper.h>)
    using ShaperConfig = ::shaperconf;
    #endif
}
