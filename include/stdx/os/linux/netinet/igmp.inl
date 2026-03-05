#pragma once

/**
 * @namespace stdx::os::linux::netinet
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::netinet {
    #ifdef __linux__
    using Igmp = ::igmp;
    #endif
}
