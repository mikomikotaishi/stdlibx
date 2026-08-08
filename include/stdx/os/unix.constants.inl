#pragma once

/**
 * @file unix.constants.inl
 * @brief Numeric values of the POSIX macros, spelled out so they cross a module boundary.
 *
 * @warning Every value in this file is the **Linux** one. The names are POSIX
 * but the numbers are not portable, and there are no platform guards: Darwin
 * and the BSDs disagree on almost all of them - SOL_SOCKET is 0xffff rather
 * than 1, SO_REUSEADDR is 4 rather than 2, AF_INET6 is 30 rather than 10,
 * EAGAIN is 35 rather than 11, O_NONBLOCK is 0x0004 rather than 0o4000. Code
 * built against these on such a host would not fail to compile; it would call
 * the kernel with the wrong option numbers.
 *
 * A macro does not cross `import :os`, and this file sits on the stdx:main side
 * where the system headers are not visible, so a value can only be written as a
 * literal here or read from a `constexpr` captured on the stdx:os side - the
 * stdx::os::unix::captured namespace, whose files (unix/errno.inl,
 * unix/sys/socket.inl, ...) do include the real headers and so are right on every
 * platform by construction.
 *
 * That migration is partly done: as of 2026-08-10, 279 of the 747 constants here
 * read `captured::`, and the remaining 468 are still Linux literals. The warning
 * above applies to those 468 - O_CLOEXEC is 02000000 here and 0x01000000 on
 * Darwin, and nothing catches the difference at build time.
 */

/**
 * @namespace stdx::os::unix
 * @brief Unix operations.
 */
export namespace stdx::os::unix {
    #if defined(__unix__) || defined(__APPLE__)
    /**
     * <errno.h> constants
     */

    inline constexpr i32 EPERM = captured::EPERM_VALUE; ///< Operation not permitted
    inline constexpr i32 ENOENT = captured::ENOENT_VALUE; ///< No such file or directory
    inline constexpr i32 ESRCH = captured::ESRCH_VALUE; ///< No such process
    inline constexpr i32 EINTR = captured::EINTR_VALUE; ///< Interrupted system call
    inline constexpr i32 EIO = captured::EIO_VALUE; ///< Input/output error
    inline constexpr i32 ENXIO = captured::ENXIO_VALUE; ///< No such device or address
    inline constexpr i32 E2BIG = captured::E2BIG_VALUE; ///< Argument list too long
    inline constexpr i32 ENOEXEC = captured::ENOEXEC_VALUE; ///< Exec format error
    inline constexpr i32 EBADF = captured::EBADF_VALUE; ///< Bad file descriptor
    inline constexpr i32 ECHILD = captured::ECHILD_VALUE; ///< No child processes
    inline constexpr i32 EAGAIN = captured::EAGAIN_VALUE; ///< Resource temporarily unavailable
    inline constexpr i32 EWOULDBLOCK = captured::EWOULDBLOCK_VALUE; ///< Operation would block (the same value as EAGAIN on Linux)
    inline constexpr i32 ENOMEM = captured::ENOMEM_VALUE; ///< Cannot allocate memory
    inline constexpr i32 EACCES = captured::EACCES_VALUE; ///< Permission denied
    inline constexpr i32 EFAULT = captured::EFAULT_VALUE; ///< Bad address
    inline constexpr i32 ENOTBLK = captured::ENOTBLK_VALUE; ///< Block device required
    inline constexpr i32 EBUSY = captured::EBUSY_VALUE; ///< Device or resource busy
    inline constexpr i32 EEXIST = captured::EEXIST_VALUE; ///< File exists
    inline constexpr i32 EXDEV = captured::EXDEV_VALUE; ///< Invalid cross-device link
    inline constexpr i32 ENODEV = captured::ENODEV_VALUE; ///< No such device
    inline constexpr i32 ENOTDIR = captured::ENOTDIR_VALUE; ///< Not a directory
    inline constexpr i32 EISDIR = captured::EISDIR_VALUE; ///< Is a directory
    inline constexpr i32 EINVAL = captured::EINVAL_VALUE; ///< Invalid argument
    inline constexpr i32 ENFILE = captured::ENFILE_VALUE; ///< Too many open files in system
    inline constexpr i32 EMFILE = captured::EMFILE_VALUE; ///< Too many open files
    inline constexpr i32 ENOTTY = captured::ENOTTY_VALUE; ///< Inappropriate ioctl for device
    inline constexpr i32 ETXTBSY = captured::ETXTBSY_VALUE; ///< Text file busy
    inline constexpr i32 EFBIG = captured::EFBIG_VALUE; ///< File too large
    inline constexpr i32 ENOSPC = captured::ENOSPC_VALUE; ///< No space left on device
    inline constexpr i32 ESPIPE = captured::ESPIPE_VALUE; ///< Illegal seek
    inline constexpr i32 EROFS = captured::EROFS_VALUE; ///< Read-only file system
    inline constexpr i32 EMLINK = captured::EMLINK_VALUE; ///< Too many links
    inline constexpr i32 EPIPE = captured::EPIPE_VALUE; ///< Broken pipe
    inline constexpr i32 EDOM = captured::EDOM_VALUE; ///< Numerical argument out of domain
    inline constexpr i32 ERANGE = captured::ERANGE_VALUE; ///< Numerical result out of range
    inline constexpr i32 EDEADLK = captured::EDEADLK_VALUE; ///< Resource deadlock avoided
    inline constexpr i32 ENAMETOOLONG = captured::ENAMETOOLONG_VALUE; ///< File name too long
    inline constexpr i32 ENOLCK = captured::ENOLCK_VALUE; ///< No locks available
    inline constexpr i32 ENOSYS = captured::ENOSYS_VALUE; ///< Function not implemented
    inline constexpr i32 ENOTEMPTY = captured::ENOTEMPTY_VALUE; ///< Directory not empty
    inline constexpr i32 ELOOP = captured::ELOOP_VALUE; ///< Too many levels of symbolic links
    inline constexpr i32 ENOMSG = captured::ENOMSG_VALUE; ///< No message of desired type
    inline constexpr i32 EIDRM = captured::EIDRM_VALUE; ///< Identifier removed
    inline constexpr i32 ENOSTR = captured::ENOSTR_VALUE; ///< Device not a stream
    inline constexpr i32 ENODATA = captured::ENODATA_VALUE; ///< No data available
    inline constexpr i32 ETIME = captured::ETIME_VALUE; ///< Timer expired
    inline constexpr i32 ENOSR = captured::ENOSR_VALUE; ///< Out of streams resources
    inline constexpr i32 EREMOTE = captured::EREMOTE_VALUE; ///< Object is remote
    inline constexpr i32 ENOLINK = captured::ENOLINK_VALUE; ///< Link has been severed
    inline constexpr i32 EPROTO = captured::EPROTO_VALUE; ///< Protocol error
    inline constexpr i32 EMULTIHOP = captured::EMULTIHOP_VALUE; ///< Multihop attempted
    inline constexpr i32 EBADMSG = captured::EBADMSG_VALUE; ///< Bad message
    inline constexpr i32 EOVERFLOW = captured::EOVERFLOW_VALUE; ///< Value too large for defined data type
    inline constexpr i32 EILSEQ = captured::EILSEQ_VALUE; ///< Invalid or incomplete multibyte or wide character
    inline constexpr i32 EUSERS = captured::EUSERS_VALUE; ///< Too many users
    inline constexpr i32 ENOTSOCK = captured::ENOTSOCK_VALUE; ///< Socket operation on non-socket
    inline constexpr i32 EDESTADDRREQ = captured::EDESTADDRREQ_VALUE; ///< Destination address required
    inline constexpr i32 EMSGSIZE = captured::EMSGSIZE_VALUE; ///< Message too long
    inline constexpr i32 EPROTOTYPE = captured::EPROTOTYPE_VALUE; ///< Protocol wrong type for socket
    inline constexpr i32 ENOPROTOOPT = captured::ENOPROTOOPT_VALUE; ///< Protocol not available
    inline constexpr i32 EPROTONOSUPPORT = captured::EPROTONOSUPPORT_VALUE; ///< Protocol not supported
    inline constexpr i32 ESOCKTNOSUPPORT = captured::ESOCKTNOSUPPORT_VALUE; ///< Socket type not supported
    inline constexpr i32 EOPNOTSUPP = captured::EOPNOTSUPP_VALUE; ///< Operation not supported
    inline constexpr i32 EPFNOSUPPORT = captured::EPFNOSUPPORT_VALUE; ///< Protocol family not supported
    inline constexpr i32 EAFNOSUPPORT = captured::EAFNOSUPPORT_VALUE; ///< Address family not supported by protocol
    inline constexpr i32 EADDRINUSE = captured::EADDRINUSE_VALUE; ///< Address already in use
    inline constexpr i32 EADDRNOTAVAIL = captured::EADDRNOTAVAIL_VALUE; ///< Cannot assign requested address
    inline constexpr i32 ENETDOWN = captured::ENETDOWN_VALUE; ///< Network is down
    inline constexpr i32 ENETUNREACH = captured::ENETUNREACH_VALUE; ///< Network is unreachable
    inline constexpr i32 ENETRESET = captured::ENETRESET_VALUE; ///< Network dropped connection on reset
    inline constexpr i32 ECONNABORTED = captured::ECONNABORTED_VALUE; ///< Software caused connection abort
    inline constexpr i32 ECONNRESET = captured::ECONNRESET_VALUE; ///< Connection reset by peer
    inline constexpr i32 ENOBUFS = captured::ENOBUFS_VALUE; ///< No buffer space available
    inline constexpr i32 EISCONN = captured::EISCONN_VALUE; ///< Transport endpoint is already connected
    inline constexpr i32 ENOTCONN = captured::ENOTCONN_VALUE; ///< Transport endpoint is not connected
    inline constexpr i32 ESHUTDOWN = captured::ESHUTDOWN_VALUE; ///< Cannot send after transport endpoint shutdown
    inline constexpr i32 ETOOMANYREFS = captured::ETOOMANYREFS_VALUE; ///< Too many references: cannot splice
    inline constexpr i32 ETIMEDOUT = captured::ETIMEDOUT_VALUE; ///< Connection timed out
    inline constexpr i32 ECONNREFUSED = captured::ECONNREFUSED_VALUE; ///< Connection refused
    inline constexpr i32 EHOSTDOWN = captured::EHOSTDOWN_VALUE; ///< Host is down
    inline constexpr i32 EHOSTUNREACH = captured::EHOSTUNREACH_VALUE; ///< No route to host
    inline constexpr i32 EALREADY = captured::EALREADY_VALUE; ///< Operation already in progress
    inline constexpr i32 EINPROGRESS = captured::EINPROGRESS_VALUE; ///< Operation now in progress
    inline constexpr i32 ESTALE = captured::ESTALE_VALUE; ///< Stale file handle
    inline constexpr i32 EDQUOT = captured::EDQUOT_VALUE; ///< Disk quota exceeded
    inline constexpr i32 ECANCELED = captured::ECANCELED_VALUE; ///< Operation canceled
    inline constexpr i32 EOWNERDEAD = captured::EOWNERDEAD_VALUE; ///< Owner died
    inline constexpr i32 ENOTRECOVERABLE = captured::ENOTRECOVERABLE_VALUE; ///< State not recoverable
    inline constexpr i32 ENOTSUP = captured::ENOTSUP_VALUE; ///< Not supported parameter or option

