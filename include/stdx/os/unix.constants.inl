#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Unix operations.
 */
export namespace stdx::os::unix {
    /**
     * <errno.h> constants
     */

    inline constexpr i32 EPERM = 1; ///< Operation not permitted
    inline constexpr i32 ENOENT = 2; ///< No such file or directory
    inline constexpr i32 ESRCH = 3; ///< No such process
    inline constexpr i32 EINTR = 4; ///< Interrupted system call
    inline constexpr i32 EIO = 5; ///< Input/output error
    inline constexpr i32 ENXIO = 6; ///< No such device or address
    inline constexpr i32 E2BIG = 7; ///< Argument list too long
    inline constexpr i32 ENOEXEC = 8; ///< Exec format error
    inline constexpr i32 EBADF = 9; ///< Bad file descriptor
    inline constexpr i32 ECHILD = 10; ///< No child processes
    inline constexpr i32 EAGAIN = 11; ///< Resource temporarily unavailable
    inline constexpr i32 ENOMEM = 12; ///< Cannot allocate memory
    inline constexpr i32 EACCES = 13; ///< Permission denied
    inline constexpr i32 EFAULT = 14; ///< Bad address
    inline constexpr i32 ENOTBLK = 15; ///< Block device required
    inline constexpr i32 EBUSY = 16; ///< Device or resource busy
    inline constexpr i32 EEXIST = 17; ///< File exists
    inline constexpr i32 EXDEV = 18; ///< Invalid cross-device link
    inline constexpr i32 ENODEV = 19; ///< No such device
    inline constexpr i32 ENOTDIR = 20; ///< Not a directory
    inline constexpr i32 EISDIR = 21; ///< Is a directory
    inline constexpr i32 EINVAL = 22; ///< Invalid argument
    inline constexpr i32 ENFILE = 23; ///< Too many open files in system
    inline constexpr i32 EMFILE = 24; ///< Too many open files
    inline constexpr i32 ENOTTY = 25; ///< Inappropriate ioctl for device
    inline constexpr i32 ETXTBSY = 26; ///< Text file busy
    inline constexpr i32 EFBIG = 27; ///< File too large
    inline constexpr i32 ENOSPC = 28; ///< No space left on device
    inline constexpr i32 ESPIPE = 29; ///< Illegal seek
    inline constexpr i32 EROFS = 30; ///< Read-only file system
    inline constexpr i32 EMLINK = 31; ///< Too many links
    inline constexpr i32 EPIPE = 32; ///< Broken pipe
    inline constexpr i32 EDOM = 33; ///< Numerical argument out of domain
    inline constexpr i32 ERANGE = 34; ///< Numerical result out of range
    inline constexpr i32 EDEADLK = 35; ///< Resource deadlock avoided
    inline constexpr i32 ENAMETOOLONG = 36; ///< File name too long
    inline constexpr i32 ENOLCK = 37; ///< No locks available
    inline constexpr i32 ENOSYS = 38; ///< Function not implemented
    inline constexpr i32 ENOTEMPTY = 39; ///< Directory not empty
    inline constexpr i32 ELOOP = 40; ///< Too many levels of symbolic links
    inline constexpr i32 ENOMSG = 42; ///< No message of desired type
    inline constexpr i32 EIDRM = 43; ///< Identifier removed
    inline constexpr i32 ECHRNG = 44; ///< Channel number out of range
    inline constexpr i32 EL2NSYNC = 45; ///< Level 2 not synchronized
    inline constexpr i32 EL3HLT = 46; ///< Level 3 halted
    inline constexpr i32 EL3RST = 47; ///< Level 3 reset
    inline constexpr i32 ELNRNG = 48; ///< Link number out of range
    inline constexpr i32 EUNATCH = 49; ///< Protocol driver not attached
    inline constexpr i32 ENOCSI = 50; ///< No CSI structure available
    inline constexpr i32 EL2HLT = 51; ///< Level 2 halted
    inline constexpr i32 EBADE = 52; ///< Invalid exchange
    inline constexpr i32 EBADR = 53; ///< Invalid request descriptor
    inline constexpr i32 EXFULL = 54; ///< Exchange full
    inline constexpr i32 ENOANO = 55; ///< No anode
    inline constexpr i32 EBADRQC = 56; ///< Invalid request code
    inline constexpr i32 EBADSLT = 57; ///< Invalid slot
    inline constexpr i32 EBFONT = 59; ///< Bad font file format
    inline constexpr i32 ENOSTR = 60; ///< Device not a stream
    inline constexpr i32 ENODATA = 61; ///< No data available
    inline constexpr i32 ETIME = 62; ///< Timer expired
    inline constexpr i32 ENOSR = 63; ///< Out of streams resources
    inline constexpr i32 ENONET = 64; ///< Machine is not on the network
    inline constexpr i32 ENOPKG = 65; ///< Package not installed
    inline constexpr i32 EREMOTE = 66; ///< Object is remote
    inline constexpr i32 ENOLINK = 67; ///< Link has been severed
    inline constexpr i32 EADV = 68; ///< Advertise error
    inline constexpr i32 ESRMNT = 69; ///< Srmount error
    inline constexpr i32 ECOMM = 70; ///< Communication error on send
    inline constexpr i32 EPROTO = 71; ///< Protocol error
    inline constexpr i32 EMULTIHOP = 72; ///< Multihop attempted
    inline constexpr i32 EDOTDOT = 73; ///< RFS specific error
    inline constexpr i32 EBADMSG = 74; ///< Bad message
    inline constexpr i32 EOVERFLOW = 75; ///< Value too large for defined data type
    inline constexpr i32 ENOTUNIQ = 76; ///< Name not unique on network
    inline constexpr i32 EBADFD = 77; ///< File descriptor in bad state
    inline constexpr i32 EREMCHG = 78; ///< Remote address changed
    inline constexpr i32 ELIBACC = 79; ///< Can not access a needed shared library
    inline constexpr i32 ELIBBAD = 80; ///< Accessing a corrupted shared library
    inline constexpr i32 ELIBSCN = 81; ///< .lib section in a.out corrupted
    inline constexpr i32 ELIBMAX = 82; ///< Attempting to link in too many shared libraries
    inline constexpr i32 ELIBEXEC = 83; ///< Cannot exec a shared library directly
    inline constexpr i32 EILSEQ = 84; ///< Invalid or incomplete multibyte or wide character
    inline constexpr i32 ERESTART = 85; ///< Interrupted system call should be restarted
    inline constexpr i32 ESTRPIPE = 86; ///< Streams pipe error
    inline constexpr i32 EUSERS = 87; ///< Too many users
    inline constexpr i32 ENOTSOCK = 88; ///< Socket operation on non-socket
    inline constexpr i32 EDESTADDRREQ = 89; ///< Destination address required
    inline constexpr i32 EMSGSIZE = 90; ///< Message too long
    inline constexpr i32 EPROTOTYPE = 91; ///< Protocol wrong type for socket
    inline constexpr i32 ENOPROTOOPT = 92; ///< Protocol not available
    inline constexpr i32 EPROTONOSUPPORT = 93; ///< Protocol not supported
    inline constexpr i32 ESOCKTNOSUPPORT = 94; ///< Socket type not supported
    inline constexpr i32 EOPNOTSUPP = 95; ///< Operation not supported
    inline constexpr i32 EPFNOSUPPORT = 96; ///< Protocol family not supported
    inline constexpr i32 EAFNOSUPPORT = 97; ///< Address family not supported by protocol
    inline constexpr i32 EADDRINUSE = 98; ///< Address already in use
    inline constexpr i32 EADDRNOTAVAIL = 99; ///< Cannot assign requested address
    inline constexpr i32 ENETDOWN = 100; ///< Network is down
    inline constexpr i32 ENETUNREACH = 101; ///< Network is unreachable
    inline constexpr i32 ENETRESET = 102; ///< Network dropped connection on reset
    inline constexpr i32 ECONNABORTED = 103; ///< Software caused connection abort
    inline constexpr i32 ECONNRESET = 104; ///< Connection reset by peer
    inline constexpr i32 ENOBUFS = 105; ///< No buffer space available
    inline constexpr i32 EISCONN = 106; ///< Transport endpoint is already connected
    inline constexpr i32 ENOTCONN = 107; ///< Transport endpoint is not connected
    inline constexpr i32 ESHUTDOWN = 108; ///< Cannot send after transport endpoint shutdown
    inline constexpr i32 ETOOMANYREFS = 109; ///< Too many references: cannot splice
    inline constexpr i32 ETIMEDOUT = 110; ///< Connection timed out
    inline constexpr i32 ECONNREFUSED = 111; ///< Connection refused
    inline constexpr i32 EHOSTDOWN = 112; ///< Host is down
    inline constexpr i32 EHOSTUNREACH = 113; ///< No route to host
    inline constexpr i32 EALREADY = 114; ///< Operation already in progress
    inline constexpr i32 EINPROGRESS = 115; ///< Operation now in progress
    inline constexpr i32 ESTALE = 116; ///< Stale file handle
    inline constexpr i32 EUCLEAN = 117; ///< Structure needs cleaning
    inline constexpr i32 ENOTNAM = 118; ///< Not a Xenix named type file
    inline constexpr i32 ENAVAIL = 119; ///< No Xenix semaphores available
    inline constexpr i32 EISNAM = 120; ///< Is a named type file
    inline constexpr i32 EREMOTEIO = 121; ///< Remote I/O error
    inline constexpr i32 EDQUOT = 122; ///< Disk quota exceeded
    inline constexpr i32 ENOMEDIUM = 123; ///< No medium found
    inline constexpr i32 EMEDIUMTYPE = 124; ///< Wrong medium type
    inline constexpr i32 ECANCELED = 125; ///< Operation canceled
    inline constexpr i32 ENOKEY = 126; ///< Required key not available
    inline constexpr i32 EKEYEXPIRED = 127; ///< Key has expired
    inline constexpr i32 EKEYREVOKED = 128; ///< Key has been revoked
    inline constexpr i32 EKEYREJECTED = 129; ///< Key was rejected by service
    inline constexpr i32 EOWNERDEAD = 130; ///< Owner died
    inline constexpr i32 ENOTRECOVERABLE = 131; ///< State not recoverable
    inline constexpr i32 ERFKILL = 132; ///< Operation not possible due to RF-kill
    inline constexpr i32 EHWPOISON = 133; ///< Memory page has hardware error
    inline constexpr i32 ENOTSUP = 134; ///< Not supported parameter or option
    inline constexpr i32 ENOMEDIUM_ALT = 135; ///< Missing media
    inline constexpr i32 EILSEQ_ALT = 138; ///< Invalid multibyte sequence
    inline constexpr i32 EOVERFLOW_ALT = 139; ///< Value too large
    inline constexpr i32 ECANCELED_ALT = 140; ///< Asynchronous operation stopped before normal completion
    inline constexpr i32 ENOTRECOVERABLE_ALT = 141; ///< State not recoverable
    inline constexpr i32 EOWNERDEAD_ALT = 142; ///< Previous owner died
    inline constexpr i32 ESTRPIPE_ALT = 143; ///< Streams pipe error

