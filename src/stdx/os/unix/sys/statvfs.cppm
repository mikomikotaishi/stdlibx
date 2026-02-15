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

export module stdx:os.unix.sys.statvfs;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using ::statvfs;
    using ::statvfs64;
    using ::fstatvfs;
    using ::fstatvfs64;
    #endif
}
