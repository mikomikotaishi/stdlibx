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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.ftw;
#else
export module stdlibx:os.unix.ftw;
#endif

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix {
#else 
export namespace stdlibx::os::unix {
#endif
    #ifdef __unix__
    using ::FTW;

    using ::ftw;
    using ::ftw64;
    using ::nftw;
    using ::nftw64;
    #endif
}
