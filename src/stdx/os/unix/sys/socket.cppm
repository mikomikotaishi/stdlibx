/**
 * @file socket.cppm
 * @module stdx:os.unix.sys.socket
 * @brief Module file for Unix Berkley sockets operations.
 *
 * This file contains the implementation of the Unix Berkley sockets operations from the POSIX libraries,
 * located in <sys/socket.h>.
 */

module;

#ifdef __unix__
#include <sys/socket.h>
#endif

export module stdx:os.unix.sys.socket;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
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
