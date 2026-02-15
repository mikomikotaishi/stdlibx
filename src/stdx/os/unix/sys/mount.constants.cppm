/**
 * @file mount.constants.cppm
 * @module stdx:os.unix.sys.mount.constants
 * @brief Module file for mount operations.
 *
 * This file contains the implementation of the constants used 
 * in <sys/mount.h>.
 */

module;

export module stdx:os.unix.sys.mount.constants;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    // Block size constants
    inline constexpr int BLOCK_SIZE_BITS = 10;
    inline constexpr int BLOCK_SIZE = (1 << BLOCK_SIZE_BITS);

    // Filesystem-independent mount flags
    inline constexpr unsigned long MS_RDONLY = 1; ///< Mount read-only
    inline constexpr unsigned long MS_NOSUID = 2; ///< Ignore suid and sgid bits
    inline constexpr unsigned long MS_NODEV = 4; ///< Disallow access to device special files
    inline constexpr unsigned long MS_NOEXEC = 8; ///< Disallow program execution
    inline constexpr unsigned long MS_SYNCHRONOUS = 16; ///< Writes are synced at once
    inline constexpr unsigned long MS_REMOUNT = 32; ///< Alter flags of a mounted FS
    inline constexpr unsigned long MS_MANDLOCK = 64; ///< Allow mandatory locks on an FS
    inline constexpr unsigned long MS_DIRSYNC = 128; ///< Directory modifications are synchronous
    inline constexpr unsigned long MS_NOSYMFOLLOW = 256; ///< Do not follow symlinks
    inline constexpr unsigned long MS_NOATIME = 1024; ///< Do not update access times
    inline constexpr unsigned long MS_NODIRATIME = 2048; ///< Do not update directory access times
    inline constexpr unsigned long MS_BIND = 4096; ///< Bind directory at different place
    inline constexpr unsigned long MS_MOVE = 8192;
    inline constexpr unsigned long MS_REC = 16384;
    inline constexpr unsigned long MS_SILENT = 32768;
    inline constexpr unsigned long MS_POSIXACL = 1 << 16; ///< VFS does not apply the umask
    inline constexpr unsigned long MS_UNBINDABLE = 1 << 17; ///< Change to unbindable
    inline constexpr unsigned long MS_PRIVATE = 1 << 18; ///< Change to private
    inline constexpr unsigned long MS_SLAVE = 1 << 19; ///< Change to slave
    inline constexpr unsigned long MS_SHARED = 1 << 20; ///< Change to shared
    inline constexpr unsigned long MS_RELATIME = 1 << 21; ///< Update atime relative to mtime/ctime
    inline constexpr unsigned long MS_KERNMOUNT = 1 << 22; ///< This is a kern_mount call
    inline constexpr unsigned long MS_I_VERSION = 1 << 23; ///< Update inode I_version field
    inline constexpr unsigned long MS_STRICTATIME = 1 << 24; ///< Always perform atime updates
    inline constexpr unsigned long MS_LAZYTIME = 1 << 25; ///< Update the on-disk [acm]times lazily
    inline constexpr unsigned long MS_ACTIVE = 1 << 30;
    inline constexpr unsigned long MS_NOUSER = 1U << 31;

    // Flags that can be altered by MS_REMOUNT
    inline constexpr unsigned long MS_RMT_MASK = (MS_RDONLY | MS_SYNCHRONOUS | MS_MANDLOCK | MS_I_VERSION | MS_LAZYTIME);

    // Magic mount flag number
    inline constexpr unsigned long MS_MGC_VAL = 0xc0ed0000; ///< Magic flag number to indicate "new" flags
    inline constexpr unsigned long MS_MGC_MSK = 0xffff0000; ///< Magic flag number mask

    // Block device ioctl constants
    inline constexpr int BLKROSET = 0x1000 | (0x12 << 8) | 93; ///< Set device read-only (0 = read-write)
    inline constexpr int BLKROGET = 0x1000 | (0x12 << 8) | 94; ///< Get read-only status (0 = read_write)
    inline constexpr int BLKRRPART = 0x1000 | (0x12 << 8) | 95; ///< Re-read partition table
    inline constexpr int BLKGETSIZE = 0x1000 | (0x12 << 8) | 96; ///< Return device size
    inline constexpr int BLKFLSBUF = 0x1000 | (0x12 << 8) | 97; ///< Flush buffer cache
    inline constexpr int BLKRASET = 0x1000 | (0x12 << 8) | 98; ///< Set read ahead for block device
    inline constexpr int BLKRAGET = 0x1000 | (0x12 << 8) | 99; ///< Get current read ahead setting
    inline constexpr int BLKFRASET = 0x1000 | (0x12 << 8) | 100; ///< Set filesystem read-ahead
    inline constexpr int BLKFRAGET = 0x1000 | (0x12 << 8) | 101; ///< Get filesystem read-ahead
    inline constexpr int BLKSECTSET = 0x1000 | (0x12 << 8) | 102; ///< Set max sectors per request
    inline constexpr int BLKSECTGET = 0x1000 | (0x12 << 8) | 103; ///< Get max sectors per request
    inline constexpr int BLKSSZGET = 0x1000 | (0x12 << 8) | 104; ///< Get block device sector size

    // Unmount flags
    inline constexpr int MNT_FORCE = 1; ///< Force unmounting
    inline constexpr int MNT_DETACH = 2; ///< Just detach from the tree
    inline constexpr int MNT_EXPIRE = 4; ///< Mark for expiry
    inline constexpr int UMOUNT_NOFOLLOW = 8; ///< Don't follow symlink on umount

    // fsmount flags
    inline constexpr int FSMOUNT_CLOEXEC = 0x00000001;

    // Mount attributes
    inline constexpr int MOUNT_ATTR_RDONLY = 0x00000001; ///< Mount read-only
    inline constexpr int MOUNT_ATTR_NOSUID = 0x00000002; ///< Ignore suid and sgid bits
    inline constexpr int MOUNT_ATTR_NODEV = 0x00000004; ///< Disallow access to device special files
    inline constexpr int MOUNT_ATTR_NOEXEC = 0x00000008; ///< Disallow program execution
    inline constexpr int MOUNT_ATTR__ATIME = 0x00000070; ///< Setting on how atime should be updated
    inline constexpr int MOUNT_ATTR_RELATIME = 0x00000000; ///< Update atime relative to mtime/ctime
    inline constexpr int MOUNT_ATTR_NOATIME = 0x00000010; ///< Do not update access times
    inline constexpr int MOUNT_ATTR_STRICTATIME = 0x00000020; ///< Always perform atime updates
    inline constexpr int MOUNT_ATTR_NODIRATIME = 0x00000080; ///< Do not update directory access times
    inline constexpr int MOUNT_ATTR_IDMAP = 0x00100000; ///< Idmap mount to @userns_fd in struct mount_attr
    inline constexpr int MOUNT_ATTR_NOSYMFOLLOW = 0x00200000; ///< Do not follow symlinks

    inline constexpr int MOUNT_ATTR_SIZE_VER0 = 32; ///< sizeof first published struct

    // move_mount flags
    inline constexpr int MOVE_MOUNT_F_SYMLINKS = 0x00000001; ///< Follow symlinks on from path
    inline constexpr int MOVE_MOUNT_F_AUTOMOUNTS = 0x00000002; ///< Follow automounts on from path
    inline constexpr int MOVE_MOUNT_F_EMPTY_PATH = 0x00000004; ///< Empty from path permitted
    inline constexpr int MOVE_MOUNT_T_SYMLINKS = 0x00000010; ///< Follow symlinks on to path
    inline constexpr int MOVE_MOUNT_T_AUTOMOUNTS = 0x00000020; ///< Follow automounts on to path
    inline constexpr int MOVE_MOUNT_T_EMPTY_PATH = 0x00000040; ///< Empty to path permitted
    inline constexpr int MOVE_MOUNT_SET_GROUP = 0x00000100; ///< Set sharing group instead
    inline constexpr int MOVE_MOUNT_BENEATH = 0x00000200; ///< Mount beneath top mount

    // fspick flags
    inline constexpr int FSPICK_CLOEXEC = 0x00000001;
    inline constexpr int FSPICK_SYMLINK_NOFOLLOW = 0x00000002;
    inline constexpr int FSPICK_NO_AUTOMOUNT = 0x00000004;
    inline constexpr int FSPICK_EMPTY_PATH = 0x00000008;

    // fsconfig commands
    inline constexpr int FSCONFIG_SET_FLAG = 0; ///< Set parameter, supplying no value
    inline constexpr int FSCONFIG_SET_STRING = 1; ///< Set parameter, supplying a string value
    inline constexpr int FSCONFIG_SET_BINARY = 2; ///< Set parameter, supplying a binary blob value
    inline constexpr int FSCONFIG_SET_PATH = 3; ///< Set parameter, supplying an object by path
    inline constexpr int FSCONFIG_SET_PATH_EMPTY = 4; ///< Set parameter, supplying an object by (empty) path
    inline constexpr int FSCONFIG_SET_FD = 5; ///< Set parameter, supplying an object by fd
    inline constexpr int FSCONFIG_CMD_CREATE = 6; ///< Invoke superblock creation
    inline constexpr int FSCONFIG_CMD_RECONFIGURE = 7; ///< Invoke superblock reconfiguration
    inline constexpr int FSCONFIG_CMD_CREATE_EXCL = 8; ///< Create new superblock, fail if reusing existing superblock

    // fsopen flags
    inline constexpr int FSOPEN_CLOEXEC = 0x00000001;

    // open_tree flags
    inline constexpr int OPEN_TREE_CLONE = 1; ///< Clone the target tree and attach the clone
    inline constexpr int OPEN_TREE_CLOEXEC = 0x80000; ///< Close the file on execve() (O_CLOEXEC)
}
