/**
 * @file dirent.cppm
 * @module stdx:os.unix.dirent
 * @brief Module file for Unix directory navigating operations.
 *
 * This file contains the implementation of the Unix directory navigating operations from the POSIX libraries,
 * located in <dirent.h>.
 */

module;

#ifdef __unix__
#include <dirent.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.dirent;
#else
export module stdlibx:os.unix.dirent;
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
    using ::closedir;
    using ::opendir;
    using ::fdopendir;
    using ::readdir;
    using ::dirent64;
    using ::readdir_r;
    using ::readdir64_r;
    using ::rewinddir;
    using ::seekdir;
    using ::telldir;
    using ::dirfd;
    using ::scandir;
    using ::scandir64;
    using ::scandirat;
    using ::scandirat64;
    using ::alphasort;
    using ::alphasort64;
    using ::getdirentries;
    using ::getdirentries64;
    using ::versionsort;
    using ::versionsort64;
    #endif
}
