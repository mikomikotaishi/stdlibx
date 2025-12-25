/**
 * @file grp.cppm
 * @module stdx:os.unix.grp
 * @brief Module file for Unix user group control operations.
 *
 * This file contains the implementation of the Unix user group control operations from the POSIX libraries,
 * located in <grp.h>.
 */

module;

#ifdef __unix__
#include <grp.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.grp;
#else
export module stdlibx:os.unix.grp;
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
    using Group = ::group;

    using ::setgrent;
    using ::endgrent;
    using ::getgrent;
    using ::fgetgrent;
    using ::putgrent;
    using ::getgrgid;
    using ::getgrnam;
    using ::getgrent_r;
    using ::getgrgid_r;
    using ::getgrnam_r;
    using ::fgetgrent_r;
    using ::setgroups;
    using ::getgrouplist;
    using ::initgroups;
    #endif
}
