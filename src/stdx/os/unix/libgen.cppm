/**
 * @file libgen.cppm
 * @module stdx:os.unix.libgen
 * @brief Module file for Unix pathname manipulation operations.
 *
 * This file contains the implementation of the Unix pathname manipulation operations from the POSIX libraries,
 * located in <libgen.h>.
 */

module;

#ifdef __unix__
#include <libgen.h>
#endif

export module stdx:os.unix.libgen;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using ::dirname;
    #endif
}
