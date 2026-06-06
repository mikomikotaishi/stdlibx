#pragma once

/**
 * @namespace stdx::os::unix::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #if defined(__unix__) && __has_include(<sys/stat.h>)
    using Stat = struct stat;
    using Stat64 = struct stat64;

    using ::stat;
    using ::fstat;
    using ::stat64;
    using ::fstat64;
    using ::fstatat;
    using ::fstatat64;
    using ::lstat;
    using ::lstat64;
    using ::chmod;
    using ::lchmod;
    using ::fchmod;
    using ::fchmodat;
    using ::umask;
    using ::getumask;
    using ::mkdir;
    using ::mkdirat;
    using ::mknod;
    using ::mknodat;
    using ::mkfifo;
    using ::mkfifoat;
    using ::utimensat;
    using ::futimens;
    #endif
}