    /**
     * @brief Linux's own additions to errno.
     *
     * Guarded to match stdx::os::unix::captured, where the values are read.
     * Darwin defines none of these; it has its own set instead (EBADMACHO,
     * ESHLIBVERS, EAUTH and friends), which would belong in darwin.constants.inl
     * if anything needed them.
     */
    #ifdef __linux__
    inline constexpr i32 ECHRNG = captured::ECHRNG_VALUE; ///< Channel number out of range
    inline constexpr i32 EL2NSYNC = captured::EL2NSYNC_VALUE; ///< Level 2 not synchronized
    inline constexpr i32 EL3HLT = captured::EL3HLT_VALUE; ///< Level 3 halted
    inline constexpr i32 EL3RST = captured::EL3RST_VALUE; ///< Level 3 reset
    inline constexpr i32 ELNRNG = captured::ELNRNG_VALUE; ///< Link number out of range
    inline constexpr i32 EUNATCH = captured::EUNATCH_VALUE; ///< Protocol driver not attached
    inline constexpr i32 ENOCSI = captured::ENOCSI_VALUE; ///< No CSI structure available
    inline constexpr i32 EL2HLT = captured::EL2HLT_VALUE; ///< Level 2 halted
    inline constexpr i32 EBADE = captured::EBADE_VALUE; ///< Invalid exchange
    inline constexpr i32 EBADR = captured::EBADR_VALUE; ///< Invalid request descriptor
    inline constexpr i32 EXFULL = captured::EXFULL_VALUE; ///< Exchange full
    inline constexpr i32 ENOANO = captured::ENOANO_VALUE; ///< No anode
    inline constexpr i32 EBADRQC = captured::EBADRQC_VALUE; ///< Invalid request code
    inline constexpr i32 EBADSLT = captured::EBADSLT_VALUE; ///< Invalid slot
    inline constexpr i32 EBFONT = captured::EBFONT_VALUE; ///< Bad font file format
    inline constexpr i32 ENONET = captured::ENONET_VALUE; ///< Machine is not on the network
    inline constexpr i32 ENOPKG = captured::ENOPKG_VALUE; ///< Package not installed
    inline constexpr i32 EADV = captured::EADV_VALUE; ///< Advertise error
    inline constexpr i32 ESRMNT = captured::ESRMNT_VALUE; ///< Srmount error
    inline constexpr i32 ECOMM = captured::ECOMM_VALUE; ///< Communication error on send
    inline constexpr i32 EDOTDOT = captured::EDOTDOT_VALUE; ///< RFS specific error
    inline constexpr i32 ENOTUNIQ = captured::ENOTUNIQ_VALUE; ///< Name not unique on network
    inline constexpr i32 EBADFD = captured::EBADFD_VALUE; ///< File descriptor in bad state
    inline constexpr i32 EREMCHG = captured::EREMCHG_VALUE; ///< Remote address changed
    inline constexpr i32 ELIBACC = captured::ELIBACC_VALUE; ///< Can not access a needed shared library
    inline constexpr i32 ELIBBAD = captured::ELIBBAD_VALUE; ///< Accessing a corrupted shared library
    inline constexpr i32 ELIBSCN = captured::ELIBSCN_VALUE; ///< .lib section in a.out corrupted
    inline constexpr i32 ELIBMAX = captured::ELIBMAX_VALUE; ///< Attempting to link in too many shared libraries
    inline constexpr i32 ELIBEXEC = captured::ELIBEXEC_VALUE; ///< Cannot exec a shared library directly
    inline constexpr i32 ERESTART = captured::ERESTART_VALUE; ///< Interrupted system call should be restarted
    inline constexpr i32 ESTRPIPE = captured::ESTRPIPE_VALUE; ///< Streams pipe error
    inline constexpr i32 EUCLEAN = captured::EUCLEAN_VALUE; ///< Structure needs cleaning
    inline constexpr i32 ENOTNAM = captured::ENOTNAM_VALUE; ///< Not a Xenix named type file
    inline constexpr i32 ENAVAIL = captured::ENAVAIL_VALUE; ///< No Xenix semaphores available
    inline constexpr i32 EISNAM = captured::EISNAM_VALUE; ///< Is a named type file
    inline constexpr i32 EREMOTEIO = captured::EREMOTEIO_VALUE; ///< Remote I/O error
    inline constexpr i32 ENOMEDIUM = captured::ENOMEDIUM_VALUE; ///< No medium found
    inline constexpr i32 EMEDIUMTYPE = captured::EMEDIUMTYPE_VALUE; ///< Wrong medium type
    inline constexpr i32 ENOKEY = captured::ENOKEY_VALUE; ///< Required key not available
    inline constexpr i32 EKEYEXPIRED = captured::EKEYEXPIRED_VALUE; ///< Key has expired
    inline constexpr i32 EKEYREVOKED = captured::EKEYREVOKED_VALUE; ///< Key has been revoked
    inline constexpr i32 EKEYREJECTED = captured::EKEYREJECTED_VALUE; ///< Key was rejected by service
    inline constexpr i32 ERFKILL = captured::ERFKILL_VALUE; ///< Operation not possible due to RF-kill
    inline constexpr i32 EHWPOISON = captured::EHWPOISON_VALUE; ///< Memory page has hardware error
    #endif

    /**
        * <fcntl.h> constants
        */

    inline constexpr i32 F_DUPFD = captured::F_DUPFD_VALUE; ///< Duplicate file descriptor
    inline constexpr i32 F_GETFD = captured::F_GETFD_VALUE; ///< Get file descriptor flags
    inline constexpr i32 F_SETFD = captured::F_SETFD_VALUE; ///< Set file descriptor flags
    inline constexpr i32 F_GETFL = captured::F_GETFL_VALUE; ///< Get file status flags
    inline constexpr i32 F_SETFL = captured::F_SETFL_VALUE; ///< Set file status flags
    inline constexpr i32 F_GETLK = captured::F_GETLK_VALUE; ///< Get record locking information
    inline constexpr i32 F_SETLK = captured::F_SETLK_VALUE; ///< Set record locking information
    inline constexpr i32 F_SETLKW = captured::F_SETLKW_VALUE; ///< Set record locking information; wait if blocked
    inline constexpr i32 F_SETOWN = captured::F_SETOWN_VALUE; ///< Set owner (process receiving SIGIO)
    inline constexpr i32 F_GETOWN = captured::F_GETOWN_VALUE; ///< Get owner (process receiving
    inline constexpr i32 F_DUPFD_CLOEXEC = captured::F_DUPFD_CLOEXEC_VALUE; ///< Duplicate descriptor with close-on-exec set

    inline constexpr i32 FD_CLOEXEC = captured::FD_CLOEXEC_VALUE; ///< Close descriptor on exec

    inline constexpr i32 F_RDLCK = captured::F_RDLCK_VALUE; ///< Shared/read lock
    inline constexpr i32 F_WRLCK = captured::F_WRLCK_VALUE; ///< Exclusive/write lock
    inline constexpr i32 F_UNLCK = captured::F_UNLCK_VALUE; ///< Unlock

    inline constexpr i32 SEEK_SET = captured::SEEK_SET_VALUE; ///< Seek from beginning
    inline constexpr i32 SEEK_CUR = captured::SEEK_CUR_VALUE; ///< Seek from current position
    inline constexpr i32 SEEK_END = captured::SEEK_END_VALUE; ///< Seek from end

    inline constexpr i32 O_ACCMODE = captured::O_ACCMODE_VALUE;
    inline constexpr i32 O_RDONLY = captured::O_RDONLY_VALUE;
    inline constexpr i32 O_WRONLY = captured::O_WRONLY_VALUE;
    inline constexpr i32 O_RDWR = captured::O_RDWR_VALUE;
    inline constexpr i32 O_CREAT = captured::O_CREAT_VALUE;
    inline constexpr i32 O_DIRECTORY = captured::O_DIRECTORY_VALUE;
    inline constexpr i32 O_EXCL = captured::O_EXCL_VALUE;
    inline constexpr i32 O_NOCTTY = captured::O_NOCTTY_VALUE;
    inline constexpr i32 O_NOFOLLOW = captured::O_NOFOLLOW_VALUE;
    inline constexpr i32 O_TRUNC = captured::O_TRUNC_VALUE;
    inline constexpr i32 O_APPEND = captured::O_APPEND_VALUE;
    inline constexpr i32 O_DSYNC = captured::O_DSYNC_VALUE;
    inline constexpr i32 O_NONBLOCK = captured::O_NONBLOCK_VALUE;
    inline constexpr i32 O_NDELAY = captured::O_NDELAY_VALUE;
    inline constexpr i32 O_SYNC = captured::O_SYNC_VALUE;
    inline constexpr i32 O_FSYNC = captured::O_FSYNC_VALUE;
    /**
     * @brief Open for execute only, and open a directory for search only.
     *
     * Darwin implements both; glibc defines neither macro, so on Linux they fall
     * back to O_RDONLY - which is what this file has always done, though as a bare
     * literal it read as a real value rather than as a substitute. POSIX allows
     * the fallback: a plain read-only open is permitted to serve where the mode is
     * unsupported.
     */
    #ifdef __APPLE__
    inline constexpr i32 O_EXEC = captured::O_EXEC_VALUE;
    inline constexpr i32 O_SEARCH = captured::O_SEARCH_VALUE;
    #else
    inline constexpr i32 O_EXEC = O_RDONLY;
    inline constexpr i32 O_SEARCH = O_RDONLY;
    #endif
    inline constexpr i32 O_CLOEXEC  = 02000000;

