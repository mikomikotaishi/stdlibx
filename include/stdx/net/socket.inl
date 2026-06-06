#pragma once

using stdx::io::IOException;
using stdx::io::InterruptedIOException;

/**
 * @namespace stdx::net
 * @brief Experimental standard library networking operations.
 */
export namespace stdx::net {
    /**
     * @class SocketException
     * @brief Exception class for handling socket errors.
     *
     * @extends IOException
     * 
     * The SocketException class is used to represent an exception that occurs when a socket operation fails.
     */
    class SocketException: public IOException {
    public:
        using IOException::IOException;
    };

    /**
     * @class SocketException
     * @brief Exception class for handling socket timeout errors.
     *
     * @extends InterruptedIOException
     * 
     * The SocketTimeoutException class is used to represent an exception that occurs when a socket operation times out.
     */
    class SocketTimeoutException: public InterruptedIOException {
    public:
        using InterruptedIOException::InterruptedIOException;
    };

    /**
     * @class PortUnreachableException
     * @brief Exception class for handling port unreachable errors.
     *
     * @extends SocketException
     * 
     * The PortUnreachableException class is used to represent an exception that occurs when a port is unreachable.
     */
    class PortUnreachableException: public SocketException {
    public:
        using SocketException::SocketException;
    };

    /**
     * @class BindException
     * @brief Exception class for handling bind errors.
     *
     * @extends SocketException
     * 
     * The BindException class is used to represent an exception that occurs when a socket cannot be bound to a local address and port.
     */
    class BindException: public SocketException {
    public:
        using SocketException::SocketException;
    };

    /**
     * @class NoRouteToHostException
     * @brief Exception class for handling no route to host errors.
     *
     * @extends SocketException
     * 
     * The NoRouteToHostException class is used to represent an exception that occurs when there is no route to a host.
     */
    class NoRouteToHostException: public SocketException {
    public:
        using SocketException::SocketException;
    };

    #if __has_include(<experimental/socket>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
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
