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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.swap;
#else
export module stdlibx:os.linux.sys.swap;
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
    using ::swapon;
    using ::swapoff;
    #endif
}
