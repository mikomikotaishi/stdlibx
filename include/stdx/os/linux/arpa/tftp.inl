#pragma once

/**
 * @namespace stdx::os::linux::arpa
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::arpa {
    #ifdef __linux__
    using TftpHeader = ::tftphdr;
    #endif
}
