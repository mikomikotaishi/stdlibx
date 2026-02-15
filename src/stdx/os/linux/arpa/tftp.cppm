/**
 * @file tftp.cppm
 * @module stdx:os.linux.arpa.tftp
 * @brief Module file for Unix ARPA operations.
 *
 * This file contains the implementation of the Unix ARPA operations from the POSIX libraries,
 * located in <arpa/tftp.h>.
 */

module;

#ifdef __linux__
#include <arpa/tftp.h>
#endif

export module stdx:os.linux.arpa.tftp;

/**
 * @namespace stdx::os::linux::arpa
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::arpa {
    #ifdef __linux__
    using TftpHeader = ::tftphdr;
    #endif
}
