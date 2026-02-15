/**
 * @file igmp.cppm
 * @module stdx:os.linux.netinet.igmp
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <netinet/igmp.h>.
 */

module;

#ifdef __linux__
#include <netinet/igmp.h>
#endif

export module stdx:os.linux.netinet.igmp;

/**
 * @namespace stdx::os::linux::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::netinet {
    #ifdef __linux__
    using Igmp = ::igmp;
    #endif
}