    /**
        * <fcntl.h> constants
        */

    inline constexpr i32 F_DUPFD = 0; ///< Duplicate file descriptor
    inline constexpr i32 F_GETFD = 1; ///< Get file descriptor flags
    inline constexpr i32 F_SETFD = 2; ///< Set file descriptor flags
    inline constexpr i32 F_GETFL = 3; ///< Get file status flags
    inline constexpr i32 F_SETFL = 4; ///< Set file status flags
    inline constexpr i32 F_GETLK = 5; ///< Get record locking information
    inline constexpr i32 F_SETLK = 6; ///< Set record locking information
    inline constexpr i32 F_SETLKW = 7; ///< Set record locking information; wait if blocked
    inline constexpr i32 F_SETOWN = 8; ///< Set owner (process receiving SIGIO)
    inline constexpr i32 F_GETOWN = 9; ///< Get owner (process receiving
    inline constexpr i32 F_SETSIG = 10; ///< Set number of signal to be sent on SIGIO
    inline constexpr i32 F_GETSIG = 11; ///< Get number of signal to be sent on SIGIO
    inline constexpr i32 F_GETLK64 = 12; ///< Get record locking information (using 64-bit file sizes)
    inline constexpr i32 F_SETLK64 = 13; ///< Set record locking information (using 64-bit file sizes)
    inline constexpr i32 F_SETLKW64 = 14; ///< Set record locking information; wait if blocked (using 64-bit file sizes)
    inline constexpr i32 F_SETOWN_EX = 15; ///< Set owner, using pidfd (instead of process id)
    inline constexpr i32 F_GETOWN_EX = 16; ///< Get owner, using pidfd (instead of process id)
    inline constexpr i32 F_DUPFD_CLOEXEC = 1030; ///< Duplicate descriptor with close-on-exec set

