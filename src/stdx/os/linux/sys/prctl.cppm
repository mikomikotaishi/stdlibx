/**
 * @file prctl.cppm
 * @module stdx:os.linux.sys.prctl
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/prctl.h>.
 */

module;

#ifdef __linux__
#include <sys/prctl.h>
#endif

export module stdx:os.linux.sys.prctl;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::prctl;
    #endif
}
