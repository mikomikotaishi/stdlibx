/**
 * @file ucontext.cppm
 * @module stdx:os.linux.sys.ucontext
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/ucontext.h>.
 */

module;

#ifdef __linux__
#include <sys/ucontext.h>
#endif

export module stdx:os.linux.sys.ucontext;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using UserlevelContext = ::ucontext_t;
    #endif
}
