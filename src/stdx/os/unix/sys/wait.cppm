/**
 * @file wait.cppm
 * @module stdx:os.unix.sys.wait
 * @brief Module file for Unix wait operations.
 *
 * This file contains the implementation of the Unix wait operations from the POSIX libraries,
 * located in <sys/wait.h>.
 */

module;

#ifdef __unix__
#include <sys/wait.h>
#endif

export module stdx:os.unix.sys.wait;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using ::wait;
    using ::waitpid;
    using ::waitid;
    using ::wait3;
    using ::wait4;
    #endif
}
