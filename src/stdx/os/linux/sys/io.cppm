/**
 * @file io.cppm
 * @module stdx:os.linux.sys.io
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/io.h>.
 */

module;

#ifdef __linux__
#include <sys/io.h>
#endif

export module stdx:os.linux.sys.io;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::ioperm;
    using ::iopl;
    #endif
}
