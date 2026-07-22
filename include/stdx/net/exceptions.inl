#pragma once

using stdx::io::IOException;
using stdx::io::InterruptedIOException;

/**
 * @namespace stdx::net
 * @brief Experimental standard library networking operations.
 */
export namespace stdx::net {
    /**
     * @class UnknownHostException
     * @brief Exception class for handling unknown host errors.
     * @extends IOException
     * 
     * The UnknownHostException class is used to represent an exception that occurs when a host cannot be resolved.
     */
    class UnknownHostException: public IOException {
    public:
        using IOException::IOException;
    };

    /**
     * @class UnknownServiceException
     * @brief Exception class for handling unknown service errors.
     * @extends IOException
     * 
     * The UnknownServiceException class is used to represent an exception that occurs when a service cannot be resolved.
     */
    class UnknownServiceException: public IOException {
    public:
        using IOException::IOException;
    };

    /**
     * @class ProtocolException
     * @brief Exception class for handling protocol errors.
     * @extends IOException
     * 
     * The ProtocolException class is used to represent an exception that occurs when a protocol error happens.
     */
    class ProtocolException: public IOException {
    public:
        using IOException::IOException;
    };
}
