/**
 * @file fcntl.cppm
 * @module stdx:os.unix.fcntl
 * @brief Module file for Unix file control operations.
 *
 * This file contains the implementation of the Unix file control operations from the POSIX libraries,
 * located in <fcntl.h>.
 */

module;

#ifdef __unix__
#include <fcntl.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.fcntl;
#else
export module stdlibx:os.unix.fcntl;
#endif

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix {
#else 
export namespace stdlibx::os::unix {
#endif
    #ifdef __unix__
    using ::fcntl;
    using ::fcntl64;
    using ::open;
    using ::open64;
    using ::openat;
    using ::openat64;
    using ::creat;
    using ::creat64;
    using ::lockf;
    using ::lockf64;
    using ::posix_fadvise;
    using ::posix_fadvise64;
    using ::posix_fallocate;
    using ::posix_fallocate64;
    #endif
}
