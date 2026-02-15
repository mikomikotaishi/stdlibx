/**
 * @file personality.cppm
 * @module stdx:os.linux.sys.personality
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/personality.h>.
 */

module;

#ifdef __linux__
#include <sys/personality.h>
#endif

export module stdx:os.linux.sys.personality;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::personality;
    #endif
}
