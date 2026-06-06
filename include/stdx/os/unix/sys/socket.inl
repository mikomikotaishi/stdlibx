#pragma once

/**
 * @namespace stdx::os::unix::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #if defined(__unix__) && __has_include(<sys/socket.h>)
    using MultiMessageHeader = ::mmsghdr;

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
    using ::sendmmsg;
    using ::recvmsg;
    using ::recvmmsg;
    using ::getsockopt;
    using ::setsockopt;
    using ::listen;
    using ::accept;
    using ::accept4;
    using ::shutdown;
    using ::sockatmark;
    using ::isfdtype;
    #endif
}
