#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX file-control operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
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
