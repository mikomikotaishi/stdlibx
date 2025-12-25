/**
 * @file single_threaded.cppm
 * @module stdx:os.linux.sys.single_threaded
 * @brief Module file for Unix single-threaded optimisation support.
 *
 * This file contains the implementation of the Unix single-threaded optimisation support from the POSIX libraries,
 * located in <sys/single_threaded.h>.
 */

module;

#ifdef __linux__
#include <sys/single_threaded.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.single_threaded;
#else
export module stdlibx:os.linux.sys.single_threaded;
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
    using ::__libc_single_threaded;
    #endif
}
