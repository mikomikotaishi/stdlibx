/**
 * @file file.cppm
 * @module stdx:os.linux.sys.file
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/file.h>.
 */

module;

#ifdef __linux__
#include <sys/file.h>
#endif

export module stdx:os.linux.sys.file;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::flock;
    #endif
}
