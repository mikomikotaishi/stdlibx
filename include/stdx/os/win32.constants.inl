#pragma once

/**
 * @namespace win32
 * @brief Windows operations.
 */
export namespace stdx::os::win32 {
    constexpr usize PATH_MAX = 260;
    constexpr usize MAX_PATH = 260;

    /// The UTF-8 code page, for WideCharToMultiByte and MultiByteToWideChar.
    constexpr u32 CP_UTF8 = 65001;
    /// Makes a WideCharToMultiByte conversion fail on ill-formed UTF-16 rather
    /// than substituting U+FFFD. Valid only with CP_UTF8 and CP_UTF7.
    constexpr u32 WC_ERR_INVALID_CHARS = 0x00000080;

    constexpr u8 MINCHAR = 0x80;
    constexpr u8 MAXCHAR = 0x7f;
    constexpr u16 MINSHORT = 0x8000;
    constexpr u16 MAXSHORT = 0x7fff;
    constexpr u32 MINLONG = 0x80000000;
    constexpr u32 MAXLONG = 0x7fffffff;
    constexpr u8 MAXBYTE = 0xff;
    constexpr u16 MAXWORD = 0xffff;
    constexpr u64 MAXDWORD = 0xffffffff;

    constexpr i64 ERROR_SUCCESS = 0;
    constexpr i64 ERROR_FILE_NOT_FOUND = 2;
    constexpr i64 ERROR_PATH_NOT_FOUND = 3;
    constexpr i64 ERROR_TOO_MANY_OPEN_FILES = 4;
    constexpr i64 ERROR_ACCESS_DENIED = 5;
    constexpr i64 ERROR_NO_MORE_FILES = 18;
    constexpr i64 ERROR_SHARING_VIOLATION = 32;
    constexpr i64 ERROR_FILE_EXISTS = 80;
    constexpr i64 ERROR_INSUFFICIENT_BUFFER = 122;
    constexpr i64 ERROR_ALREADY_EXISTS = 183;
    constexpr i64 ERROR_MORE_DATA = 234;

    constexpr u16 DLL_PROCESS_ATTACH = 1;
    constexpr u16 DLL_PROCESS_DETACH = 0;
    constexpr u16 DLL_THREAD_ATTACH = 2;
    constexpr u16 DLL_THREAD_DETACH = 3;

    constexpr i64 EXCEPTION_MAXIMUM_PARAMETERS = 15;
    constexpr i64 EXCEPTION_EXECUTE_HANDLER = 0x1;
    constexpr i64 EXCEPTION_CONTINUE_EXECUTION = 0xFFFFFFFF;
    constexpr i64 EXCEPTION_CONTINUE_SEARCH = 0x0;

    constexpr u64 EXCEPTION_ACCESS_VIOLATION = 0xC0000005ul;
    constexpr u64 EXCEPTION_DATATYPE_MISALIGNMENT = 0x80000002ul;
    constexpr u64 EXCEPTION_BREAKPOINT = 0x80000003ul;
    constexpr u64 EXCEPTION_SINGLE_STEP = 0x80000004ul;
    constexpr u64 EXCEPTION_ARRAY_BOUNDS_EXCEEDED = 0xC000008Cul;
    constexpr u64 EXCEPTION_FLT_DENORMAL_OPERAND = 0xC000008Dul;
    constexpr u64 EXCEPTION_FLT_DIVIDE_BY_ZERO = 0xC000008Eul;
    constexpr u64 EXCEPTION_FLT_INEXACT_RESULT = 0xC000008Ful;
    constexpr u64 EXCEPTION_FLT_INVALID_OPERATION = 0xC0000090ul;
    constexpr u64 EXCEPTION_FLT_OVERFLOW = 0xC0000091ul;
    constexpr u64 EXCEPTION_FLT_STACK_CHECK = 0xC0000092ul;
    constexpr u64 EXCEPTION_FLT_UNDERFLOW = 0xC0000093ul;
    constexpr u64 EXCEPTION_INT_DIVIDE_BY_ZERO = 0xC0000094ul;
    constexpr u64 EXCEPTION_INT_OVERFLOW = 0xC0000095ul;
    constexpr u64 EXCEPTION_PRIV_INSTRUCTION = 0xC0000096ul;
    constexpr u64 EXCEPTION_IN_PAGE_ERROR = 0xC0000006ul;
    constexpr u64 EXCEPTION_STACK_OVERFLOW = 0xC00000FDul;
    constexpr u64 EXCEPTION_ILLEGAL_INSTRUCTION = 0xC000001Dul;
    constexpr u64 EXCEPTION_NONCONTINUABLE_EXCEPTION = 0xC0000025ul;
    constexpr u64 EXCEPTION_INVALID_DISPOSITION = 0xC0000026ul;
    constexpr u64 EXCEPTION_GUARD_PAGE = 0x80000001ul;
    constexpr u64 EXCEPTION_INVALID_HANDLE = 0xC0000008ul;
    constexpr u64 EXCEPTION_POSSIBLE_DEADLOCK = 0xC0000194ul;
    constexpr u64 CONTROL_C_EXIT = 0xC000013Aul;

