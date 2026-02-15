/**
 * @file select.cppm
 * @module stdx:os.unix.sys.select
 * @brief Module file for Unix fd_set type and select/pselect declaration operations.
 *
 * This file contains the implementation of the Unix fd_set type and select/pselect declaration operations from the POSIX libraries,
 * located in <sys/select.h>.
 */

module;

#ifdef __unix__
#include <sys/select.h>
#endif

export module stdx:os.unix.sys.select;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using ::select;
    using ::pselect;
    #endif
}
