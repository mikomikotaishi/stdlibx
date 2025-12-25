/**
 * @file errno.cppm
 * @module stdx:os.linux.asmgeneric.errno
 * @brief Module file for Linux error number operations.
 *
 * This file contains the implementation of the Linux error number  from the POSIX libraries,
 * located in <asm-generic/errno.h>.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.asmgeneric.errno;
#else
export module stdlibx:os.linux.asmgeneric.errno;
#endif

/**
 * @namespace stdx::os::linux::arpa
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::asmgeneric {
#else 
export namespace stdlibx::os::linux::asmgeneric {
#endif
    inline constexpr int EPERM = 1; ///< Operation not permitted
    inline constexpr int ENOENT = 2; ///< No such file or directory
    inline constexpr int ESRCH = 3; ///< No such process
    inline constexpr int EINTR = 4; ///< Interrupted system call
    inline constexpr int EIO = 5; ///< I/O error
    inline constexpr int ENXIO = 6; ///< No such device or address
    inline constexpr int E2BIG = 7; ///< Argument list too long
    inline constexpr int ENOEXEC = 8; ///< Exec format error
    inline constexpr int EBADF = 9; ///< Bad file number
    inline constexpr int ECHILD = 10; ///< No child processes
    inline constexpr int EAGAIN = 11; ///< Try again
    inline constexpr int ENOMEM = 12; ///< Out of memory
    inline constexpr int EACCES = 13; ///< Permission denied
    inline constexpr int EFAULT = 14; ///< Bad address
    inline constexpr int ENOTBLK = 15; ///< Block device required
    inline constexpr int EBUSY = 16; ///< Device or resource busy
    inline constexpr int EEXIST = 17; ///< File exists
    inline constexpr int EXDEV = 18; ///< Cross-device link
    inline constexpr int ENODEV = 19; ///< No such device
    inline constexpr int ENOTDIR = 20; ///< Not a directory
    inline constexpr int EISDIR = 21; ///< Is a directory
    inline constexpr int EINVAL = 22; ///< Invalid argument
    inline constexpr int ENFILE = 23; ///< File table overflow
    inline constexpr int EMFILE = 24; ///< Too many open files
    inline constexpr int ENOTTY = 25; ///< Not a typewriter
    inline constexpr int ETXTBSY = 26; ///< Text file busy
    inline constexpr int EFBIG = 27; ///< File too large
    inline constexpr int ENOSPC = 28; ///< No space left on device
    inline constexpr int ESPIPE = 29; ///< Illegal seek
    inline constexpr int EROFS = 30; ///< Read-only file system
    inline constexpr int EMLINK = 31; ///< Too many links
    inline constexpr int EPIPE = 32; ///< Broken pipe
    inline constexpr int EDOM = 33; ///< Math argument out of domain of func
    inline constexpr int ERANGE = 34; ///< Math result not representable
    inline constexpr int EDEADLK = 35; ///< Resource deadlock would occur
    inline constexpr int ENAMETOOLONG = 36; ///< File name too long
    inline constexpr int ENOLCK = 37; ///< No record locks available
    inline constexpr int ENOSYS = 38; ///< Invalid system call number
    inline constexpr int ENOTEMPTY = 39; ///< Directory not empty
    inline constexpr int ELOOP = 40; ///< Too many symbolic links encountered
    inline constexpr int EWOULDBLOCK = EAGAIN; ///< Operation would block
    inline constexpr int ENOMSG = 42; ///< No message of desired type
    inline constexpr int EIDRM = 43; ///< Identifier removed
    inline constexpr int ECHRNG = 44; ///< Channel number out of range
    inline constexpr int EL2NSYNC = 45; ///< Level 2 not synchronized
    inline constexpr int EL3HLT = 46; ///< Level 3 halted
    inline constexpr int EL3RST = 47; ///< Level 3 reset
    inline constexpr int ELNRNG = 48; ///< Link number out of range
    inline constexpr int EUNATCH = 49; ///< Protocol driver not attached
    inline constexpr int ENOCSI = 50; ///< No CSI structure available
    inline constexpr int EL2HLT = 51; ///< Level 2 halted
    inline constexpr int EBADE = 52; ///< Invalid exchange
    inline constexpr int EBADR = 53; ///< Invalid request descriptor
    inline constexpr int EXFULL = 54; ///< Exchange full
    inline constexpr int ENOANO = 55; ///< No anode
    inline constexpr int EBADRQC = 56; ///< Invalid request code
    inline constexpr int EBADSLT = 57; ///< Invalid slot
    inline constexpr int EDEADLOCK = EDEADLK; ///< Alias for EDEADLK
    inline constexpr int EBFONT = 59; ///< Bad font file format
    inline constexpr int ENOSTR = 60; ///< Device not a stream
    inline constexpr int ENODATA = 61; ///< No data available
    inline constexpr int ETIME = 62; ///< Timer expired
    inline constexpr int ENOSR = 63; ///< Out of streams resources
    inline constexpr int ENONET = 64; ///< Machine is not on the network
    inline constexpr int ENOPKG = 65; ///< Package not installed
    inline constexpr int EREMOTE = 66; ///< Object is remote
    inline constexpr int ENOLINK = 67; ///< Link has been severed
    inline constexpr int EADV = 68; ///< Advertise error
    inline constexpr int ESRMNT = 69; ///< Srmount error
    inline constexpr int ECOMM = 70; ///< Communication error on send
    inline constexpr int EPROTO = 71; ///< Protocol error
    inline constexpr int EMULTIHOP = 72; ///< Multihop attempted
    inline constexpr int EDOTDOT = 73; ///< RFS specific error
    inline constexpr int EBADMSG = 74; ///< Not a data message
    inline constexpr int EOVERFLOW = 75; ///< Value too large for defined data type
    inline constexpr int ENOTUNIQ = 76; ///< Name not unique on network
    inline constexpr int EBADFD = 77; ///< File descriptor in bad state
    inline constexpr int EREMCHG = 78; ///< Remote address changed
    inline constexpr int ELIBACC = 79; ///< Cannot access a needed shared library
    inline constexpr int ELIBBAD = 80; ///< Accessing a corrupted shared library
    inline constexpr int ELIBSCN = 81; ///< .lib section in a.out corrupted
    inline constexpr int ELIBMAX = 82; ///< Too many shared libraries
    inline constexpr int ELIBEXEC = 83; ///< Cannot exec a shared library directly
    inline constexpr int EILSEQ = 84; ///< Illegal byte sequence
    inline constexpr int ERESTART = 85; ///< Interrupted system call should be restarted
    inline constexpr int ESTRPIPE = 86; ///< Streams pipe error
    inline constexpr int EUSERS = 87; ///< Too many users
    inline constexpr int ENOTSOCK = 88; ///< Socket operation on non-socket
    inline constexpr int EDESTADDRREQ = 89; ///< Destination address required
    inline constexpr int EMSGSIZE = 90; ///< Message too long
    inline constexpr int EPROTOTYPE = 91; ///< Protocol wrong type for socket
    inline constexpr int ENOPROTOOPT = 92; ///< Protocol not available
    inline constexpr int EPROTONOSUPPORT = 93; ///< Protocol not supported
    inline constexpr int ESOCKTNOSUPPORT = 94; ///< Socket type not supported
    inline constexpr int EOPNOTSUPP = 95; ///< Operation not supported on transport endpoint
    inline constexpr int EPFNOSUPPORT = 96; ///< Protocol family not supported
    inline constexpr int EAFNOSUPPORT = 97; ///< Address family not supported by protocol
    inline constexpr int EADDRINUSE = 98; ///< Address already in use
    inline constexpr int EADDRNOTAVAIL = 99; ///< Cannot assign requested address
    inline constexpr int ENETDOWN = 100; ///< Network is down
    inline constexpr int ENETUNREACH = 101; ///< Network is unreachable
    inline constexpr int ENETRESET = 102; ///< Network dropped connection because of reset
    inline constexpr int ECONNABORTED = 103; ///< Software caused connection abort
    inline constexpr int ECONNRESET = 104; ///< Connection reset by peer
    inline constexpr int ENOBUFS = 105; ///< No buffer space available
    inline constexpr int EISCONN = 106; ///< Transport endpoint is already connected
    inline constexpr int ENOTCONN = 107; ///< Transport endpoint is not connected
    inline constexpr int ESHUTDOWN = 108; ///< Cannot send after transport endpoint shutdown
    inline constexpr int ETOOMANYREFS = 109; ///< Too many references: cannot splice
    inline constexpr int ETIMEDOUT = 110; ///< Connection timed out
    inline constexpr int ECONNREFUSED = 111; ///< Connection refused
    inline constexpr int EHOSTDOWN = 112; ///< Host is down
    inline constexpr int EHOSTUNREACH = 113; ///< No route to host
    inline constexpr int EALREADY = 114; ///< Operation already in progress
    inline constexpr int EINPROGRESS = 115; ///< Operation now in progress
    inline constexpr int ESTALE = 116; ///< Stale file handle
    inline constexpr int EUCLEAN = 117; ///< Structure needs cleaning
    inline constexpr int ENOTNAM = 118; ///< Not a XENIX named type file
    inline constexpr int ENAVAIL = 119; ///< No XENIX semaphores available
    inline constexpr int EISNAM = 120; ///< Is a named type file
    inline constexpr int EREMOTEIO = 121; ///< Remote I/O error
    inline constexpr int EDQUOT = 122; ///< Quota exceeded
    inline constexpr int ENOMEDIUM = 123; ///< No medium found
    inline constexpr int EMEDIUMTYPE = 124; ///< Wrong medium type
    inline constexpr int ECANCELED = 125; ///< Operation Canceled
    inline constexpr int ENOKEY = 126; ///< Required key not available
    inline constexpr int EKEYEXPIRED = 127; ///< Key has expired
    inline constexpr int EKEYREVOKED = 128; ///< Key has been revoked
    inline constexpr int EKEYREJECTED = 129; ///< Key was rejected by service
    inline constexpr int EOWNERDEAD = 130; ///< Owner died
    inline constexpr int ENOTRECOVERABLE = 131; ///< State not recoverable
    inline constexpr int ERFKILL = 132; ///< Operation not possible due to RF-kill
    inline constexpr int EHWPOISON = 133; ///< Memory page has hardware error
}
