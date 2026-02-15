/**
 * @file langinfo.cppm
 * @module stdx:os.unix.langinfo
 * @brief Module file for Unix language information operations.
 *
 * This file contains the implementation of the Unix language information operations from the POSIX libraries,
 * located in <langinfo.h>.
 */

module;

#ifdef __unix__
#include <langinfo.h>
#endif

export module stdx:os.unix.langinfo;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using ::nl_langinfo;
    using ::nl_langinfo_l;
    #endif
}
