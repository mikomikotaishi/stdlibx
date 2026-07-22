#pragma once

using namespace stdx::os;

/**
 * @namespace stdx::net
 * @brief Experimental standard library networking operations.
 */
export namespace stdx::net {
    /**
     * @class SocketException
     * @brief Exception class for handling socket errors.
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
     * @extends SocketException
     * 
     * The NoRouteToHostException class is used to represent an exception that occurs when there is no route to a host.
     */
    class NoRouteToHostException: public SocketException {
    public:
        using SocketException::SocketException;
    };

    class [[nodiscard]] Socket {
    public:
        #ifdef _WIN32
        using NativeHandle = win32::WinSocket;
        #else
        using NativeHandle = int;
        #endif

        enum class Type {
            STREAM, ///< TCP
            DATAGRAM, ///< UDP
        };
    private:
    public:
        Socket() = default;
    };
}
