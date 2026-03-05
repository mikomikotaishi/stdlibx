#pragma once

/**
 * @namespace stdx::os::linux
 * @brief Wrapper namespace for Linux operations.
 */
export namespace stdx::os::linux {
    #ifdef __linux__
    using LinuxSystemInformation = struct sysinfo;
    #endif
}
