/**
 * @file signalfd.cppm
 * @module stdx:os.linux.sys.signalfd
 * @brief Module file for Unix operations, located in <sys/signalfd.h>
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/signalfd.h>.
 */

module;

#ifdef __linux__
#include <sys/signalfd.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.signalfd;
#else
export module stdlibx:os.linux.sys.signalfd;
#endif

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::sys {
#else 
export namespace stdlibx::os::linux::sys {
#endif
    #ifdef __linux__
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using SignalFdSigInfo = ::signalfd_siginfo;
    #endif

    using SignalFileDescriptionSignatureInfo = ::signalfd_siginfo;

    using ::signalfd;
    #endif
}