    inline constexpr i32 AT_FDCWD = captured::AT_FDCWD_VALUE;
    inline constexpr i32 AT_EACCESS = captured::AT_EACCESS_VALUE;
    inline constexpr i32 AT_SYMLINK_NOFOLLOW = captured::AT_SYMLINK_NOFOLLOW_VALUE;
    inline constexpr i32 AT_SYMLINK_FOLLOW = captured::AT_SYMLINK_FOLLOW_VALUE;
    inline constexpr i32 AT_REMOVEDIR = captured::AT_REMOVEDIR_VALUE;

    /// glibc and Linux additions; Darwin defines none of these.
    #ifdef __linux__
    inline constexpr i32 F_SETSIG = captured::F_SETSIG_VALUE; ///< Set number of signal to be sent on SIGIO
    inline constexpr i32 F_GETSIG = captured::F_GETSIG_VALUE; ///< Get number of signal to be sent on SIGIO
    inline constexpr i32 F_GETLK64 = captured::F_GETLK64_VALUE; ///< Get record locking information (using 64-bit file sizes)
    inline constexpr i32 F_SETLK64 = captured::F_SETLK64_VALUE; ///< Set record locking information (using 64-bit file sizes)
    inline constexpr i32 F_SETLKW64 = captured::F_SETLKW64_VALUE; ///< Set record locking information; wait if blocked (using 64-bit file sizes)
    inline constexpr i32 F_SETOWN_EX = captured::F_SETOWN_EX_VALUE; ///< Set owner, using pidfd (instead of process id)
    inline constexpr i32 F_GETOWN_EX = captured::F_GETOWN_EX_VALUE; ///< Get owner, using pidfd (instead of process id)
    /**
     * @brief Leave a newly opened terminal in a standard state.
     *
     * The one constant in this file that cannot be derived, because neither Linux
     * nor Darwin defines the macro. POSIX permits it to be zero where opening a
     * terminal already yields a usable state, which is the case on both, so zero
     * is the correct value rather than a placeholder.
     */
    inline constexpr i32 O_TTY_INIT = 0;
    inline constexpr i32 O_RSYNC = captured::O_RSYNC_VALUE;
    inline constexpr i32 POSIX_FADV_NORMAL = captured::POSIX_FADV_NORMAL_VALUE;
    inline constexpr i32 POSIX_FADV_RANDOM = captured::POSIX_FADV_RANDOM_VALUE;
    inline constexpr i32 POSIX_FADV_SEQUENTIAL = captured::POSIX_FADV_SEQUENTIAL_VALUE;
    inline constexpr i32 POSIX_FADV_WILLNEED = captured::POSIX_FADV_WILLNEED_VALUE;
    inline constexpr i32 POSIX_FADV_DONTNEED = captured::POSIX_FADV_DONTNEED_VALUE;
    inline constexpr i32 POSIX_FADV_NOREUSE = captured::POSIX_FADV_NOREUSE_VALUE;
    #endif


    /**
     * <glob.h> constants
     */

    inline constexpr i32 GLOB_ERR = 1 << 0; ///< Return on read errors.
    inline constexpr i32 GLOB_MARK = 1 << 1; ///< Append a slash to each name.
    inline constexpr i32 GLOB_NOSORT = 1 << 2; ///< Don't sort the names.
    inline constexpr i32 GLOB_DOOFFS = 1 << 3; ///< Insert PGLOB->gl_offs NULLs.
    inline constexpr i32 GLOB_NOCHECK = 1 << 4; ///< If nothing matches, return the pattern.
    inline constexpr i32 GLOB_APPEND = 1 << 5; ///< Append to results of a previous call.
    inline constexpr i32 GLOB_NOESCAPE = 1 << 6; ///< Backslashes don't quote metacharacters.
    inline constexpr i32 GLOB_PERIOD = 1 << 7; ///< Leading `.` can be matched by metachars.

    inline constexpr i32 GLOB_MAGCHAR = 1 << 8; ///< Set in gl_flags if any metachars seen.
    inline constexpr i32 GLOB_ALTDIRFUNC = 1 << 9; ///< Use gl_opendir et al functions.
    inline constexpr i32 GLOB_BRACE = 1 << 10; ///< Expand "{a,b}" to "a" "b".
    inline constexpr i32 GLOB_NOMAGIC = 1 << 11; ///< If no magic chars, return the pattern.
    inline constexpr i32 GLOB_TILDE = 1 << 12; ///< Expand ~user and ~ to home directories.
    inline constexpr i32 GLOB_ONLYDIR = 1 << 13; ///< Match only directories.
    inline constexpr i32 GLOB_TILDE_CHECK = 1 << 14; ///< Like GLOB_TILDE but return an error if user name is unavailable.

    inline constexpr i32 __GLOB_FLAGS =
        GLOB_ERR | GLOB_MARK | GLOB_NOSORT | GLOB_DOOFFS |
        GLOB_NOESCAPE | GLOB_NOCHECK | GLOB_APPEND | GLOB_PERIOD |
        GLOB_ALTDIRFUNC | GLOB_BRACE | GLOB_NOMAGIC |
        GLOB_TILDE | GLOB_ONLYDIR | GLOB_TILDE_CHECK;

    /// Error returns from glob
    inline constexpr i32 GLOB_NOSPACE = 1; ///< Ran out of memory.
    inline constexpr i32 GLOB_ABORTED = 2; ///< Read error.
    inline constexpr i32 GLOB_NOMATCH = 3; ///< No matches found.
    inline constexpr i32 GLOB_NOSYS = 4; ///< Not implemented.

    /**
     * <poll.h> constants
     */

    inline constexpr i32 POLLIN = 0x001; ///< There is data to read.
    inline constexpr i32 POLLPRI = 0x002; ///< There is urgent data to read.
    inline constexpr i32 POLLOUT = 0x004; ///< Writing now will not block.

    inline constexpr i32 POLLRDNORM = 0x040; ///< Normal data may be read.
    inline constexpr i32 POLLRDBAND = 0x080; ///< Priority data may be read.
    inline constexpr i32 POLLWRNORM = 0x100; ///< Writing now will not block.
    inline constexpr i32 POLLWRBAND = 0x200; ///< Priority data may be written.

    inline constexpr i32 POLLMSG = 0x400; ///< Streams message available
    inline constexpr i32 POLLREMOVE = 0x1000; ///< Streams data tosociated
    inline constexpr i32 POLLRDHUP = 0x2000; ///< Stream socket peer closed connection, or shut down writing half of connection

    inline constexpr i32 POLLERR = 0x008; ///< Error condition.
    inline constexpr i32 POLLHUP = 0x010; ///< Hung up.
    inline constexpr i32 POLLNVAL = 0x020; ///< Invalid polling request.

    /**
     * <termios.h> constants
     */

    // Baud rates (c_cflag bit meanings)
    inline constexpr u32 B0 = 0000000; ///< Hang up
    inline constexpr u32 B50 = 0000001;
    inline constexpr u32 B75 = 0000002;
    inline constexpr u32 B110 = 0000003;
    inline constexpr u32 B134 = 0000004;
    inline constexpr u32 B150 = 0000005;
    inline constexpr u32 B200 = 0000006;
    inline constexpr u32 B300 = 0000007;
    inline constexpr u32 B600 = 0000010;
    inline constexpr u32 B1200 = 0000011;
    inline constexpr u32 B1800 = 0000012;
    inline constexpr u32 B2400 = 0000013;
    inline constexpr u32 B4800 = 0000014;
    inline constexpr u32 B9600 = 0000015;
    inline constexpr u32 B19200 = 0000016;
    inline constexpr u32 B38400 = 0000017;

    // Aliases (used with __USE_MISC)
    inline constexpr u32 EXTA = B19200;
    inline constexpr u32 EXTB = B38400;

    // TIOCSERGETLSR result bits (used with ioctl)
    inline constexpr u32 TIOCSER_TEMT = 0x01; ///< Transmitter physically empty

    // tcflow() / TCXONC options
    inline constexpr i32 TCOOFF = 0;
    inline constexpr i32 TCOON = 1;
    inline constexpr i32 TCIOFF = 2;
    inline constexpr i32 TCION = 3;

    // tcflush() / TCFLSH options
    inline constexpr i32 TCIFLUSH = 0;
    inline constexpr i32 TCOFLUSH = 1;
    inline constexpr i32 TCIOFLUSH = 2;

    inline constexpr i32 VINTR = 0;
    inline constexpr i32 VQUIT = 1;
    inline constexpr i32 VERASE = 2;
    inline constexpr i32 VKILL = 3;
    inline constexpr i32 VEOF = 4;
    inline constexpr i32 VTIME = 5;
    inline constexpr i32 VMIN = 6;
    inline constexpr i32 VSWTC = 7;
    inline constexpr i32 VSTART = 8;
    inline constexpr i32 VSTOP = 9;
    inline constexpr i32 VSUSP = 10;
    inline constexpr i32 VEOL = 11;
    inline constexpr i32 VREPRINT = 12;
    inline constexpr i32 VDISCARD = 13;
    inline constexpr i32 VWERASE = 14;
    inline constexpr i32 VLNEXT = 15;
    inline constexpr i32 VEOL2 = 16;

