/**
 * @file stat.cppm
 * @module stdx:os.unix.sys.stat
 * @brief Module file for Unix file information operations.
 *
 * This file contains the implementation of the Unix file information operations from the POSIX libraries,
 * located in <sys/stat.h>.
 */

module;

#ifdef __unix__
#include <sys/stat.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sys.stat;
#else
export module stdlibx:os.unix.sys.stat;
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
    using ::stat;
    using ::fstat;
    using ::stat64;
    using ::fstat64;
    using ::fstatat;
    using ::fstatat64;
    using ::lstat;
    using ::lstat64;
    using ::chmod;
    using ::lchmod;
    using ::fchmod;
    using ::fchmodat;
    using ::umask;
    using ::getumask;
    using ::mkdir;
    using ::mkdirat;
    using ::mknod;
    using ::mknodat;
    using ::mkfifo;
    using ::mkfifoat;
    using ::utimensat;
    using ::futimens;
    #endif
}