    inline constexpr i32 FD_CLOEXEC = 1; ///< Close descriptor on exec

    inline constexpr i32 F_RDLCK = 0; ///< Shared/read lock
    inline constexpr i32 F_WRLCK = 1; ///< Exclusive/write lock
    inline constexpr i32 F_UNLCK = 2; ///< Unlock

    inline constexpr i32 SEEK_SET = 0; ///< Seek from beginning
    inline constexpr i32 SEEK_CUR = 1; ///< Seek from current position
    inline constexpr i32 SEEK_END = 2; ///< Seek from end

    inline constexpr i32 O_ACCMODE = 0003;
    inline constexpr i32 O_RDONLY = 00;
    inline constexpr i32 O_WRONLY = 01;
    inline constexpr i32 O_RDWR = 02;
    inline constexpr i32 O_CREAT = 0100;
    inline constexpr i32 O_DIRECTORY = 0200000;
    inline constexpr i32 O_EXCL = 0200;
    inline constexpr i32 O_NOCTTY = 0400;
    inline constexpr i32 O_NOFOLLOW = 0400000;
    inline constexpr i32 O_TRUNC = 01000;
    inline constexpr i32 O_TTY_INIT = 0;
    inline constexpr i32 O_APPEND = 02000;
    inline constexpr i32 O_DSYNC = 010000;
    inline constexpr i32 O_NONBLOCK = 04000;
    inline constexpr i32 O_NDELAY = O_NONBLOCK;
    inline constexpr i32 O_SYNC = 04010000;
    inline constexpr i32 O_RSYNC = O_SYNC;
    inline constexpr i32 O_FSYNC = O_SYNC;
    inline constexpr i32 O_EXEC = O_RDONLY;
    inline constexpr i32 O_SEARCH = O_RDONLY;
    inline constexpr i32 O_CLOEXEC  = 02000000;

