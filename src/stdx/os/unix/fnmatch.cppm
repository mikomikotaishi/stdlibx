/**
 * @file fnmatch.cppm
 * @module stdx:os.unix.fnmatch
 * @brief Module file for Unix filename matching operations.
 *
 * This file contains the implementation of the Unix filename matching operations from the POSIX libraries,
 * located in <fnmatch.h>.
 */

module;

#ifdef __unix__
#include <fnmatch.h>
#endif

export module stdx:os.unix.fnmatch;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using ::fnmatch;
    #endif
}
