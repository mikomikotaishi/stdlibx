/**
 * @file ftw.cppm
 * @module stdx:os.unix.ftw
 * @brief Module file for Unix file tree traversal operations.
 *
 * This file contains the implementation of the Unix file tree traversal operations from the POSIX libraries,
 * located in <ftw.h>.
 */

module;

#ifdef __unix__
#include <ftw.h>
#endif

export module stdx:os.unix.ftw;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using Ftw = ::FTW;

    using ::ftw;
    using ::ftw64;
    using ::nftw;
    using ::nftw64;
    #endif
}
