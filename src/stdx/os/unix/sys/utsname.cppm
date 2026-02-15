/**
 * @file utsname.cppm
 * @module stdx:os.unix.sys.utsname
 * @brief Module file for Unix operating system information operations.
 *
 * This file contains the implementation of the Unix operating system information operations from the POSIX libraries,
 * located in <sys/utsname.h>.
 */

module;

#ifdef __unix__
#include <sys/utsname.h>
#endif

export module stdx:os.unix.sys.utsname;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using UnixTimeSystemName = ::utsname;

    using ::uname;
    #endif
}
