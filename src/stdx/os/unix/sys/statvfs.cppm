/**
 * @file statvfs.cppm
 * @module stdx:os.unix.sys.statvfs
 * @brief Module file for Unix filesystem information operations.
 *
 * This file contains the implementation of the Unix filesystem information operations from the POSIX libraries,
 * located in <sys/statvfs.h>.
 */

module;

#ifdef __unix__
#include <sys/statvfs.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sys.statvfs;
#else
export module stdlibx:os.unix.sys.statvfs;
#endif

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix::sys {
#else 
export namespace stdlibx::os::unix::sys {
#endif
    #ifdef __unix__
    using ::statvfs;
    using ::statvfs64;
    using ::fstatvfs;
    using ::fstatvfs64;
    #endif
}