    constexpr u64 EXCEPTION_ASSERTION = 0xEF000001ul;

    constexpr i64 TH32CS_SNAPTHREAD = 0x00000004;
    constexpr i64 IMAGE_FILE_MACHINE_I386 = 0x014c;
    constexpr i64 IMAGE_FILE_MACHINE_AMD64 = 0x8664;
    constexpr i64 CONTEXT_AMD64 = 0x100000;
    constexpr i64 CONTEXT_CONTROL = CONTEXT_AMD64 | 0x1l;
    constexpr i64 CONTEXT_INTEGER = CONTEXT_AMD64 | 0x2l;
    constexpr i64 CONTEXT_SEGMENTS = CONTEXT_AMD64 | 0x4l;
    constexpr i64 CONTEXT_FLOATING_POINT = CONTEXT_AMD64 | 0x8l;
    constexpr i64 CONTEXT_DEBUG_REGISTERS = CONTEXT_AMD64 | 0x10l;
    constexpr i64 CONTEXT_FULL = CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_FLOATING_POINT;
    constexpr i64 CONTEXT_ALL = CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_SEGMENTS | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS;

    constexpr u64 SYMOPT_CASE_INSENSITIVE = 0x00000001;
    constexpr u64 SYMOPT_UNDNAME = 0x00000002;
    constexpr u64 SYMOPT_DEFERRED_LOADS = 0x00000004;
    constexpr u64 SYMOPT_NO_CPP = 0x00000008;
    constexpr u64 SYMOPT_LOAD_LINES = 0x00000010;
    constexpr u64 SYMOPT_OMAP_FIND_NEAREST = 0x00000020;
    constexpr u64 SYMOPT_LOAD_ANYTHING = 0x00000040;
    constexpr u64 SYMOPT_IGNORE_CVREC = 0x00000080;
    constexpr u64 SYMOPT_NO_UNQUALIFIED_LOADS = 0x00000100;
    constexpr u64 SYMOPT_FAIL_CRITICAL_ERRORS = 0x00000200;
    constexpr u64 SYMOPT_EXACT_SYMBOLS = 0x00000400;
    constexpr u64 SYMOPT_ALLOW_ABSOLUTE_SYMBOLS = 0x00000800;
    constexpr u64 SYMOPT_IGNORE_NT_SYMPATH = 0x00001000;
    constexpr u64 SYMOPT_INCLUDE_32BIT_MODULES = 0x00002000;
    constexpr u64 SYMOPT_PUBLICS_ONLY = 0x00004000;
    constexpr u64 SYMOPT_NO_PUBLICS = 0x00008000;
    constexpr u64 SYMOPT_AUTO_PUBLICS = 0x00010000;
    constexpr u64 SYMOPT_NO_IMAGE_SEARCH = 0x00020000;
    constexpr u64 SYMOPT_SECURE = 0x00040000;
    constexpr u64 SYMOPT_NO_PROMPTS = 0x00080000;
    constexpr u64 SYMOPT_OVERWRITE = 0x00100000;
    constexpr u64 SYMOPT_IGNORE_IMAGEDIR = 0x00200000;
    constexpr u64 SYMOPT_FLAT_DIRECTORY = 0x00400000;
    constexpr u64 SYMOPT_FAVOR_COMPRESSED = 0x00800000;
    constexpr u64 SYMOPT_ALLOW_ZERO_ADDRESS = 0x01000000;
    constexpr u64 SYMOPT_DISABLE_SYMSRV_AUTODETECT = 0x02000000;
    constexpr u64 SYMOPT_DEBUG = 0x80000000;

    constexpr i32 MAXIMUM_SUPPORTED_EXTENSION = 512;

    constexpr u64 WAIT_ABANDONED = 0x00000080l;
    constexpr u64 WAIT_OBJECT_0 = 0x00000000l;
    constexpr u64 WAIT_TIMEOUT = 0x00000102l;
    constexpr u64 WAIT_FAILED = 0xFFFFFFFFl;

    constexpr u32 INFINITE = 0xFFFFFFFF; ///< Infinite timeout for the wait functions.

