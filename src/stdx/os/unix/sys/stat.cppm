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

export module stdx:os.unix.sys.stat;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
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