    inline constexpr u32 IGNBRK = 0000001;  ///< Ignore break condition
    inline constexpr u32 BRKINT = 0000002;  ///< Signal i32errupt on break
    inline constexpr u32 IGNPAR = 0000004;  ///< Ignore characters with parity errors
    inline constexpr u32 PARMRK = 0000010;  ///< Mark parity and framing errors
    inline constexpr u32 INPCK = 0000020;  ///< Enable input parity check
    inline constexpr u32 ISTRIP = 0000040;  ///< Strip 8th bit off characters
    inline constexpr u32 INLCR = 0000100;  ///< Map NL to CR on input
    inline constexpr u32 IGNCR = 0000200;  ///< Ignore CR
    inline constexpr u32 ICRNL = 0000400;  ///< Map CR to NL on input
    inline constexpr u32 IUCLC = 0001000;  ///< Map uppercase to lowercase (non-POSIX)
    inline constexpr u32 IXON = 0002000;  ///< Enable start/stop output control
    inline constexpr u32 IXANY = 0004000;  ///< Any char restarts output
    inline constexpr u32 IXOFF = 0010000;  ///< Enable start/stop input control
    inline constexpr u32 IMAXBEL = 0020000;  ///< Bell on input queue full (non-POSIX)
    inline constexpr u32 IUTF8 = 0040000;  ///< Input is UTF-8 (non-POSIX)

    inline constexpr u32 OPOST = 0000001; ///< Post-process output
    inline constexpr u32 OLCUC = 0000002; ///< Map lowercase to uppercase (non-POSIX)
    inline constexpr u32 ONLCR = 0000004; ///< Map NL to CR-NL on output
    inline constexpr u32 OCRNL = 0000010; ///< Map CR to NL on output
    inline constexpr u32 ONOCR = 0000020; ///< No CR at column 0
    inline constexpr u32 ONLRET = 0000040; ///< NL performs CR function
    inline constexpr u32 OFILL = 0000100; ///< Use fill characters for delay
    inline constexpr u32 OFDEL = 0000200; ///< Fill is DEL

    // Newline delays
    inline constexpr u32 NLDLY = 0000400;
    inline constexpr u32 NL0 = 0000000;
    inline constexpr u32 NL1 = 0000400;

    // Carriage-return delays
    inline constexpr u32 CRDLY = 0003000;
    inline constexpr u32 CR0 = 0000000;
    inline constexpr u32 CR1 = 0001000;
    inline constexpr u32 CR2 = 0002000;
    inline constexpr u32 CR3 = 0003000;

    // Horizontal-tab delays
    inline constexpr u32 TABDLY = 0014000;
    inline constexpr u32 TAB0 = 0000000;
    inline constexpr u32 TAB1 = 0004000;
    inline constexpr u32 TAB2 = 0010000;
    inline constexpr u32 TAB3 = 0014000; ///< Expand tabs to spaces

    // Backspace delays
    inline constexpr u32 BSDLY = 0020000;
    inline constexpr u32 BS0 = 0000000;
    inline constexpr u32 BS1 = 0020000;

    // Vertical-tab delays
    inline constexpr u32 VTDLY = 0040000;
    inline constexpr u32 VT0 = 0000000;
    inline constexpr u32 VT1 = 0040000;

    // Form-feed delays
    inline constexpr u32 FFDLY = 0100000;
    inline constexpr u32 FF0 = 0000000;
    inline constexpr u32 FF1 = 0100000;

    // Non-POSIX alias
    inline constexpr u32 XTABS = 0014000;

    inline constexpr u32 CBAUD = 000000010017; ///< Baud speed mask (non-POSIX)
    inline constexpr u32 CBAUDEX = 000000010000; ///< Extra baud speed mask, part of CBAUD (non-POSIX)
    inline constexpr u32 CIBAUD = 002003600000; ///< Input baud rate mask (unused, non-POSIX)
    inline constexpr u32 CMSPAR = 010000000000; ///< Mark or space (stick) parity (non-POSIX)
    inline constexpr u32 CRTSCTS = 020000000000; ///< RTS/CTS flow control (non-POSIX)

    inline constexpr u32 B57600 = 0010001; ///< Baud rate: 57600
    inline constexpr u32 B115200 = 0010002; ///< Baud rate: 115200
    inline constexpr u32 B230400 = 0010003; ///< Baud rate: 230400
    inline constexpr u32 B460800 = 0010004; ///< Baud rate: 460800
    inline constexpr u32 B500000 = 0010005; ///< Baud rate: 500000
    inline constexpr u32 B576000 = 0010006; ///< Baud rate: 576000
    inline constexpr u32 B921600 = 0010007; ///< Baud rate: 921600
    inline constexpr u32 B1000000 = 0010010; ///< Baud rate: 1000000
    inline constexpr u32 B1152000 = 0010011; ///< Baud rate: 1152000
    inline constexpr u32 B1500000 = 0010012; ///< Baud rate: 1500000
    inline constexpr u32 B2000000 = 0010013; ///< Baud rate: 2000000
    inline constexpr u32 B2500000 = 0010014; ///< Baud rate: 2500000
    inline constexpr u32 B3000000 = 0010015; ///< Baud rate: 3000000
    inline constexpr u32 B3500000 = 0010016; ///< Baud rate: 3500000
    inline constexpr u32 B4000000 = 0010017; ///< Baud rate: 4000000

    inline constexpr u32 MAX_BAUD = B4000000; ///< Maximum supported baud rate

    inline constexpr u32 CSIZE = 0000060; ///< Character size mask
    inline constexpr u32 CS5 = 0000000; ///< 5 bits per character
    inline constexpr u32 CS6 = 0000020; ///< 6 bits per character
    inline constexpr u32 CS7 = 0000040; ///< 7 bits per character
    inline constexpr u32 CS8 = 0000060; ///< 8 bits per character

    inline constexpr u32 CSTOPB = 0000100; ///< 2 stop bits
    inline constexpr u32 CREAD = 0000200; ///< Enable receiver
    inline constexpr u32 PARENB = 0000400; ///< Enable parity
    inline constexpr u32 PARODD = 0001000; ///< Odd parity
    inline constexpr u32 HUPCL = 0002000; ///< Hang up on last close
    inline constexpr u32 CLOCAL = 0004000; ///< Local line (ignore modem control)

    inline constexpr u32 ADDRB = 04000000000; ///< Address of device (non-POSIX)

    inline constexpr u32 ISIG = 0000001; ///< Enable signals
    inline constexpr u32 ICANON = 0000002; ///< Canonical input (erase and kill processing)
    inline constexpr u32 XCASE = 0000004; ///< Enable case mapping (non-POSIX)
    inline constexpr u32 ECHO = 0000010; ///< Enable echo
    inline constexpr u32 ECHOE = 0000020; ///< Echo erase character as error-correcting backspace
    inline constexpr u32 ECHOK = 0000040; ///< Echo KILL
    inline constexpr u32 ECHONL = 0000100; ///< Echo NL
    inline constexpr u32 NOFLSH = 0000200; ///< Disable flush after i32errupt or quit
    inline constexpr u32 TOSTOP = 0000400; ///< Send SIGTTOU for background output
    inline constexpr u32 ECHOCTL = 0001000; ///< If ECHO is also set, terminal special characters echoed as ^X
    inline constexpr u32 ECHOPRT = 0002000; ///< If ICANON and ECHO are set, characters pri32ed as they are erased
    inline constexpr u32 ECHOKE = 0004000; ///< If ICANON is set, KILL echoed by erasing each character on the line
    inline constexpr u32 FLUSHO = 0010000; ///< Output is being flushed (toggle by DISCARD character)
    inline constexpr u32 PENDIN = 0040000; ///< All characters in the input queue are repri32ed when next character is read
    inline constexpr u32 IEXTEN = 0100000; ///< Enable implementation-defined input processing
    inline constexpr u32 EXTPROC = 0200000; ///< Enable external processing (non-POSIX)

    inline constexpr u32 TCSANOW = 0;
    inline constexpr u32 TCSADRAIN = 1;
    inline constexpr u32 TCSAFLUSH = 2;

    /**
     * <time.h> constants
     */
    inline constexpr u32 CLOCK_REALTIME = 0; ///< Identifier for system-wide realtime clock.
    inline constexpr u32 CLOCK_MONOTONIC = 1; ///< Monotonic system-wide clock.
    inline constexpr u32 CLOCK_PROCESS_CPUTIME_ID = 2; ///< High-resolution timer from the CPU.
    inline constexpr u32 CLOCK_THREAD_CPUTIME_ID = 3; ///< Thread-specific CPU-time clock.
    inline constexpr u32 CLOCK_MONOTONIC_RAW = 4; ///< Monotonic system-wide clock, not adjusted for frequency scaling.
    inline constexpr u32 CLOCK_REALTIME_COARSE = 5; ///< Identifier for system-wide realtime clock, updated only on ticks.
    inline constexpr u32 CLOCK_MONOTONIC_COARSE = 6; ///< Monotonic system-wide clock, updated only on ticks.
    inline constexpr u32 CLOCK_BOOTTIME = 7; ///< Monotonic system-wide clock that includes time spent in suspension.
    inline constexpr u32 CLOCK_REALTIME_ALARM = 8; ///< Like CLOCK_REALTIME but also wakes suspended system.
    inline constexpr u32 CLOCK_BOOTTIME_ALARM = 9; ///< Like CLOCK_BOOTTIME but also wakes suspended system.
    inline constexpr u32 CLOCK_TAI = 11; ///< Like CLOCK_REALTIME but in International Atomic Time.
    inline constexpr u32 TIMER_ABSTIME = 1; ///< Flag to indicate time is absolute.

    inline constexpr u32 ADJ_OFFSET = 0x0001; ///< time offset
    inline constexpr u32 ADJ_FREQUENCY = 0x0002; ///< frequency offset
    inline constexpr u32 ADJ_MAXERROR = 0x0004; ///< maximum time error
    inline constexpr u32 ADJ_ESTERROR = 0x0008; ///< estimated time error
    inline constexpr u32 ADJ_STATUS = 0x0010; ///< clock status
    inline constexpr u32 ADJ_TIMECONST = 0x0020; ///< pll time constant
    inline constexpr u32 ADJ_TAI = 0x0080; ///< set TAI offset
    inline constexpr u32 ADJ_SETOFFSET = 0x0100; ///< set 'time' to current time
    inline constexpr u32 ADJ_MICRO = 0x1000; ///< select microsecond resolution
    inline constexpr u32 ADJ_NANO = 0x2000; ///< select nanosecond resolution
    inline constexpr u32 ADJ_TICK = 0x4000; ///< tick value
    inline constexpr u32 ADJ_OFFSET_SINGLESHOT = 0x8000; ///< old-fashioned adjtime
    inline constexpr u32 ADJ_OFFSET_SS_READ = 0xa001; ///< read-only adjtime

