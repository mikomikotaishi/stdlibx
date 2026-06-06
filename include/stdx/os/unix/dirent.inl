#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #if defined(__unix__) && __has_include(<dirent.h>)
    using DirectoryEntry64 = ::dirent64;
    using ::closedir;
    using ::opendir;
    using ::fdopendir;
    using ::readdir;
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
