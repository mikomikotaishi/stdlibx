#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/xattr.h>)
    using ::setxattr;
    using ::lsetxattr;
    using ::fsetxattr;
    using ::getxattr;
    using ::lgetxattr;
    using ::fgetxattr;
    using ::listxattr;
    using ::llistxattr;
    using ::flistxattr;
    using ::removexattr;
    using ::lremovexattr;
    using ::fremovexattr;
    #endif
}
