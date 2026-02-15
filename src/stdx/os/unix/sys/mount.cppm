/**
 * @file mount.cppm
 * @module stdx:os.unix.sys.mount
 * @brief Module file for Unix filesystem mounting/unmounting operations.
 *
 * This file contains the implementation of the Unix filesystem mounting/unmounting operations from the POSIX libraries,
 * located in <sys/mount.h>.
 */

module;

#ifdef __unix__
#include <sys/mount.h>
#endif

export module stdx:os.unix.sys.mount;

export import :os.unix.sys.mount.constants;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using MountAttribute = ::mount_attr;
    using FileSystemConfigCommand = ::fsconfig_command;

    using ::mount;
    using ::umount;
    using ::umount2;
    using ::fsopen;
    using ::fsmount;
    using ::move_mount;
    using ::fsconfig;
    using ::fspick;
    using ::open_tree;
    using ::mount_setattr;
    #endif
}
