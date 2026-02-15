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

export module stdx:os.linux.sys.reboot;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::reboot;
    #endif
}
