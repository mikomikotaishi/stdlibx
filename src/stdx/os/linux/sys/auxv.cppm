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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.auxv;
#else
export module stdlibx:os.linux.sys.auxv;
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
    using ::getauxval;
    #endif
}
