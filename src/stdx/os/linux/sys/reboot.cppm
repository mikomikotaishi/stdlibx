/**
 * @file reboot.cppm
 * @module stdx:os.linux.sys.reboot
 * @brief Module file for Unix reboot operations.
 *
 * This file contains the implementation of the Unix reboot operations from the POSIX libraries,
 * located in <sys/reboot.h>.
 */

module;

#ifdef __linux__
#include <sys/reboot.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.reboot;
#else
export module stdlibx:os.linux.sys.reboot;
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
    using ::reboot;
    #endif
}
