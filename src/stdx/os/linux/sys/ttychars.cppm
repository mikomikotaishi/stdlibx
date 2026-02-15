/**
 * @file ttychars.cppm
 * @module stdx:os.linux.sys.ttychars
 * @brief Module file for Unix terminal handling constants.
 *
 * This file contains the implementation of the Unix terminal handling constants from the POSIX libraries,
 * located in <sys/ttychars.h>.
 */

module;

#ifdef __linux__
#include <sys/ttychars.h>
#endif

export module stdx:os.linux.sys.ttychars;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using TeletypewriterChars = ::ttychars;
    #endif
}