    constexpr u32 STD_INPUT_HANDLE = 0xFFFFFFF6; ///< GetStdHandle's stdin device, (DWORD)-10.
    constexpr u32 STD_OUTPUT_HANDLE = 0xFFFFFFF5; ///< GetStdHandle's stdout device, (DWORD)-11.
    constexpr u32 STD_ERROR_HANDLE = 0xFFFFFFF4; ///< GetStdHandle's stderr device, (DWORD)-12.

    constexpr u32 HANDLE_FLAG_INHERIT = 0x00000001; ///< SetHandleInformation: child processes inherit the handle.
    constexpr u32 ENABLE_VIRTUAL_TERMINAL_PROCESSING = 0x0004; ///< SetConsoleMode: interpret ANSI escape sequences.
    constexpr u32 STARTF_USESTDHANDLES = 0x00000100; ///< STARTUPINFO.dwFlags: hStd* fields are valid.

    constexpr u32 GENERIC_READ = 0x80000000;
    constexpr u32 GENERIC_WRITE = 0x40000000;
    constexpr u32 FILE_SHARE_READ = 0x00000001;
    constexpr u32 FILE_SHARE_WRITE = 0x00000002;
    constexpr u32 OPEN_EXISTING = 3; ///< CreateFile disposition: the file must already exist.

    #ifdef _WIN32
    inline const Handle INVALID_HANDLE_VALUE = reinterpret_cast<Handle>(~static_cast<usize>(0));
    #endif

    /**
     * <winsock2.h> and <ws2tcpip.h> constants
     *
     * Winsock deliberately disagrees with POSIX on several of these values
     * (AF_INET6, SOL_SOCKET, the SO_* options, IPV6_V6ONLY), so they are kept in
     * this namespace rather than shared with the Unix constants.
     */

    constexpr i32 SOCKET_ERROR = -1; ///< Returned by a Winsock call that failed.

    constexpr i32 WINSOCK_VERSION_2_2 = 0x0202; ///< MAKEWORD(2, 2), for WSAStartup.

    constexpr i32 AF_UNSPEC = 0; ///< Unspecified address family.
    constexpr i32 AF_INET = 2; ///< IPv4.
    constexpr i32 AF_INET6 = 23; ///< IPv6 (not POSIX's 10).

    constexpr i32 SOCK_STREAM = 1; ///< Sequenced, reliable, connection-based byte stream.
    constexpr i32 SOCK_DGRAM = 2; ///< Connectionless, unreliable datagrams of fixed length.

    constexpr i32 IPPROTO_TCP = 6; ///< Transmission Control Protocol.
    constexpr i32 IPPROTO_UDP = 17; ///< User Datagram Protocol.
    constexpr i32 IPPROTO_IPV6 = 41; ///< Options for the IPv6 header itself.

    constexpr i32 SOL_SOCKET = 0xffff; ///< Options at the socket API level.

    constexpr i32 SO_REUSEADDR = 0x0004; ///< Allow reuse of a local address.
    constexpr i32 SO_KEEPALIVE = 0x0008; ///< Send keepalive probes on an idle connection.
    constexpr i32 SO_BROADCAST = 0x0020; ///< Permit sending to a broadcast address.
    constexpr i32 SO_LINGER = 0x0080; ///< Block on close until queued data is sent.
    constexpr i32 SO_SNDBUF = 0x1001; ///< Send-buffer size.
    constexpr i32 SO_RCVBUF = 0x1002; ///< Receive-buffer size.
    constexpr i32 SO_SNDTIMEO = 0x1005; ///< Send timeout, in milliseconds.
    constexpr i32 SO_RCVTIMEO = 0x1006; ///< Receive timeout, in milliseconds.
    constexpr i32 SO_ERROR = 0x1007; ///< Fetch and clear the pending error (read-only).
    constexpr i32 SO_TYPE = 0x1008; ///< The socket's type (read-only).

    constexpr i32 TCP_NODELAY = 0x0001; ///< Send segments as soon as possible (disable Nagle).

    constexpr i32 IPV6_V6ONLY = 27; ///< Refuse IPv4-mapped peers on an AF_INET6 socket (not POSIX's 26).

    constexpr i32 MSG_PEEK = 0x2; ///< Read without consuming.

    constexpr i32 SD_RECEIVE = 0; ///< Shut the receive half down.
    constexpr i32 SD_SEND = 1; ///< Shut the send half down.
    constexpr i32 SD_BOTH = 2; ///< Shut both halves down.

    constexpr i32 SOMAXCONN_HINT = 0x7fffffff; ///< Backlog ceiling accepted by listen().

    constexpr u32 FIONBIO = 0x8004667e; ///< ioctlsocket command that sets non-blocking mode.

