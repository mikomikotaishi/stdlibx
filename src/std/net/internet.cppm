/**
 * @file internet.cppm
 * @module std:net.internet
 * @brief Module file for (experimental) standard library networking internet operations.
 *
 * This file contains the implementation of the (experimental) networking internet operations in the standard library.
 */


module;

#if __has_include(<experimental/internet>)
#include <experimental/internet>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:net.internet;
#else
export module stdlib:net.internet;
#endif

/**
 * @namespace std::net
 * @brief Wrapper namespace for (experimental) standard library networking operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::net {
#else 
export namespace stdlib::net {
#endif
    /**
     * @namespace ip
     * @brief Namespace for operations IP addresses.
     */
    namespace ip {
        #if __has_include(<experimental/internet>)
        using AddressV4 = std::experimental::net::ip::address_v4;
        using AddressV6 = std::experimental::net::ip::address_v6;
        using Address = std::experimental::net::ip::address;
        using NetworkV4 = std::experimental::net::ip::network_v4;
        using NetworkV6 = std::experimental::net::ip::network_v6;
        using ResolverBase = std::experimental::net::ip::resolver_base;
        using UDP = std::experimental::net::ip::udp;
        using TCP = std::experimental::net::ip::tcp;

        template <typename InternetProtocol>
        using BasicEndpoint = std::experimental::net::ip::basic_endpoint<InternetProtocol>;

        template <typename InternetProtocol>
        using BasicResolverResults = std::experimental::net::ip::basic_resolver_results<InternetProtocol>;

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
