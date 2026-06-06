#pragma once

/**
 * @namespace stdx::os::linux::arpa
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::arpa {
    #if defined(__linux__) && __has_include(<arpa/tftp.h>)
    using TftpHeader = ::tftphdr;
    #endif
}