    /**
     * WSAPoll event flags.
     *
     * Winsock disagrees with POSIX on every one of these too, and they live in a
     * SHORT rather than an int, so they are i16 here. POLLERR, POLLHUP and
     * POLLNVAL are reported in revents whether or not they were requested;
     * asking for them in events is an error.
     */

    constexpr i16 POLLRDNORM = 0x0100; ///< Normal data may be read.
    constexpr i16 POLLRDBAND = 0x0200; ///< Priority data may be read.
    constexpr i16 POLLIN = 0x0300; ///< POLLRDNORM | POLLRDBAND (not POSIX's 0x001).
    constexpr i16 POLLPRI = 0x0400; ///< Accepted and ignored by WSAPoll.
    constexpr i16 POLLWRNORM = 0x0010; ///< Writing now will not block.
    constexpr i16 POLLOUT = 0x0010; ///< POLLWRNORM (not POSIX's 0x004).
    constexpr i16 POLLWRBAND = 0x0020; ///< Priority data may be written.
    constexpr i16 POLLERR = 0x0001; ///< Error condition. Output only.
    constexpr i16 POLLHUP = 0x0002; ///< The connection has been closed. Output only.
    constexpr i16 POLLNVAL = 0x0004; ///< The socket is not valid. Output only.

    constexpr i32 AI_PASSIVE = 0x01; ///< Address is intended for bind().
    constexpr i32 AI_CANONNAME = 0x02; ///< Request the canonical name.
    constexpr i32 AI_NUMERICHOST = 0x04; ///< Reject host names; the node is a literal.
    constexpr i32 AI_NUMERICSERV = 0x08; ///< Reject service names; the service is a port number.
    constexpr i32 AI_ALL = 0x0100; ///< Return both IPv4-mapped and IPv6 addresses.
    constexpr i32 AI_ADDRCONFIG = 0x0400; ///< Only return families the host is configured for.
    constexpr i32 AI_V4MAPPED = 0x0800; ///< Map IPv4 results into IPv6 when no IPv6 exists.

    constexpr i32 NI_NOFQDN = 0x01; ///< Return only the hostname part of an FQDN.
    constexpr i32 NI_NUMERICHOST = 0x02; ///< Return the address in numeric form.
    constexpr i32 NI_NAMEREQD = 0x04; ///< Fail rather than fall back to a numeric form.
    constexpr i32 NI_NUMERICSERV = 0x08; ///< Return the service as a port number.
    constexpr i32 NI_DGRAM = 0x10; ///< Look the service up for a datagram, not a stream.

    constexpr usize NI_MAXHOST = 1025; ///< Buffer size that always holds a host name.
    constexpr usize NI_MAXSERV = 32; ///< Buffer size that always holds a service name.

    constexpr i32 WSAEINTR = 10004; ///< Interrupted call.
    constexpr i32 WSAEWOULDBLOCK = 10035; ///< Operation would block.
    constexpr i32 WSAEINPROGRESS = 10036; ///< Operation now in progress.
    constexpr i32 WSAEALREADY = 10037; ///< Operation already in progress.
    constexpr i32 WSAENOTSOCK = 10038; ///< The descriptor is not a socket.
    constexpr i32 WSAEMSGSIZE = 10040; ///< Message too long.
    constexpr i32 WSAEADDRINUSE = 10048; ///< Address already in use.
    constexpr i32 WSAEADDRNOTAVAIL = 10049; ///< Cannot assign requested address.
    constexpr i32 WSAENETUNREACH = 10051; ///< Network is unreachable.
    constexpr i32 WSAECONNABORTED = 10053; ///< Connection aborted locally.
    constexpr i32 WSAECONNRESET = 10054; ///< Connection reset by peer.
    constexpr i32 WSAEISCONN = 10056; ///< Socket is already connected.
    constexpr i32 WSAENOTCONN = 10057; ///< Socket is not connected.
    constexpr i32 WSAETIMEDOUT = 10060; ///< Connection timed out.
    constexpr i32 WSAECONNREFUSED = 10061; ///< Connection refused.
    constexpr i32 WSAEHOSTUNREACH = 10065; ///< No route to host.
    constexpr i32 WSATYPE_NOT_FOUND = 10109; ///< The service is unknown for this socket type.
    constexpr i32 WSAHOST_NOT_FOUND = 11001; ///< The host name resolves to nothing.
    constexpr i32 WSATRY_AGAIN = 11002; ///< Temporary resolution failure.
    constexpr i32 WSANO_RECOVERY = 11003; ///< Non-recoverable resolution failure.
    constexpr i32 WSANO_DATA = 11004; ///< The host name is valid but has no address.
}
