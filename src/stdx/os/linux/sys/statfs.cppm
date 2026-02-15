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

export module stdx:os.linux.sys.statfs;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::statfs;
    using ::statfs64;
    using ::fstatfs;
    using ::fstatfs64;
    #endif
}
