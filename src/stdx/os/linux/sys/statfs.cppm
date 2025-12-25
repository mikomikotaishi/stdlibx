/**
 * @file statfs.cppm
 * @module stdx:os.linux.sys.statfs
 * @brief Module file for Unix filesystem information retrieval operations.
 *
 * This file contains the implementation of the Unix filesystem information retrieval operations from the POSIX libraries,
 * located in <sys/statfs.h>.
 */

module;

#ifdef __linux__
#include <sys/statfs.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.statfs;
#else
export module stdlibx:os.linux.sys.statfs;
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
    using ::statfs;
    using ::statfs64;
    using ::fstatfs;
    using ::fstatfs64;
    #endif
}