    inline constexpr u32 MOD_OFFSET = ADJ_OFFSET;
    inline constexpr u32 MOD_FREQUENCY = ADJ_FREQUENCY;
    inline constexpr u32 MOD_MAXERROR = ADJ_MAXERROR;
    inline constexpr u32 MOD_ESTERROR = ADJ_ESTERROR;
    inline constexpr u32 MOD_STATUS = ADJ_STATUS;
    inline constexpr u32 MOD_TIMECONST = ADJ_TIMECONST;
    inline constexpr u32 MOD_CLKB = ADJ_TICK;
    inline constexpr u32 MOD_CLKA = ADJ_OFFSET_SINGLESHOT; ///< 0x8000 in original
    inline constexpr u32 MOD_TAI = ADJ_TAI;
    inline constexpr u32 MOD_MICRO = ADJ_MICRO;
    inline constexpr u32 MOD_NANO = ADJ_NANO;

    inline constexpr u32 STA_PLL = 0x0001; ///< enable PLL updates (rw)
    inline constexpr u32 STA_PPSFREQ = 0x0002; ///< enable PPS frequency discipline (rw)
    inline constexpr u32 STA_PPSTIME = 0x0004; ///< enable PPS time discipline (rw)
    inline constexpr u32 STA_FLL = 0x0008; ///< select frequency lock loop (rw)
    inline constexpr u32 STA_INS = 0x0010; ///< insert leap second (rw)
    inline constexpr u32 STA_DEL = 0x0020; ///< delete leap second (rw)
    inline constexpr u32 STA_UNSYNC = 0x0040; ///< clock unsynchronized (rw)
    inline constexpr u32 STA_FREQHOLD = 0x0080; ///< hold frequency (rw)
    inline constexpr u32 STA_PPSSIGNAL = 0x0100; ///< PPS signal present (ro)
    inline constexpr u32 STA_PPSJITTER = 0x0200; ///< PPS jitter exceeded (ro)
    inline constexpr u32 STA_PPSWANDER = 0x0400; ///< PPS wander exceeded (ro)
    inline constexpr u32 STA_PPSERROR = 0x0800; ///< PPS error exceeded (ro)
    inline constexpr u32 STA_CLOCKERR = 0x1000; ///< clock hardware fault (ro)
    inline constexpr u32 STA_NANO = 0x2000; ///< resolution (0 = us, 1 = ns) (rw)
    inline constexpr u32 STA_MODE = 0x4000; ///< mode (0 = PLL, 1 = FLL) (rw)
    inline constexpr u32 STA_CLK = 0x8000; ///< clock source (0 = A, 1 = B) (rw)
    inline constexpr u32 STA_RONLY = STA_PPSSIGNAL
        | STA_PPSJITTER | STA_PPSWANDER | STA_PPSERROR
        | STA_CLOCKERR | STA_NANO | STA_MODE | STA_CLK; ///< read-only bits

    /**
     * <unistd.h> constants
     */

    inline constexpr u32 STDIN_FILENO = 0;
    inline constexpr u32 STDOUT_FILENO = 1;
    inline constexpr u32 STDERR_FILENO = 2;

    /**
     * <netdb.h> constants
     */

    inline constexpr i32 AI_PASSIVE = captured::AI_PASSIVE_VALUE; ///< Address is intended for bind().
    inline constexpr i32 AI_CANONNAME = captured::AI_CANONNAME_VALUE; ///< Request the canonical name.
    inline constexpr i32 AI_NUMERICHOST = captured::AI_NUMERICHOST_VALUE; ///< Reject host names; the node is a literal.
    inline constexpr i32 AI_V4MAPPED = captured::AI_V4MAPPED_VALUE; ///< Map IPv4 results into IPv6 when no IPv6 exists.
    inline constexpr i32 AI_ALL = captured::AI_ALL_VALUE; ///< Return both IPv4-mapped and IPv6 addresses.
    inline constexpr i32 AI_ADDRCONFIG = captured::AI_ADDRCONFIG_VALUE; ///< Only return families the host is configured for.
    inline constexpr i32 AI_NUMERICSERV = captured::AI_NUMERICSERV_VALUE; ///< Reject service names; the service is a port number.

    inline constexpr i32 NI_NUMERICHOST = captured::NI_NUMERICHOST_VALUE; ///< Return the address in numeric form.
    inline constexpr i32 NI_NUMERICSERV = captured::NI_NUMERICSERV_VALUE; ///< Return the service as a port number.
    inline constexpr i32 NI_NOFQDN = captured::NI_NOFQDN_VALUE; ///< Return only the hostname part of an FQDN.
    inline constexpr i32 NI_NAMEREQD = captured::NI_NAMEREQD_VALUE; ///< Fail rather than fall back to a numeric form.
    inline constexpr i32 NI_DGRAM = captured::NI_DGRAM_VALUE; ///< Look the service up for a datagram, not a stream.

    inline constexpr usize NI_MAXHOST = captured::NI_MAXHOST_VALUE; ///< Buffer size that always holds a host name.
    inline constexpr usize NI_MAXSERV = captured::NI_MAXSERV_VALUE; ///< Buffer size that always holds a service name.

    inline constexpr i32 EAI_BADFLAGS = captured::EAI_BADFLAGS_VALUE; ///< An unsupported flag was given.
    inline constexpr i32 EAI_NONAME = captured::EAI_NONAME_VALUE; ///< The name resolves to nothing.
    inline constexpr i32 EAI_AGAIN = captured::EAI_AGAIN_VALUE; ///< Temporary failure; the lookup may succeed later.
    inline constexpr i32 EAI_FAIL = captured::EAI_FAIL_VALUE; ///< Non-recoverable failure.
    inline constexpr i32 EAI_NODATA = captured::EAI_NODATA_VALUE; ///< The name is valid but has no address.
    inline constexpr i32 EAI_FAMILY = captured::EAI_FAMILY_VALUE; ///< The address family is not supported.
    inline constexpr i32 EAI_SOCKTYPE = captured::EAI_SOCKTYPE_VALUE; ///< The socket type is not supported.
    inline constexpr i32 EAI_SERVICE = captured::EAI_SERVICE_VALUE; ///< The service is not available for this socket type.
    inline constexpr i32 EAI_MEMORY = captured::EAI_MEMORY_VALUE; ///< Out of memory.
    inline constexpr i32 EAI_SYSTEM = captured::EAI_SYSTEM_VALUE; ///< A system error; consult errno.
    inline constexpr i32 EAI_OVERFLOW = captured::EAI_OVERFLOW_VALUE; ///< An argument buffer was too small.

    /**
     * @namespace netinet
     * @brief Unix Internet protocol operations.
     */
    namespace netinet {
        /**
         * <netinet/in.h> constants
         */

        inline constexpr i32 IPPROTO_IP = captured::IPPROTO_IP_VALUE; ///< Dummy protocol for the IP level itself.
        inline constexpr i32 IPPROTO_ICMP = captured::IPPROTO_ICMP_VALUE; ///< Internet Control Message Protocol.
        inline constexpr i32 IPPROTO_TCP = captured::IPPROTO_TCP_VALUE; ///< Transmission Control Protocol.
        inline constexpr i32 IPPROTO_UDP = captured::IPPROTO_UDP_VALUE; ///< User Datagram Protocol.
        inline constexpr i32 IPPROTO_IPV6 = captured::IPPROTO_IPV6_VALUE; ///< Options for the IPv6 header itself.

        inline constexpr u32 INADDR_ANY = captured::INADDR_ANY_VALUE; ///< The IPv4 wildcard address 0.0.0.0.
        inline constexpr u32 INADDR_LOOPBACK = captured::INADDR_LOOPBACK_VALUE; ///< The IPv4 loopback address 127.0.0.1.
        inline constexpr u32 INADDR_BROADCAST = captured::INADDR_BROADCAST_VALUE; ///< The IPv4 limited broadcast address.

        inline constexpr i32 IPV6_V6ONLY = captured::IPV6_V6ONLY_VALUE; ///< Refuse IPv4-mapped peers on an AF_INET6 socket.
        inline constexpr i32 IPV6_JOIN_GROUP = captured::IPV6_JOIN_GROUP_VALUE; ///< Join an IPv6 multicast group.
        inline constexpr i32 IPV6_LEAVE_GROUP = captured::IPV6_LEAVE_GROUP_VALUE; ///< Leave an IPv6 multicast group.
        inline constexpr i32 IPV6_MULTICAST_HOPS = captured::IPV6_MULTICAST_HOPS_VALUE; ///< Hop limit for outgoing multicast.
        inline constexpr i32 IPV6_MULTICAST_IF = captured::IPV6_MULTICAST_IF_VALUE; ///< Interface for outgoing multicast.
        inline constexpr i32 IPV6_UNICAST_HOPS = captured::IPV6_UNICAST_HOPS_VALUE; ///< Hop limit for outgoing unicast.

        /**
         * <netinet/tcp.h> constants
         */

        inline constexpr i32 TCP_NODELAY = captured::TCP_NODELAY_VALUE; ///< Send segments as soon as possible (disable Nagle).
        inline constexpr i32 TCP_MAXSEG = captured::TCP_MAXSEG_VALUE; ///< Maximum segment size.
        inline constexpr i32 TCP_KEEPINTVL = captured::TCP_KEEPINTVL_VALUE; ///< Seconds between keepalive probes.
        inline constexpr i32 TCP_KEEPCNT = captured::TCP_KEEPCNT_VALUE; ///< Unanswered probes before the peer is declared dead.
        inline constexpr i32 TCP_FASTOPEN = captured::TCP_FASTOPEN_VALUE; ///< Accept data in the opening SYN.

