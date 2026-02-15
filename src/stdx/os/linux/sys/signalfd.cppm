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

export module stdx:os.linux.sys.signalfd;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using SignalFileDescriptionSignatureInfo = ::signalfd_siginfo;

    using ::signalfd;
    #endif
}
