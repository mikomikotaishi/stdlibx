/**
 * @file utime.cppm
 * @module stdx:os.unix.utime
 * @brief Module file for Unix inode access operations.
 *
 * This file contains the implementation of the Unix inode access operations from the POSIX libraries,
 * located in <utime.h>.
 */

module;

#ifdef __unix__
#include <utime.h>
#endif

export module stdx:os.unix.utime;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using UpdateTimeBuffer = ::utimbuf;

    using ::utime;
    #endif
}
