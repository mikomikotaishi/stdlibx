#pragma once

using stdx::io::IOException;
using stdx::io::InterruptedIOException;

/**
 * @namespace stdx::net
 * @brief Wrapper namespace for (experimental) standard library networking operations.
 */
export namespace stdx::net {
    /**
     * @class UnknownHostException
     * @brief Exception class for handling unknown host errors.
     *
     * @extends IOException
     * 
     * The UnknownHostException class is used to represent an exception that occurs when a host cannot be resolved.
     */
    class UnknownHostException: public IOException {
    public:
        /**
        * @brief Constructor that initialises the exception with a message.
        * @param msg The exception message.
        */
        explicit UnknownHostException(const String& msg = ""):
            IOException(msg) {}
    };

    /**
     * @class UnknownServiceException
     * @brief Exception class for handling unknown service errors.
     *
     * @extends IOException
     * 
     * The UnknownServiceException class is used to represent an exception that occurs when a service cannot be resolved.
     */
    class UnknownServiceException: public IOException {
    public:
        /**
        * @brief Constructor that initialises the exception with a message.
        * @param msg The exception message.
        */
        explicit UnknownServiceException(const String& msg = ""):
            IOException(msg) {}
    };

    /**
     * @class ProtocolException
     * @brief Exception class for handling protocol errors.
     *
     * @extends IOException
     * 
     * The ProtocolException class is used to represent an exception that occurs when a protocol error happens.
     */
    class ProtocolException: public IOException {
    public:
        /**
        * @brief Constructor that initialises the exception with a message.
        * @param msg The exception message.
        */
        explicit ProtocolException(const String& msg = ""):
            IOException(msg) {}
    };

    /**
     * @namespace ip
     * @brief Namespace for operations IP addresses.
     */
    namespace ip {
        #if __has_include(<experimental/internet>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
        using IPv4Address = std::experimental::net::ip::address_v4;
        using IPv6Address = std::experimental::net::ip::address_v6;
        using Address = std::experimental::net::ip::address;
        using IPv4NetworkAddress = std::experimental::net::ip::network_v4;
        using IPv6NetworkAddress = std::experimental::net::ip::network_v6;
        using ResolverBase = std::experimental::net::ip::resolver_base;
        using Udp = std::experimental::net::ip::udp;
        using Tcp = std::experimental::net::ip::tcp;
        using TcpEndpoint = std::experimental::net::ip::tcp::endpoint;
        using TcpResolver = std::experimental::net::ip::tcp::resolver;
        using TcpSocket = std::experimental::net::ip::tcp::socket;
        using TcpAcceptor = std::experimental::net::ip::tcp::acceptor;
        using TcpInputOutputStream = std::experimental::net::ip::tcp::iostream;
        using UdpEndpoint = std::experimental::net::ip::udp::endpoint;
        using UdpResolver = std::experimental::net::ip::udp::resolver;
        using UdpSocket = std::experimental::net::ip::udp::socket;

        template <typename InetProtocol>
        using BasicEndpoint = std::experimental::net::ip::basic_endpoint<InetProtocol>;

        template <typename InetProtocol>
        using BasicResolverResults = std::experimental::net::ip::basic_resolver_results<InetProtocol>;

        using V6Only = std::experimental::net::ip::v6_only;
        #endif

        /**
         * @namespace unicast
         * @brief Namespace for operations on unicast.
         */
        namespace unicast {
            #if __has_include(<experimental/internet>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
            using Hops = std::experimental::net::ip::unicast::hops;
            #endif
        }

        /**
         * @namespace multicast
         * @brief Namespace for operations on multicast.
         */
        namespace multicast {
            #if __has_include(<experimental/internet>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
            using JoinGroup = std::experimental::net::ip::multicast::join_group;
            using LeaveGroup = std::experimental::net::ip::multicast::leave_group;
            using OutboundInterface = std::experimental::net::ip::multicast::outbound_interface;
            using Hops = std::experimental::net::ip::multicast::hops;
            using EnableLoopback = std::experimental::net::ip::multicast::enable_loopback;
            #endif
        }
    }
}
