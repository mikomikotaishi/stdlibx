/**
 * @file random.cppm
 * @module stdx:os.linux.sys.random
 * @brief Module file for Unix random number retrieval operations.
 *
 * This file contains the implementation of the Unix random number retrieval operations from the POSIX libraries,
 * located in <sys/random.h>.
 */

module;

#ifdef __linux__
#include <sys/random.h>
#endif

export module stdx:os.linux.sys.random;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::getrandom;
    using ::getentropy;
    #endif
}
