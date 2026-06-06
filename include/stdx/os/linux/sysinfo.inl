#pragma once

/**
 * @namespace stdx::os::linux
 * @brief Linux operations.
 */
export namespace stdx::os::linux {
    #if defined(__linux__) && __has_include(<linux/sysinfo.h>)
    using LinuxSystemInformation = struct sysinfo;
    #endif
}
