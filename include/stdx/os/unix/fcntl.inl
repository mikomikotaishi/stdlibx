#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX file-control operations.
 */
export namespace stdx::os::unix {
    #if defined(__unix__) && __has_include(<fcntl.h>)
    using ::open;
    using ::open64;
    using ::openat;
    using ::openat64;
    using ::creat;
    using ::creat64;
    using ::fcntl;
    using ::posix_fadvise;
    using ::posix_fadvise64;
    using ::posix_fallocate;
    using ::posix_fallocate64;
    #endif
}
