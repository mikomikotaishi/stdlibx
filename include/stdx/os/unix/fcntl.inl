#pragma once

/**
 * @internal
 * @namespace stdx::os::unix::captured
 * @brief The numeric values of the <fcntl.h> macros, read where they exist.
 *
 * See stdx::os::unix::sys::captured for why the values are laundered through a
 * variable rather than copied.
 */
namespace stdx::os::unix::captured {
    #if (defined(__unix__) || defined(__APPLE__)) && __has_include(<fcntl.h>)
    constexpr auto F_DUPFD_VALUE = F_DUPFD;
    constexpr auto F_GETFD_VALUE = F_GETFD;
    constexpr auto F_SETFD_VALUE = F_SETFD;
    constexpr auto F_GETFL_VALUE = F_GETFL;
    constexpr auto F_SETFL_VALUE = F_SETFL;
    constexpr auto F_GETLK_VALUE = F_GETLK;
    constexpr auto F_SETLK_VALUE = F_SETLK;
    constexpr auto F_SETLKW_VALUE = F_SETLKW;
    constexpr auto F_SETOWN_VALUE = F_SETOWN;
    constexpr auto F_GETOWN_VALUE = F_GETOWN;
    constexpr auto F_DUPFD_CLOEXEC_VALUE = F_DUPFD_CLOEXEC;
    constexpr auto FD_CLOEXEC_VALUE = FD_CLOEXEC;
    constexpr auto F_RDLCK_VALUE = F_RDLCK;
    constexpr auto F_WRLCK_VALUE = F_WRLCK;
    constexpr auto F_UNLCK_VALUE = F_UNLCK;
    constexpr auto SEEK_SET_VALUE = SEEK_SET;
    constexpr auto SEEK_CUR_VALUE = SEEK_CUR;
    constexpr auto SEEK_END_VALUE = SEEK_END;
    constexpr auto O_ACCMODE_VALUE = O_ACCMODE;
    constexpr auto O_RDONLY_VALUE = O_RDONLY;
    constexpr auto O_WRONLY_VALUE = O_WRONLY;
    constexpr auto O_RDWR_VALUE = O_RDWR;
    constexpr auto O_CREAT_VALUE = O_CREAT;
    constexpr auto O_DIRECTORY_VALUE = O_DIRECTORY;
    constexpr auto O_EXCL_VALUE = O_EXCL;
    constexpr auto O_NOCTTY_VALUE = O_NOCTTY;
    constexpr auto O_NOFOLLOW_VALUE = O_NOFOLLOW;
    constexpr auto O_TRUNC_VALUE = O_TRUNC;
    constexpr auto O_APPEND_VALUE = O_APPEND;
    constexpr auto O_DSYNC_VALUE = O_DSYNC;
    constexpr auto O_NONBLOCK_VALUE = O_NONBLOCK;
    constexpr auto O_NDELAY_VALUE = O_NDELAY;
    constexpr auto O_SYNC_VALUE = O_SYNC;
    constexpr auto O_FSYNC_VALUE = O_FSYNC;
    constexpr auto AT_FDCWD_VALUE = AT_FDCWD;
    constexpr auto AT_EACCESS_VALUE = AT_EACCESS;
    constexpr auto AT_SYMLINK_NOFOLLOW_VALUE = AT_SYMLINK_NOFOLLOW;
    constexpr auto AT_SYMLINK_FOLLOW_VALUE = AT_SYMLINK_FOLLOW;
    constexpr auto AT_REMOVEDIR_VALUE = AT_REMOVEDIR;

    /**
     * @brief POSIX's execute-only and search-only open modes.
     *
     * Darwin implements them; glibc does not define either macro, which is why
     * unix.constants.inl has always fallen back to O_RDONLY there. That fallback
     * was previously indistinguishable from a real value.
     */
    #ifdef __APPLE__
    constexpr auto O_EXEC_VALUE = O_EXEC;
    constexpr auto O_SEARCH_VALUE = O_SEARCH;
    #endif

    /// Linux and glibc additions; Darwin defines none of these.
    #ifdef __linux__
    constexpr auto F_SETSIG_VALUE = F_SETSIG;
    constexpr auto F_GETSIG_VALUE = F_GETSIG;
    constexpr auto F_GETLK64_VALUE = F_GETLK64;
    constexpr auto F_SETLK64_VALUE = F_SETLK64;
    constexpr auto F_SETLKW64_VALUE = F_SETLKW64;
    constexpr auto F_SETOWN_EX_VALUE = F_SETOWN_EX;
    constexpr auto F_GETOWN_EX_VALUE = F_GETOWN_EX;
    constexpr auto O_RSYNC_VALUE = O_RSYNC;
    constexpr auto POSIX_FADV_NORMAL_VALUE = POSIX_FADV_NORMAL;
    constexpr auto POSIX_FADV_RANDOM_VALUE = POSIX_FADV_RANDOM;
    constexpr auto POSIX_FADV_SEQUENTIAL_VALUE = POSIX_FADV_SEQUENTIAL;
    constexpr auto POSIX_FADV_WILLNEED_VALUE = POSIX_FADV_WILLNEED;
    constexpr auto POSIX_FADV_DONTNEED_VALUE = POSIX_FADV_DONTNEED;
    constexpr auto POSIX_FADV_NOREUSE_VALUE = POSIX_FADV_NOREUSE;
    #endif
    #endif
}

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX file-control operations.
 */
export namespace stdx::os::unix {
    #if (defined(__unix__) || defined(__APPLE__)) && __has_include(<fcntl.h>)
    using ::open;
    using ::openat;
    using ::creat;
    using ::fcntl;

    /**
     * @brief glibc's large-file and advisory calls.
     *
     * The *64 forms are glibc's transitional large-file interface, which Darwin
     * never needed - its off_t has always been 64-bit. posix_fadvise and
     * posix_fallocate are POSIX but unimplemented there; the nearest equivalent
     * is fcntl with F_PREALLOCATE.
     */
    #ifdef __linux__
    using ::open64;
    using ::openat64;
    using ::creat64;
    using ::posix_fadvise;
    using ::posix_fadvise64;
    using ::posix_fallocate;
    using ::posix_fallocate64;
    #endif
    #endif
}
