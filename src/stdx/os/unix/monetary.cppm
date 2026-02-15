/**
 * @file monetary.cppm
 * @module stdx:os.unix.monetary
 * @brief Module file for Unix monetary unit string formatting operations.
 *
 * This file contains the implementation of the Unix monetary unit string formatting operations from the POSIX libraries,
 * located in <monetary.h>.
 */

module;

#ifdef __unix__
#include <monetary.h>
#endif

export module stdx:os.unix.monetary;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using ::strfmon;
    using ::strfmon_l;
    #endif
}