        /// Linux-only knobs; the guard matches captured. TCP_CORK's nearest Darwin equivalent is TCP_NOPUSH, and Darwin's idle-time option is stdx::os::darwin::netinet::TCP_KEEPALIVE.
        #ifdef __linux__
        inline constexpr i32 TCP_CORK = captured::TCP_CORK_VALUE; ///< Hold back partial segments.
        inline constexpr i32 TCP_KEEPIDLE = captured::TCP_KEEPIDLE_VALUE; ///< Idle seconds before the first keepalive probe.
        inline constexpr i32 TCP_QUICKACK = captured::TCP_QUICKACK_VALUE; ///< Leave delayed-ack mode.
        #endif

        enum class TcpCongestionAlgorithmState: u8 {
            OPEN = 0,
            DISORDER = 1,
            CWR = 2,
            RECOVERY = 3,
            LOSS = 4
        };
    }

    /**
     * @namespace sys
     * @brief Unix system operations.
     */
    namespace sys {
        /**
         * <sys/socket.h> constants
         *
         * Read from the real header rather than written out here; see
         * stdx::os::unix::sys::captured in os/unix/sys/socket.inl for how and
         * why. The Linux numbers these used to be are in the git history, and
         * every one of them was correct - on Linux.
         */

        inline constexpr i32 AF_UNSPEC = captured::AF_UNSPEC_VALUE; ///< Unspecified address family.
        inline constexpr i32 AF_UNIX = captured::AF_UNIX_VALUE; ///< Local communication.
        inline constexpr i32 AF_INET = captured::AF_INET_VALUE; ///< IPv4.
        inline constexpr i32 AF_INET6 = captured::AF_INET6_VALUE; ///< IPv6.

        inline constexpr i32 PF_UNSPEC = AF_UNSPEC; ///< Unspecified protocol family.
        inline constexpr i32 PF_INET = AF_INET; ///< IPv4 protocol family.
        inline constexpr i32 PF_INET6 = AF_INET6; ///< IPv6 protocol family.

        inline constexpr i32 SOCK_STREAM = captured::SOCK_STREAM_VALUE; ///< Sequenced, reliable, connection-based byte stream.
        inline constexpr i32 SOCK_DGRAM = captured::SOCK_DGRAM_VALUE; ///< Connectionless, unreliable datagrams of fixed length.
        inline constexpr i32 SOCK_RAW = captured::SOCK_RAW_VALUE; ///< Raw protocol interface.
        inline constexpr i32 SOCK_SEQPACKET = captured::SOCK_SEQPACKET_VALUE; ///< Sequenced, reliable, connection-based datagrams.

        inline constexpr i32 SOL_SOCKET = captured::SOL_SOCKET_VALUE; ///< Options at the socket API level.

        inline constexpr i32 SO_DEBUG = captured::SO_DEBUG_VALUE; ///< Record debugging information.
        inline constexpr i32 SO_REUSEADDR = captured::SO_REUSEADDR_VALUE; ///< Allow reuse of a local address in TIME_WAIT.
        inline constexpr i32 SO_TYPE = captured::SO_TYPE_VALUE; ///< The socket's type (read-only).
        inline constexpr i32 SO_ERROR = captured::SO_ERROR_VALUE; ///< Fetch and clear the pending error (read-only).
        inline constexpr i32 SO_DONTROUTE = captured::SO_DONTROUTE_VALUE; ///< Bypass the routing table.
        inline constexpr i32 SO_BROADCAST = captured::SO_BROADCAST_VALUE; ///< Permit sending to a broadcast address.
        inline constexpr i32 SO_SNDBUF = captured::SO_SNDBUF_VALUE; ///< Send-buffer size.
        inline constexpr i32 SO_RCVBUF = captured::SO_RCVBUF_VALUE; ///< Receive-buffer size.
        inline constexpr i32 SO_KEEPALIVE = captured::SO_KEEPALIVE_VALUE; ///< Send keepalive probes on an idle connection.
        inline constexpr i32 SO_OOBINLINE = captured::SO_OOBINLINE_VALUE; ///< Deliver out-of-band data in the normal stream.
        inline constexpr i32 SO_REUSEPORT = captured::SO_REUSEPORT_VALUE; ///< Permit several sockets to bind the same port.
        inline constexpr i32 SO_RCVLOWAT = captured::SO_RCVLOWAT_VALUE; ///< Minimum bytes before a receive completes.
        inline constexpr i32 SO_SNDLOWAT = captured::SO_SNDLOWAT_VALUE; ///< Minimum bytes before a send completes.
        inline constexpr i32 SO_RCVTIMEO = captured::SO_RCVTIMEO_VALUE; ///< Receive timeout.
        inline constexpr i32 SO_SNDTIMEO = captured::SO_SNDTIMEO_VALUE; ///< Send timeout.
        inline constexpr i32 SO_ACCEPTCONN = captured::SO_ACCEPTCONN_VALUE; ///< Whether the socket is listening (read-only).

        /**
         * @brief Block on close until queued data is sent.
         *
         * Deliberately not a fixed number: Darwin defines this as 0x0080 or
         * 0x1080 depending on whether the consumer defined _POSIX_C_SOURCE, the
         * two meaning ticks and seconds respectively. No table could be right
         * about it; only reading the header can.
         */
        inline constexpr i32 SO_LINGER = captured::SO_LINGER_VALUE;

        inline constexpr i32 MSG_OOB = captured::MSG_OOB_VALUE; ///< Process out-of-band data.
        inline constexpr i32 MSG_PEEK = captured::MSG_PEEK_VALUE; ///< Read without consuming.
        inline constexpr i32 MSG_DONTROUTE = captured::MSG_DONTROUTE_VALUE; ///< Bypass the routing table for this call.
        inline constexpr i32 MSG_TRUNC = captured::MSG_TRUNC_VALUE; ///< Report the untruncated datagram length.
        inline constexpr i32 MSG_DONTWAIT = captured::MSG_DONTWAIT_VALUE; ///< Make this call non-blocking.
        inline constexpr i32 MSG_EOR = captured::MSG_EOR_VALUE; ///< End of record.
        inline constexpr i32 MSG_WAITALL = captured::MSG_WAITALL_VALUE; ///< Do not return until the whole request is met.

        /**
         * @brief Report EPIPE from a send to a closed peer instead of raising SIGPIPE.
         *
         * 0x4000 on Linux and 0x80000 on Darwin, and Linux's value is Darwin's
         * MSG_RCVMORE, so the two cannot be interchanged and getting it wrong
         * does not fail loudly - it kills the process on the next write to a
         * closed peer. The guard matches the one on captured::MSG_NOSIGNAL_VALUE.
         */
        #if defined(__linux__) || defined(__APPLE__)
        inline constexpr i32 MSG_NOSIGNAL = captured::MSG_NOSIGNAL_VALUE;
        #endif

        /**
         * @brief Suppresses SIGPIPE for the socket's whole lifetime rather than per send.
         *
         * Darwin-only, and the fallback if the kernel there turns out to ignore
         * MSG_NOSIGNAL. Unlike the send flag it has to be set on every
         * descriptor, including each accepted peer, since it is not inherited
         * from the listener.
         */
        #ifdef __APPLE__
        inline constexpr i32 SO_NOSIGPIPE = captured::SO_NOSIGPIPE_VALUE;
        #endif

        /// Linux extensions; the guards match those on the captured values.
        #ifdef __linux__
        inline constexpr i32 AF_PACKET = captured::AF_PACKET_VALUE; ///< Low-level packet interface.
        inline constexpr i32 SOCK_NONBLOCK = captured::SOCK_NONBLOCK_VALUE; ///< Open the socket non-blocking.
        inline constexpr i32 SOCK_CLOEXEC = captured::SOCK_CLOEXEC_VALUE; ///< Close the socket on exec.
        inline constexpr i32 SO_PROTOCOL = captured::SO_PROTOCOL_VALUE; ///< The socket's protocol (read-only).
        inline constexpr i32 SO_DOMAIN = captured::SO_DOMAIN_VALUE; ///< The socket's address family (read-only).
        #endif

        inline constexpr i32 SHUT_RD = captured::SHUT_RD_VALUE; ///< Shut the receive half down.
        inline constexpr i32 SHUT_WR = captured::SHUT_WR_VALUE; ///< Shut the send half down.
        inline constexpr i32 SHUT_RDWR = captured::SHUT_RDWR_VALUE; ///< Shut both halves down.

        inline constexpr i32 SOMAXCONN = captured::SOMAXCONN_VALUE; ///< Backlog ceiling; listen() clamps to the sysctl anyway.

        /**
         * <sys/ioctl.h> constants
         */

        inline constexpr i32 NCC = 8;

        // TIOCM flags for modem control lines
        inline constexpr i32 TIOCM_LE = 0x001; ///< Loopback control
        inline constexpr i32 TIOCM_DTR = 0x002; ///< Data Terminal Ready
        inline constexpr i32 TIOCM_RTS = 0x004; ///< Request to Send
        inline constexpr i32 TIOCM_ST = 0x008; ///< Secondary Transmit Data
        inline constexpr i32 TIOCM_SR = 0x010; ///< Secondary Receive Data
        inline constexpr i32 TIOCM_CTS = 0x020; ///< Clear to Send
        inline constexpr i32 TIOCM_CAR = 0x040; ///< Carrier Detect
        inline constexpr i32 TIOCM_RNG = 0x080; ///< Ring Indicator
        inline constexpr i32 TIOCM_DSR = 0x100; ///< Data Set Ready
        inline constexpr i32 TIOCM_CD = TIOCM_CAR; ///< Carrier Detect (alias)
        inline constexpr i32 TIOCM_RI = TIOCM_RNG; ///< Ring Indicator (alias)

