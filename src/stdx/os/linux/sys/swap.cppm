/**
 * @file swap.cppm
 * @module stdx:os.linux.sys.swap
 * @brief Module file for Unix swapping enable/disable operations.
 *
 * This file contains the implementation of the Unix swapping enable/disable operations from the POSIX libraries,
 * located in <sys/swap.h>.
 */

module;

#ifdef __linux__
#include <sys/swap.h>
#endif

export module stdx:os.linux.sys.swap;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::swapon;
    using ::swapoff;
    #endif
}
