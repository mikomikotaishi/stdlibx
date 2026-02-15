/**
 * @file sendfile.cppm
 * @module stdx:os.linux.sys.sendfile
 * @brief Module file for Unix sendfile operations.
 *
 * This file contains the implementation of the Unix sendfile operations from the POSIX libraries,
 * located in <sys/sendfile.h>.
 */

module;

#ifdef __linux__
#include <sys/sendfile.h>
#endif

export module stdx:os.linux.sys.sendfile;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::sendfile;
    using ::sendfile64;
    #endif
}