        // Network device types for serial connections
        inline constexpr i32 N_TTY = 0; ///< TTY (teletype) serial device
        inline constexpr i32 N_SLIP = 1; ///< Serial Line Internet Protocol (SLIP)
        inline constexpr i32 N_MOUSE = 2; ///< Mouse device
        inline constexpr i32 N_PPP = 3; ///< Poi32-to-Poi32 Protocol (PPP)
        inline constexpr i32 N_STRIP = 4; ///< Striping (frame mode)
        inline constexpr i32 N_AX25 = 5; ///< AX.25 protocol (Amateur Radio)
        inline constexpr i32 N_X25 = 6; ///< X.25 asynchronous protocol
        inline constexpr i32 N_6PACK = 7; ///< 6PACK protocol
        inline constexpr i32 N_MASC = 8; ///< Mobitex module (wireless)
        inline constexpr i32 N_R3964 = 9; ///< Simatic R3964 module (industrial control)
        inline constexpr i32 N_PROFIBUS_FDL = 10; ///< Profibus Fieldbus Protocol
        inline constexpr i32 N_IRDA = 11; ///< Infrared Data Association (IR)
        inline constexpr i32 N_SMSBLOCK = 12; ///< SMS block mode
        inline constexpr i32 N_HDLC = 13; ///< Synchronous HDLC (High-Level Data Link Control)
        inline constexpr i32 N_SYNC_PPP = 14; ///< Synchronous PPP protocol
        inline constexpr i32 N_HCI = 15; ///< Bluetooth Host Controller Interface (HCI)

        inline constexpr i32 TCGETS = 0x5401;
        inline constexpr i32 TCSETS = 0x5402;
        inline constexpr i32 TCSETSW = 0x5403;
        inline constexpr i32 TCSETSF = 0x5404;
        inline constexpr i32 TCGETA = 0x5405;
        inline constexpr i32 TCSETA = 0x5406;
        inline constexpr i32 TCSETAW = 0x5407;
        inline constexpr i32 TCSETAF = 0x5408;
        inline constexpr i32 TCSBRK = 0x5409;
        inline constexpr i32 TCXONC = 0x540A;
        inline constexpr i32 TCFLSH = 0x540B;
        inline constexpr i32 TIOCEXCL = 0x540C;
        inline constexpr i32 TIOCNXCL = 0x540D;
        inline constexpr i32 TIOCSCTTY = 0x540E;
        inline constexpr i32 TIOCGPGRP = 0x540F;
        inline constexpr i32 TIOCSPGRP = 0x5410;
        inline constexpr i32 TIOCOUTQ = 0x5411;
        inline constexpr i32 TIOCSTI = 0x5412;
        inline constexpr i32 TIOCGWINSZ = 0x5413;
        inline constexpr i32 TIOCSWINSZ = 0x5414;
        inline constexpr i32 TIOCMGET = 0x5415;
        inline constexpr i32 TIOCMBIS = 0x5416;
        inline constexpr i32 TIOCMBIC = 0x5417;
        inline constexpr i32 TIOCMSET = 0x5418;
        inline constexpr i32 TIOCGSOFTCAR = 0x5419;
        inline constexpr i32 TIOCSSOFTCAR = 0x541A;
        inline constexpr i32 FIONREAD = 0x541B;
        inline constexpr i32 TIOCINQ = FIONREAD;
        inline constexpr i32 TIOCLINUX = 0x541C;
        inline constexpr i32 TIOCCONS = 0x541D;
        inline constexpr i32 TIOCGSERIAL = 0x541E;
        inline constexpr i32 TIOCSSERIAL = 0x541F;
        inline constexpr i32 TIOCPKT = 0x5420;
        inline constexpr i32 FIONBIO = 0x5421;
        inline constexpr i32 TIOCNOTTY = 0x5422;
        inline constexpr i32 TIOCSETD = 0x5423;
        inline constexpr i32 TIOCGETD = 0x5424;
        inline constexpr i32 TCSBRKP = 0x5425;
        inline constexpr i32 TIOCSBRK = 0x5427;
        inline constexpr i32 TIOCCBRK = 0x5428;
        inline constexpr i32 TIOCGSID = 0x5429;
        inline constexpr i32 TIOCGRS485 = 0x542E;
        inline constexpr i32 TIOCSRS485 = 0x542F;
        inline constexpr i32 TCGETX = 0x5432;
        inline constexpr i32 TCSETX = 0x5433;
        inline constexpr i32 TCSETXF = 0x5434;
        inline constexpr i32 TCSETXW = 0x5435;
        inline constexpr i32 TIOCSIG = 0x5436;
        inline constexpr i32 TIOCVHANGUP = 0x5437;
        inline constexpr i32 TIOCGPKT = 0x5438;
        inline constexpr i32 TIOCGPTLCK = 0x5439;
        inline constexpr i32 TIOCGEXCL = 0x5440;
        inline constexpr i32 TIOCGPTPEER = 0x5441;
        inline constexpr i32 TIOCGISO7816 = 0x5442;
        inline constexpr i32 TIOCSISO7816 = 0x5443;
        inline constexpr i32 FIONCLEX = 0x5450;
        inline constexpr i32 FIOCLEX = 0x5451;
        inline constexpr i32 FIOASYNC = 0x5452;
        inline constexpr i32 TIOCSERCONFIG = 0x5453;
        inline constexpr i32 TIOCSERGWILD = 0x5454;
        inline constexpr i32 TIOCSERSWILD = 0x5455;
        inline constexpr i32 TIOCGLCKTRMIOS = 0x5456;
        inline constexpr i32 TIOCSLCKTRMIOS = 0x5457;
        inline constexpr i32 TIOCSERGSTRUCT = 0x5458;
        inline constexpr i32 TIOCSERGETLSR = 0x5459;
        inline constexpr i32 TIOCSERGETMULTI = 0x545A;
        inline constexpr i32 TIOCSERSETMULTI = 0x545B;
        inline constexpr i32 TIOCMIWAIT = 0x545C;
        inline constexpr i32 TIOCGICOUNT = 0x545D;
        inline constexpr i32 FIOQSIZE = 0x5460;

        inline constexpr i32 TIOCPKT_DATA = 0;
        inline constexpr i32 TIOCPKT_FLUSHREAD = 1;
        inline constexpr i32 TIOCPKT_FLUSHWRITE = 2;
        inline constexpr i32 TIOCPKT_STOP = 4;
        inline constexpr i32 TIOCPKT_START = 8;
        inline constexpr i32 TIOCPKT_NOSTOP = 16;
        inline constexpr i32 TIOCPKT_DOSTOP = 32;
        inline constexpr i32 TIOCPKT_IOCTL = 64;

        /**
         * <sys/mount.h> constants
         */

        // Block size constants
        inline constexpr i32 BLOCK_SIZE_BITS = 10;
        inline constexpr i32 BLOCK_SIZE = 1 << BLOCK_SIZE_BITS;

        // Filesystem-independent mount flags
        inline constexpr u64 MS_RDONLY = 1; ///< Mount read-only
        inline constexpr u64 MS_NOSUID = 2; ///< Ignore suid and sgid bits
        inline constexpr u64 MS_NODEV = 4; ///< Disallow access to device special files
        inline constexpr u64 MS_NOEXEC = 8; ///< Disallow program execution
        inline constexpr u64 MS_SYNCHRONOUS = 16; ///< Writes are synced at once
        inline constexpr u64 MS_REMOUNT = 32; ///< Alter flags of a mounted FS
        inline constexpr u64 MS_MANDLOCK = 64; ///< Allow mandatory locks on an FS
        inline constexpr u64 MS_DIRSYNC = 128; ///< Directory modifications are synchronous
        inline constexpr u64 MS_NOSYMFOLLOW = 256; ///< Do not follow symlinks
        inline constexpr u64 MS_NOATIME = 1024; ///< Do not update access times
        inline constexpr u64 MS_NODIRATIME = 2048; ///< Do not update directory access times
        inline constexpr u64 MS_BIND = 4096; ///< Bind directory at different place
        inline constexpr u64 MS_MOVE = 8192;
        inline constexpr u64 MS_REC = 16384;
        inline constexpr u64 MS_SILENT = 32768;
        inline constexpr u64 MS_POSIXACL = 1 << 16; ///< VFS does not apply the umask
        inline constexpr u64 MS_UNBINDABLE = 1 << 17; ///< Change to unbindable
        inline constexpr u64 MS_PRIVATE = 1 << 18; ///< Change to private
        inline constexpr u64 MS_SLAVE = 1 << 19; ///< Change to slave
        inline constexpr u64 MS_SHARED = 1 << 20; ///< Change to shared
        inline constexpr u64 MS_RELATIME = 1 << 21; ///< Update atime relative to mtime/ctime
        inline constexpr u64 MS_KERNMOUNT = 1 << 22; ///< This is a kern_mount call
        inline constexpr u64 MS_I_VERSION = 1 << 23; ///< Update inode I_version field
        inline constexpr u64 MS_STRICTATIME = 1 << 24; ///< Always perform atime updates
        inline constexpr u64 MS_LAZYTIME = 1 << 25; ///< Update the on-disk [acm]times lazily
        inline constexpr u64 MS_ACTIVE = 1 << 30;
        inline constexpr u64 MS_NOUSER = 1U << 31;

        // Flags that can be altered by MS_REMOUNT
        inline constexpr u64 MS_RMT_MASK = MS_RDONLY | MS_SYNCHRONOUS | MS_MANDLOCK | MS_I_VERSION | MS_LAZYTIME;

        // Magic mount flag number
        inline constexpr u64 MS_MGC_VAL = 0xc0ed0000; ///< Magic flag number to indicate "new" flags
        inline constexpr u64 MS_MGC_MSK = 0xffff0000; ///< Magic flag number mask

        // Block device ioctl constants
        inline constexpr i32 BLKROSET = 0x1000 | (0x12 << 8) | 93; ///< Set device read-only (0 = read-write)
        inline constexpr i32 BLKROGET = 0x1000 | (0x12 << 8) | 94; ///< Get read-only status (0 = read_write)
        inline constexpr i32 BLKRRPART = 0x1000 | (0x12 << 8) | 95; ///< Re-read partition table
        inline constexpr i32 BLKGETSIZE = 0x1000 | (0x12 << 8) | 96; ///< Return device size
        inline constexpr i32 BLKFLSBUF = 0x1000 | (0x12 << 8) | 97; ///< Flush buffer cache
        inline constexpr i32 BLKRASET = 0x1000 | (0x12 << 8) | 98; ///< Set read ahead for block device
        inline constexpr i32 BLKRAGET = 0x1000 | (0x12 << 8) | 99; ///< Get current read ahead setting
        inline constexpr i32 BLKFRASET = 0x1000 | (0x12 << 8) | 100; ///< Set filesystem read-ahead
        inline constexpr i32 BLKFRAGET = 0x1000 | (0x12 << 8) | 101; ///< Get filesystem read-ahead
        inline constexpr i32 BLKSECTSET = 0x1000 | (0x12 << 8) | 102; ///< Set max sectors per request
        inline constexpr i32 BLKSECTGET = 0x1000 | (0x12 << 8) | 103; ///< Get max sectors per request
        inline constexpr i32 BLKSSZGET = 0x1000 | (0x12 << 8) | 104; ///< Get block device sector size

