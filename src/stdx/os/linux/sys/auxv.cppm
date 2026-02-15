/**
 * @file auxv.cppm
 * @module stdx:os.linux.sys.auxv
 * @brief Module file for Unix auxilliary vector operations.
 *
 * This file contains the implementation of the Unix auxilliary vector operations from the POSIX libraries,
 * located in <sys/auxv.h>.
 */

module;

#ifdef __linux__
#include <sys/auxv.h>
#endif

export module stdx:os.linux.sys.auxv;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::getauxval;
    #endif
}
