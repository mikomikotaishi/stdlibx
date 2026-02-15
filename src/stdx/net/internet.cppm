/**
 * @file internet.cppm
 * @module stdx:net.internet
 * @brief Module file for (experimental) standard library networking internet operations.
 *
 * This file contains the implementation of the (experimental) networking internet operations in the standard library.
 */

module;

#if __has_include(<experimental/internet>)
#include <experimental/internet>
#endif

export module stdx:net.internet;

/**
 * @namespace stdx::net
 * @brief Wrapper namespace for (experimental) standard library networking operations.
 */
export namespace stdx::net {
    /**
     * @namespace ip
     * @brief Namespace for operations IP addresses.
     */
    namespace ip {
        #if __has_include(<experimental/internet>)
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
            #if __has_include(<experimental/internet>)
            using Hops = std::experimental::net::ip::unicast::hops;
            #endif
        }

        /**
         * @namespace multicast
         * @brief Namespace for operations on multicast.
         */
        namespace multicast {
            #if __has_include(<experimental/internet>)
            using JoinGroup = std::experimental::net::ip::multicast::join_group;
            using LeaveGroup = std::experimental::net::ip::multicast::leave_group;
            using OutboundInterface = std::experimental::net::ip::multicast::outbound_interface;
            using Hops = std::experimental::net::ip::multicast::hops;
            using EnableLoopback = std::experimental::net::ip::multicast::enable_loopback;
            #endif
        }
    }
}
