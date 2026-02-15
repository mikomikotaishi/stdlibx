/**
 * @file xattr.cppm
 * @module stdx:os.linux.sys.xattr
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/xattr.h>.
 */

module;

#ifdef __linux__
#include <sys/xattr.h>
#endif

export module stdx:os.linux.sys.xattr;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
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
