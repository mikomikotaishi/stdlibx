#pragma once

using namespace stdx::os;

/**
 * @namespace stdx::net
 * @brief Platform glue for name resolution.
 */
namespace stdx::net {
    #ifdef _WIN32
    using AddressInfo = win32::WinAddrInfo;
    #else
    using AddressInfo = unix::AddressInfo;
    #endif

    /**
     * @internal
     * @class AddressInfoList
     * @brief Owns the linked list getaddrinfo hands back, and frees it exactly once.
     */
    class [[nodiscard]] AddressInfoList {
    private:
        AddressInfo* _head = nullptr; ///< The head of the list, or nullptr if empty.
    public:
        AddressInfoList() noexcept = default;

        AddressInfoList(const AddressInfoList&) = delete;

        AddressInfoList& operator=(const AddressInfoList&) = delete;

        ~AddressInfoList() {
            if (_head == nullptr) {
                return;
            }
            #ifdef _WIN32
            win32::freeaddrinfo(_head);
            #else
            unix::freeaddrinfo(_head);
            #endif
        }

        /**
         * @brief The address of the head pointer, for getaddrinfo to write into.
         * @return A pointer to the owned head pointer.
         */
        [[nodiscard]]
        AddressInfo** address() noexcept {
            return &_head;
        }

        /**
         * @brief The first entry of the list.
         * @return The head, or nullptr if the list is empty.
         */
        [[nodiscard]]
        const AddressInfo* head() const noexcept {
            return _head;
        }
    };

    /**
     * @internal
     * @brief The default service name for a URI scheme.
     * @param scheme The scheme, without the trailing ':'.
     * @return The service to look up, which for the schemes the OS knows is the scheme itself.
     */
    [[nodiscard]]
    inline StringView service_for_scheme(StringView scheme) noexcept {
        if (scheme == "wss") {
            return "https";
        }
        if (scheme == "ws") {
            return "http";
        }
        return scheme;
    }
}

/**
 * @namespace stdx::net
 * @brief Standard library networking operations.
 */
export namespace stdx::net {
    /**
     * @internal
     * @brief Throws the exception that best describes a getaddrinfo failure.
     * @param status The EAI_* status the lookup returned.
     * @param host The host that was being resolved, for the message.
     *
     * Defined out-of-line so the thrown types are emitted once, inside the
     * module, rather than in every consumer that inlines a lookup.
     */
    [[noreturn]]
    void raise_resolver_error(i32 status, StringView host) {
        #ifdef _WIN32
        const String reason = describe_socket_error(status);
        const i32 no_service = win32::WSATYPE_NOT_FOUND;
        #else
        const String reason = String(unix::gai_strerror(status));
        const i32 no_service = unix::EAI_SERVICE;

        if (status == unix::EAI_SYSTEM) {
            raise_socket_error(last_socket_error(), Ops::fmt("resolving {}", host));
        }
        #endif

        const String message = Ops::fmt("cannot resolve {}: {}", host, reason);
        if (status == no_service) {
            throw UnknownServiceException(message);
        }
        throw UnknownHostException(message);
    }

    /**
     * @class Resolver
     * @brief Turns host names and services into endpoints, using the OS resolver.
     *
     * A Resolver is a small value holding the lookup policy - address family,
     * transport, and whether names are allowed at all - so the policy travels
     * with the object rather than being repeated at every call site. It is freely
     * constructible and holds no process-wide state, so it can be injected.
     *
     * Every lookup blocks: the OS resolver is synchronous, and pretending
     * otherwise by running it on a thread pool would hide that cost. An async
     * resolver belongs with the reactor, not here.
     */
    class [[nodiscard]] Resolver {
    public:
        /**
         * @struct Options
         * @brief The lookup policy a Resolver applies.
         */
        struct Options {
            Optional<IPAddress::Family> family; ///< Restrict results to one family; empty accepts whatever the host is configured for.
            Socket::Type type = Socket::Type::STREAM; ///< The transport the endpoints are for, which decides which services resolve.
            bool passive = false; ///< Whether the results are for bind() rather than connect(); an empty host then means the wildcard address.
            bool numeric_host = false; ///< Whether to reject host names, accepting only address literals.
            bool numeric_service = false; ///< Whether to reject service names, accepting only port numbers.
        };
    private:
        Options _options; ///< The policy applied to every lookup.