        // Unmount flags
        inline constexpr i32 MNT_FORCE = 1; ///< Force unmounting
        inline constexpr i32 MNT_DETACH = 2; ///< Just detach from the tree
        inline constexpr i32 MNT_EXPIRE = 4; ///< Mark for expiry
        inline constexpr i32 UMOUNT_NOFOLLOW = 8; ///< Don't follow symlink on umount

        // fsmount flags
        inline constexpr i32 FSMOUNT_CLOEXEC = 0x00000001;

        // Mount attributes
        inline constexpr i32 MOUNT_ATTR_RDONLY = 0x00000001; ///< Mount read-only
        inline constexpr i32 MOUNT_ATTR_NOSUID = 0x00000002; ///< Ignore suid and sgid bits
        inline constexpr i32 MOUNT_ATTR_NODEV = 0x00000004; ///< Disallow access to device special files
        inline constexpr i32 MOUNT_ATTR_NOEXEC = 0x00000008; ///< Disallow program execution
        inline constexpr i32 MOUNT_ATTR__ATIME = 0x00000070; ///< Setting on how atime should be updated
        inline constexpr i32 MOUNT_ATTR_RELATIME = 0x00000000; ///< Update atime relative to mtime/ctime
        inline constexpr i32 MOUNT_ATTR_NOATIME = 0x00000010; ///< Do not update access times
        inline constexpr i32 MOUNT_ATTR_STRICTATIME = 0x00000020; ///< Always perform atime updates
        inline constexpr i32 MOUNT_ATTR_NODIRATIME = 0x00000080; ///< Do not update directory access times
        inline constexpr i32 MOUNT_ATTR_IDMAP = 0x00100000; ///< Idmap mount to @userns_fd in struct mount_attr
        inline constexpr i32 MOUNT_ATTR_NOSYMFOLLOW = 0x00200000; ///< Do not follow symlinks

        inline constexpr i32 MOUNT_ATTR_SIZE_VER0 = 32; ///< sizeof first published struct

        // move_mount flags
        inline constexpr i32 MOVE_MOUNT_F_SYMLINKS = 0x00000001; ///< Follow symlinks on from path
        inline constexpr i32 MOVE_MOUNT_F_AUTOMOUNTS = 0x00000002; ///< Follow automounts on from path
        inline constexpr i32 MOVE_MOUNT_F_EMPTY_PATH = 0x00000004; ///< Empty from path permitted
        inline constexpr i32 MOVE_MOUNT_T_SYMLINKS = 0x00000010; ///< Follow symlinks on to path
        inline constexpr i32 MOVE_MOUNT_T_AUTOMOUNTS = 0x00000020; ///< Follow automounts on to path
        inline constexpr i32 MOVE_MOUNT_T_EMPTY_PATH = 0x00000040; ///< Empty to path permitted
        inline constexpr i32 MOVE_MOUNT_SET_GROUP = 0x00000100; ///< Set sharing group instead
        inline constexpr i32 MOVE_MOUNT_BENEATH = 0x00000200; ///< Mount beneath top mount

        // fspick flags
        inline constexpr i32 FSPICK_CLOEXEC = 0x00000001;
        inline constexpr i32 FSPICK_SYMLINK_NOFOLLOW = 0x00000002;
        inline constexpr i32 FSPICK_NO_AUTOMOUNT = 0x00000004;
        inline constexpr i32 FSPICK_EMPTY_PATH = 0x00000008;

        // fsconfig commands
        inline constexpr i32 FSCONFIG_SET_FLAG = 0; ///< Set parameter, supplying no value
        inline constexpr i32 FSCONFIG_SET_STRING = 1; ///< Set parameter, supplying a string value
        inline constexpr i32 FSCONFIG_SET_BINARY = 2; ///< Set parameter, supplying a binary blob value
        inline constexpr i32 FSCONFIG_SET_PATH = 3; ///< Set parameter, supplying an object by path
        inline constexpr i32 FSCONFIG_SET_PATH_EMPTY = 4; ///< Set parameter, supplying an object by (empty) path
        inline constexpr i32 FSCONFIG_SET_FD = 5; ///< Set parameter, supplying an object by fd
        inline constexpr i32 FSCONFIG_CMD_CREATE = 6; ///< Invoke superblock creation
        inline constexpr i32 FSCONFIG_CMD_RECONFIGURE = 7; ///< Invoke superblock reconfiguration
        inline constexpr i32 FSCONFIG_CMD_CREATE_EXCL = 8; ///< Create new superblock, fail if reusing existing superblock

        // fsopen flags
        inline constexpr i32 FSOPEN_CLOEXEC = 0x00000001;

        // open_tree flags
        inline constexpr i32 OPEN_TREE_CLONE = 1; ///< Clone the target tree and attach the clone
        inline constexpr i32 OPEN_TREE_CLOEXEC = 0x80000; ///< Close the file on execve() (O_CLOEXEC)

        /**
         * <sys/reboot.h> constants
         */

        inline constexpr u32 RB_AUTOBOOT = 0x01234567; ///< Perform a hard reset now.
        inline constexpr u32 RB_HALT_SYSTEM = 0xCDEF0123; ///< Halt the system.
        inline constexpr u32 RB_ENABLE_CAD = 0x89ABCDEF; ///< Enable reboot using Ctrl-Alt-Delete keystroke.
        inline constexpr u32 RB_DISABLE_CAD = 0x00000000; ///< Disable reboot using Ctrl-Alt-Delete keystroke.
        inline constexpr u32 RB_POWER_OFF = 0x4321FEDC; ///< Stop system and switch power off if possible.
        inline constexpr u32 RB_SW_SUSPEND = 0xD000FCE2; ///< Suspend system using software suspend.
        inline constexpr u32 RB_KEXEC = 0x45584543; ///< Reboot system i32o new kernel.

        /**
         * <sys/ttydefaults.h> constants
         */

        inline constexpr i32 TTYDEF_IFLAG = BRKINT | ISTRIP | ICRNL | IMAXBEL | IXON | IXANY;
        inline constexpr i32 TTYDEF_OFLAG = OPOST | ONLCR | XTABS;
        inline constexpr i32 TTYDEF_LFLAG = ECHO | ICANON | ISIG | IEXTEN | ECHOE | ECHOKE | ECHOCTL;
        inline constexpr i32 TTYDEF_CFLAG = CREAD | CS7 | PARENB | HUPCL;
        inline constexpr i32 TTYDEF_SPEED = B9600;
        
        [[nodiscard]]
        constexpr u8 CTRL(char x) noexcept {
            return x & 037; 
        }
        
        inline constexpr u8 CEOF = CTRL('d');
        inline constexpr u8 CEOL = '\0';
        inline constexpr u8 CERASE = 0177;
        inline constexpr u8 CINTR = CTRL('c');
        inline constexpr u8 CSTATUS = '\0';
        inline constexpr u8 CKILL = CTRL('u');
        inline constexpr u8 CMIN = 1;
        inline constexpr u8 CQUIT = 034;
        inline constexpr u8 CSUSP = CTRL('z');
        inline constexpr u8 CTIME = 0;
        inline constexpr u8 CDSUSP = CTRL('y');
        inline constexpr u8 CSTART = CTRL('q');
        inline constexpr u8 CSTOP = CTRL('s');
        inline constexpr u8 CLNEXT = CTRL('v');
        inline constexpr u8 CDISCARD = CTRL('o');
        inline constexpr u8 CWERASE = CTRL('w');
        inline constexpr u8 CREPRINT = CTRL('r');
        inline constexpr u8 CEOT = CEOF;
        inline constexpr u8 CBRK = CEOL;
        inline constexpr u8 CRPRNT = CREPRINT;
        inline constexpr u8 CFLUSH = CDISCARD;
        
        inline constexpr Array<u8, 32> TTYDEFCHARS = {
            CEOF, CEOL, CEOL, CERASE, CWERASE, CKILL, CREPRINT,
            '\0', CINTR, CQUIT, CSUSP, CDSUSP, CSTART, CSTOP, CLNEXT,
            CDISCARD, CMIN, CTIME, CSTATUS, '\0'
        };

        /**
         * <sys/wait.h> constants
         */

        inline constexpr i32 WNOHANG = 1; ///< Don't block waiting for child to exit
        inline constexpr i32 WUNTRACED = 2; ///< Report status of stopped children
        inline constexpr i32 WSTOPPED = 2; ///< Report status of stopped children (alias)
        inline constexpr i32 WEXITED = 4; ///< Report status of exited children
        inline constexpr i32 WCONTINUED = 8; ///< Report status of continued children
        inline constexpr i32 WNOWAIT = 0x1000000; ///< Don't reap, just poll status

        [[nodiscard]]
        constexpr bool WIFCONTINUED(i32 s) noexcept {
            return s == WCONTINUED;
        }

        [[nodiscard]]
        constexpr i32 WEXITSTATUS(i32 s) noexcept {
            return (s & 0xFF00) >> 8;
        }

        [[nodiscard]]
        constexpr bool WSTOPSIG(i32 s) noexcept {
            return WEXITSTATUS(s);
        }

        [[nodiscard]]
        constexpr bool WIFSIGNALED(i32 s) noexcept {
            return ((s & 0x7F) + 1) >> 1 > 0;
        }

        [[nodiscard]]
        constexpr i32 WTERMSIG(i32 s) noexcept {
            return s & 0x7F;
        }

        [[nodiscard]]
        constexpr bool WIFEXITED(i32 s) noexcept {
            return WTERMSIG(s) == 0;
        }

        [[nodiscard]]
        constexpr bool WIFSTOPPED(i32 s) noexcept {
            return (s & 0xFF) == 0x7F;
        }
    }
    #endif
}
