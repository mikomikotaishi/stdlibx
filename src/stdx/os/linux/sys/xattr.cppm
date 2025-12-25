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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.xattr;
#else
export module stdlibx:os.linux.sys.xattr;
#endif

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::sys {
#else 
export namespace stdlibx::os::linux::sys {
#endif
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
