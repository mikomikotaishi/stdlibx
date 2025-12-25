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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.arpa.tftp;
#else
export module stdlibx:os.linux.arpa.tftp;
#endif

/**
 * @namespace stdx::os::linux::arpa
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::arpa {
#else 
export namespace stdlibx::os::linux::arpa {
#endif
    #ifdef __linux__
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using TftpHdr = ::tftphdr;
    #endif

    using TftpHeader = ::tftphdr;
    #endif
}