        /**
         * @brief The hints structure this resolver's options describe.
         * @return The hints to hand to getaddrinfo.
         */
        [[nodiscard]]
        AddressInfo hints() const noexcept {
            AddressInfo hints{};

            #ifdef _WIN32
            hints.ai_family = _options.family ? native_family(*_options.family) : win32::AF_UNSPEC;
            hints.ai_socktype = _options.type == Socket::Type::STREAM
                ? win32::SOCK_STREAM
                : win32::SOCK_DGRAM;
            hints.ai_flags = win32::AI_ADDRCONFIG;
            if (_options.passive) {
                hints.ai_flags |= win32::AI_PASSIVE;
            }
            if (_options.numeric_host) {
                hints.ai_flags |= win32::AI_NUMERICHOST;
            }
            if (_options.numeric_service) {
                hints.ai_flags |= win32::AI_NUMERICSERV;
            }
            #else
            hints.ai_family = _options.family ? native_family(*_options.family) : unix::sys::AF_UNSPEC;
            hints.ai_socktype = _options.type == Socket::Type::STREAM
                ? unix::sys::SOCK_STREAM
                : unix::sys::SOCK_DGRAM;
            hints.ai_flags = unix::AI_ADDRCONFIG;
            if (_options.passive) {
                hints.ai_flags |= unix::AI_PASSIVE;
            }
            if (_options.numeric_host) {
                hints.ai_flags |= unix::AI_NUMERICHOST;
            }
            if (_options.numeric_service) {
                hints.ai_flags |= unix::AI_NUMERICSERV;
            }
            #endif

            return hints;
        }
    public:
        Resolver() noexcept = default;

        explicit Resolver(Options options) noexcept:
            _options{options} {}

        /**
         * @brief The policy this resolver applies.
         * @return The options.
         */
        [[nodiscard]]
        const Options& options() const noexcept {
            return _options;
        }

        /**
         * @brief Resolves a host and service into every endpoint they name.
         * @param host The host name or address literal; empty means the local host.
         * @param service The service name ("https") or port number ("443"); empty leaves the port at 0.
         * @return The endpoints, in the order the OS offered them.
         * @throws UnknownHostException if the host cannot be resolved.
         * @throws UnknownServiceException if the service is unknown for this transport.
         * @throws SocketException if the resolver itself failed.
         *
         * The order is the OS's, which on a dual-stack host follows RFC 6724 -
         * so connecting to the entries in order is the closest thing to Happy
         * Eyeballs that a synchronous resolver can offer.
         */
        [[nodiscard]]
        THROWS(UnknownHostException)
        Vector<Endpoint> resolve(StringView host, StringView service) const {
            start_socket_layer();

            const String host_text(host);
            const String service_text(service);
            const AddressInfo requested = hints();

            AddressInfoList results;
            #ifdef _WIN32
            const i32 status = win32::getaddrinfo(
                host.empty() ? nullptr : host_text.c_str(),
                service.empty() ? nullptr : service_text.c_str(),
                &requested,
                results.address()
            );
            #else
            const i32 status = unix::getaddrinfo(
                host.empty() ? nullptr : host_text.c_str(),
                service.empty() ? nullptr : service_text.c_str(),
                &requested,
                results.address()
            );
            #endif
            if (status != 0) {
                raise_resolver_error(status, host.empty() ? "the local host"sv : host);
            }

            Vector<Endpoint> endpoints;
            for (const AddressInfo* entry = results.head(); entry != nullptr; entry = entry->ai_next) {
                if (entry->ai_addr == nullptr) {
                    continue;
                }
                SocketAddressStorage storage{};
                const usize length = static_cast<usize>(entry->ai_addrlen) < sizeof(storage)
                    ? static_cast<usize>(entry->ai_addrlen)
                    : sizeof(storage);
                stdx::mem::memcpy(&storage, entry->ai_addr, length);
                if (const Optional<Endpoint> endpoint = from_socket_address(storage)) {
                    endpoints.push_back(*endpoint);
                }
            }
            return endpoints;
        }

        /**
         * @brief Resolves a host and port into every endpoint they name.
         * @param host The host name or address literal; empty means the local host.
         * @param port The port to attach to each result.
         * @return The endpoints, in the order the OS offered them.
         * @throws UnknownHostException if the host cannot be resolved.
         * @throws SocketException if the resolver itself failed.
         */
        [[nodiscard]]
        THROWS(UnknownHostException)
        Vector<Endpoint> resolve(StringView host, u16 port) const {
            Vector<Endpoint> endpoints = resolve(host, Ops::fmt("{}", port));
            return endpoints;
        }

