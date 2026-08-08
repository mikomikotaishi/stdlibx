#pragma once

/**
 * @namespace stdx::os::darwin::sys
 * @brief Extended attributes - <sys/xattr.h>.
 *
 * Darwin's getxattr and setxattr take an extra `position` argument that the
 * Linux calls of the same name do not, so these are not interchangeable with
 * the ones in stdx::os::linux::sys.
 */
export namespace stdx::os::darwin::sys {
    #if defined(__APPLE__) && __has_include(<sys/xattr.h>)
    using ::getxattr;
    using ::fgetxattr;
    using ::setxattr;
    using ::fsetxattr;
    using ::removexattr;
    using ::fremovexattr;
    using ::listxattr;
    using ::flistxattr;
    #endif
}
