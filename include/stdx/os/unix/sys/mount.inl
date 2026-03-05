#pragma once

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