    inline constexpr i32 AT_FDCWD = -100;
    inline constexpr i32 AT_EACCESS = 0x200;
    inline constexpr i32 AT_SYMLINK_NOFOLLOW = 0x100;
    inline constexpr i32 AT_SYMLINK_FOLLOW = 0x400;
    inline constexpr i32 AT_REMOVEDIR = 0x200;

    inline constexpr i32 POSIX_FADV_NORMAL = 0;
    inline constexpr i32 POSIX_FADV_RANDOM = 1;
    inline constexpr i32 POSIX_FADV_SEQUENTIAL = 2;
    inline constexpr i32 POSIX_FADV_WILLNEED = 3;
    inline constexpr i32 POSIX_FADV_DONTNEED = 4;
    inline constexpr i32 POSIX_FADV_NOREUSE = 5;

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

    inline constexpr u32 ISIG = 0000001;   ///< Enable signals
    inline constexpr u32 ICANON = 0000002;  ///< Canonical input (erase and kill processing)
    inline constexpr u32 XCASE = 0000004;   ///< Enable case mapping (non-POSIX)
    inline constexpr u32 ECHO = 0000010;    ///< Enable echo
    inline constexpr u32 ECHOE = 0000020;   ///< Echo erase character as error-correcting backspace
    inline constexpr u32 ECHOK = 0000040;   ///< Echo KILL
    inline constexpr u32 ECHONL = 0000100;  ///< Echo NL
    inline constexpr u32 NOFLSH = 0000200;  ///< Disable flush after i32errupt or quit
    inline constexpr u32 TOSTOP = 0000400;  ///< Send SIGTTOU for background output
    inline constexpr u32 ECHOCTL = 0001000; ///< If ECHO is also set, terminal special characters echoed as ^X
    inline constexpr u32 ECHOPRT = 0002000; ///< If ICANON and ECHO are set, characters pri32ed as they are erased
    inline constexpr u32 ECHOKE = 0004000;  ///< If ICANON is set, KILL echoed by erasing each character on the line
    inline constexpr u32 FLUSHO = 0010000;  ///< Output is being flushed (toggle by DISCARD character)
    inline constexpr u32 PENDIN = 0040000;  ///< All characters in the input queue are repri32ed when next character is read
    inline constexpr u32 IEXTEN = 0100000;  ///< Enable implementation-defined input processing
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
     * @namespace netinet
     * @brief Unix Internet protocol operations.
     */
    namespace netinet {
        /**
         * <netinet/tcp.h> constants
         */

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
}
