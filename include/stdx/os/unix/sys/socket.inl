#pragma once

/**
 * @internal
 * @namespace stdx::os::unix::sys::captured
 * @brief The numeric values of the <sys/socket.h> macros, read where they exist.
 *
 * A macro does not cross `import :os`, which is why os/unix.constants.inl held
 * hand-copied literals and why those literals were Linux's. This namespace is
 * the fix: os.cppm includes the real <sys/socket.h> in its global module
 * fragment, so each value can be read here into a `constexpr` variable, and a
 * variable does cross the boundary. unix.constants.inl then declares the POSIX
 * names in terms of these instead of in terms of numbers someone typed, which
 * makes them right on every platform rather than right on one.
 *
 * Nothing outside unix.constants.inl should name these. There is no `#undef`
 * here, unlike unix/threads.inl, because that file redeclares the POSIX name in
 * this same translation unit and so has to clear the macro first; suffixing
 * sidesteps the need, and with it the risk of clearing a macro a later wrapper in
 * this TU still wants.
 *
 * Anything absent on a platform is guarded rather than defaulted. A socket
 * option that silently becomes 0 is a call to setsockopt with the wrong option,
 * which the kernel is under no obligation to reject.
 */
namespace stdx::os::unix::sys::captured {
    #if (defined(__unix__) || defined(__APPLE__)) && __has_include(<sys/socket.h>)
    constexpr auto AF_UNSPEC_VALUE = AF_UNSPEC;
    constexpr auto AF_UNIX_VALUE = AF_UNIX;
    constexpr auto AF_INET_VALUE = AF_INET;
    constexpr auto AF_INET6_VALUE = AF_INET6;

    constexpr auto SOCK_STREAM_VALUE = SOCK_STREAM;
    constexpr auto SOCK_DGRAM_VALUE = SOCK_DGRAM;
    constexpr auto SOCK_RAW_VALUE = SOCK_RAW;
    constexpr auto SOCK_SEQPACKET_VALUE = SOCK_SEQPACKET;

    constexpr auto SOL_SOCKET_VALUE = SOL_SOCKET;

    constexpr auto SO_DEBUG_VALUE = SO_DEBUG;
    constexpr auto SO_REUSEADDR_VALUE = SO_REUSEADDR;
    constexpr auto SO_TYPE_VALUE = SO_TYPE;
    constexpr auto SO_ERROR_VALUE = SO_ERROR;
    constexpr auto SO_DONTROUTE_VALUE = SO_DONTROUTE;
    constexpr auto SO_BROADCAST_VALUE = SO_BROADCAST;
    constexpr auto SO_SNDBUF_VALUE = SO_SNDBUF;
    constexpr auto SO_RCVBUF_VALUE = SO_RCVBUF;
    constexpr auto SO_KEEPALIVE_VALUE = SO_KEEPALIVE;
    constexpr auto SO_OOBINLINE_VALUE = SO_OOBINLINE;
    constexpr auto SO_LINGER_VALUE = SO_LINGER;
    constexpr auto SO_REUSEPORT_VALUE = SO_REUSEPORT;
    constexpr auto SO_RCVLOWAT_VALUE = SO_RCVLOWAT;
    constexpr auto SO_SNDLOWAT_VALUE = SO_SNDLOWAT;
    constexpr auto SO_RCVTIMEO_VALUE = SO_RCVTIMEO;
    constexpr auto SO_SNDTIMEO_VALUE = SO_SNDTIMEO;
    constexpr auto SO_ACCEPTCONN_VALUE = SO_ACCEPTCONN;

    constexpr auto MSG_OOB_VALUE = MSG_OOB;
    constexpr auto MSG_PEEK_VALUE = MSG_PEEK;
    constexpr auto MSG_DONTROUTE_VALUE = MSG_DONTROUTE;
    constexpr auto MSG_TRUNC_VALUE = MSG_TRUNC;
    constexpr auto MSG_DONTWAIT_VALUE = MSG_DONTWAIT;
    constexpr auto MSG_EOR_VALUE = MSG_EOR;
    constexpr auto MSG_WAITALL_VALUE = MSG_WAITALL;

    constexpr auto SHUT_RD_VALUE = SHUT_RD;
    constexpr auto SHUT_WR_VALUE = SHUT_WR;
    constexpr auto SHUT_RDWR_VALUE = SHUT_RDWR;

    constexpr auto SOMAXCONN_VALUE = SOMAXCONN;

    /**
     * @brief Suppresses SIGPIPE on a send to a peer that has closed.
     *
     * Both spellings exist and they are not interchangeable. Linux offers only
     * the send flag; Darwin offers the flag (behind __DARWIN_C_LEVEL >= 200809L)
     * and, since long before it, the socket option. The values do not agree, and
     * the disagreement is silent: Linux's MSG_NOSIGNAL is 0x4000, which on Darwin
     * is MSG_RCVMORE - a receive-side flag that send() accepts and ignores. That
     * is precisely why these are read from the header rather than copied.
     *
     * The guards are on the platform rather than on `#ifdef MSG_NOSIGNAL`, even
     * though the latter reads better, because unix.constants.inl has to declare
     * the matching name under the same condition and cannot test for a macro it
     * never sees. Two conditions that must agree are better spelled identically
     * than spelled well.
     */
    #if defined(__linux__) || defined(__APPLE__)
    constexpr auto MSG_NOSIGNAL_VALUE = MSG_NOSIGNAL;
    #endif
    #ifdef __APPLE__
    constexpr auto SO_NOSIGPIPE_VALUE = SO_NOSIGPIPE;
    #endif

    /// Linux extensions; absent on Darwin and the BSDs.
    #ifdef __linux__
    constexpr auto AF_PACKET_VALUE = AF_PACKET;
    constexpr auto SOCK_NONBLOCK_VALUE = SOCK_NONBLOCK;
    constexpr auto SOCK_CLOEXEC_VALUE = SOCK_CLOEXEC;
    constexpr auto SO_PROTOCOL_VALUE = SO_PROTOCOL;
    constexpr auto SO_DOMAIN_VALUE = SO_DOMAIN;
    #endif
    #endif
}

/**
 * @namespace stdx::os::unix::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #if (defined(__unix__) || defined(__APPLE__)) && __has_include(<sys/socket.h>)
    using SocketAddress = ::sockaddr;
    using SocketAddressStorage = ::sockaddr_storage;
    using SocketLength = ::socklen_t;

    using Linger = ::linger;

    using MessageHeader = ::msghdr;

    using ::socket;
    using ::socketpair;
    using ::bind;
    using ::getsockname;
    using ::connect;
    using ::getpeername;
    using ::send;
    using ::recv;
    using ::sendto;
    using ::recvfrom;
    using ::sendmsg;
    using ::recvmsg;
    using ::getsockopt;
    using ::setsockopt;
    using ::listen;
    using ::accept;
    using ::shutdown;
    using ::sockatmark;
    using ::isfdtype;

    /**
     * @brief Linux-only calls, guarded so the wrapper itself stays portable.
     *
     * These were previously re-exported unconditionally, which meant this header
     * could not compile anywhere but Linux however correct the constants were.
     * Nothing in stdx::net uses them; the scatter/gather and batched forms have
     * no Darwin equivalent, and accept4's flags are folded into a separate
     * fcntl there.
     */
    #ifdef __linux__
    using MultiMessageHeader = ::mmsghdr;

    using ::sendmmsg;
    using ::recvmmsg;
    using ::accept4;
    #endif
    #endif
}