        /**
         * @brief Resolves a host and service, keeping only the first endpoint.
         * @param host The host name or address literal.
         * @param service The service name or port number.
         * @return The first endpoint, or an empty Optional if the lookup found none.
         * @throws UnknownHostException if the host cannot be resolved.
         * @throws SocketException if the resolver itself failed.
         */
        [[nodiscard]]
        THROWS(UnknownHostException)
        Optional<Endpoint> resolve_one(StringView host, StringView service) const {
            const Vector<Endpoint> endpoints = resolve(host, service);
            if (endpoints.empty()) {
                return nullopt;
            }
            return endpoints.front();
        }

        /**
         * @brief Resolves a host and port, keeping only the first endpoint.
         * @param host The host name or address literal.
         * @param port The port to attach to the result.
         * @return The first endpoint, or an empty Optional if the lookup found none.
         * @throws UnknownHostException if the host cannot be resolved.
         * @throws SocketException if the resolver itself failed.
         */
        [[nodiscard]]
        THROWS(UnknownHostException)
        Optional<Endpoint> resolve_one(StringView host, u16 port) const {
            const Vector<Endpoint> endpoints = resolve(host, port);
            if (endpoints.empty()) {
                return nullopt;
            }
            return endpoints.front();
        }

        /**
         * @brief Resolves the authority of a URI into every endpoint it names.
         * @param uri The URI whose host and port to resolve.
         * @return The endpoints, in the order the OS offered them.
         * @throws UnknownHostException if the host cannot be resolved.
         * @throws UnknownServiceException if the URI's scheme names no known service.
         * @throws InvalidArgumentException if @p uri has no authority to resolve.
         *
         * A URI with no explicit port falls back to its scheme as the service
         * name, so "http://example.com/" resolves to port 80 by asking the OS
         * what "http" means rather than by carrying a table of schemes.
         */
        [[nodiscard]]
        THROWS(UnknownHostException, InvalidArgumentException)
        Vector<Endpoint> resolve(const Uri& uri) const {
            if (!uri.has_authority() || uri.host().empty()) {
                throw InvalidArgumentException("the URI has no authority to resolve");
            }
            if (const Optional<u16> port = uri.port()) {
                return resolve(uri.host(), *port);
            }
            if (uri.scheme().empty()) {
                throw InvalidArgumentException("the URI has neither a port nor a scheme to derive one from");
            }
            return resolve(uri.host(), service_for_scheme(uri.scheme()));
        }

        /**
         * @brief Looks a service name up as a port number.
         * @param service The service name, e.g. "https".
         * @return The port, or an empty Optional if the service is unknown.
         * @throws SocketException if the resolver itself failed.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Optional<u16> service_port(StringView service) const {
            try {
                const Optional<Endpoint> endpoint = resolve_one("", service);
                if (!endpoint) {
                    return nullopt;
                }
                return endpoint->port();
            } catch (const UnknownServiceException& _) {
                return nullopt;
            }
        }

        /**
         * @brief Looks an address up as a host name.
         * @param address The address to look up.
         * @return The host name, or an empty Optional if the address has no reverse record.
         * @throws SocketException if the lookup itself failed.
         *
         * A reverse record is under the control of whoever owns the address, so
         * it is evidence of nothing: never use the result to make an
         * authorization decision without forward-confirming it.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Optional<String> reverse(const IPAddress& address) const {
            start_socket_layer();

            SocketAddressStorage storage{};
            const SocketLength length = to_socket_address(Endpoint(address, 0), storage);

            #ifdef _WIN32
            Array<char, win32::NI_MAXHOST> host{};
            const i32 status = win32::getnameinfo(
                reinterpret_cast<const SocketAddress*>(&storage),
                length,
                host.data(),
                static_cast<u32>(host.size()),
                nullptr,
                0,
                win32::NI_NAMEREQD
            );
            #else
            Array<char, unix::NI_MAXHOST> host{};
            const i32 status = unix::getnameinfo(
                reinterpret_cast<const SocketAddress*>(&storage),
                length,
                host.data(),
                static_cast<u32>(host.size()),
                nullptr,
                0,
                unix::NI_NAMEREQD
            );
            #endif
            if (status != 0) {
                return nullopt;
            }
            return String(host.data());
        }

        /**
         * @brief The name of the local host.
         * @return The host name as the OS reports it.
         * @throws SocketException if the name cannot be read.
         */
        [[nodiscard]]
        THROWS(SocketException)
        static String host_name() {
            start_socket_layer();

            Array<char, 256> name{};
            #ifdef _WIN32
            const i32 result = win32::gethostname(name.data(), static_cast<i32>(name.size() - 1));
            #else
            const i32 result = unix::gethostname(name.data(), name.size() - 1);
            #endif
            if (result != 0) {
                raise_socket_error(last_socket_error(), "gethostname");
            }
            return String(name.data());
        }
    };
}
