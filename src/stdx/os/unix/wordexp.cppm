/**
 * @file wordexp.cppm
 * @module stdx:os.unix.wordexp
 * @brief Module file for Unix word expansion operations.
 *
 * This file contains the implementation of the Unix word expansion operations from the POSIX libraries,
 * located in <wordexp.h>.
 */

module;

#ifdef __unix__
#include <wordexp.h>
#endif

export module stdx:os.unix.wordexp;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using ::wordexp;
    using ::wordfree;
    #endif
}
