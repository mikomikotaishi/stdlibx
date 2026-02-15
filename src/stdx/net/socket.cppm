/**
 * @file socket.cppm
 * @module stdx:net.socket
 * @brief Module file for (experimental) standard library networking socket operations.
 *
 * This file contains the implementation of the (experimental) networking socket operations in the standard library.
 */


module;

#if __has_include(<experimental/socket>)
#include <experimental/socket>
#endif

export module stdx:net.socket;

/**
 * @namespace stdx::net
 * @brief Wrapper namespace for (experimental) standard library networking operations.
 */
export namespace stdx::net {
    #if __has_include(<experimental/socket>)
    using SocketBase = std::experimental::net::socket_base;

    template <typename Protocol>
    using BasicSocket = std::experimental::net::basic_socket<Protocol>;

    template <typename Protocol>
    using BasicDatagramSocket = std::experimental::net::basic_datagram_socket<Protocol>;

    template <typename Protocol>
    using BasicStreamSocket = std::experimental::net::basic_stream_socket<Protocol>;

    template <typename Protocol>
    using BasicSocketAcceptor = std::experimental::net::basic_socket_acceptor<Protocol>;

    template <typename Protocol>
    using BasicSocketStreamBuffer = std::experimental::net::basic_socket_streambuf<Protocol>;

    template <typename Protocol>
    using BasicSocketInputOutputStream = std::experimental::net::basic_socket_iostream<Protocol>;
    #endif
}
