/**
 * @file if_shaper.cppm
 * @module stdx:os.linux.net.if_shaper
 * @brief Module file for Unix local network interfacing operations.
 *
 * This file contains the implementation of the Unix local networking interfacing operations from the POSIX libraries,
 * located in <net/if_shaper.h>.
 */

module;

#ifdef __linux__
#include <net/if_shaper.h>
#endif

export module stdx:os.linux.net.if_shaper;

/**
 * @namespace stdx::os::linux::net
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::net {
    #ifdef __linux__
    using ShaperConfig = ::shaperconf;
    #endif
}
