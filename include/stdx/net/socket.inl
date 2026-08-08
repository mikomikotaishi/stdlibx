#pragma once

using stdx::sync::OnceFlag;

using namespace stdx::os;

/**
 * @namespace stdx::net
 * @brief Standard library networking operations.
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
     * @class ConnectException
     * @brief Exception class for handling connection errors.
     * @extends SocketException
     *
     * The ConnectException class is used to represent an exception that occurs when a connection is refused or times out
     * while being established. It is distinct from NoRouteToHostException, which means the peer was never reached at all.
     */
    class ConnectException: public SocketException {
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
}

namespace stdx::net {
    #ifdef _WIN32
    using SocketAddress = win32::SockAddr;
    using SocketAddressStorage = win32::SockAddrStorage;
    using SocketLength = win32::SocketLength;
    using InternetSocketAddress = win32::SockAddrIn;
    using Internet6SocketAddress = win32::SockAddrIn6;

    inline constexpr i32 NATIVE_AF_UNSPEC = win32::AF_UNSPEC;
    inline constexpr i32 NATIVE_AF_INET = win32::AF_INET;
    inline constexpr i32 NATIVE_AF_INET6 = win32::AF_INET6;
    inline constexpr i32 NATIVE_SOCK_STREAM = win32::SOCK_STREAM;
    inline constexpr i32 NATIVE_SOCK_DGRAM = win32::SOCK_DGRAM;

    /// Winsock raises no SIGPIPE, so a send needs no protective flag.
    inline constexpr i32 SEND_FLAGS = 0;
    #else
    using SocketAddress = unix::sys::SocketAddress;
    using SocketAddressStorage = unix::sys::SocketAddressStorage;
    using SocketLength = unix::sys::SocketLength;
    using InternetSocketAddress = unix::netinet::InternetSocketAddress;
    using Internet6SocketAddress = unix::netinet::Internet6SocketAddress;

    inline constexpr i32 NATIVE_AF_UNSPEC = unix::sys::AF_UNSPEC;
    inline constexpr i32 NATIVE_AF_INET = unix::sys::AF_INET;
    inline constexpr i32 NATIVE_AF_INET6 = unix::sys::AF_INET6;
    inline constexpr i32 NATIVE_SOCK_STREAM = unix::sys::SOCK_STREAM;
    inline constexpr i32 NATIVE_SOCK_DGRAM = unix::sys::SOCK_DGRAM;

    /**
     * @internal
     * @brief Report EPIPE from a send to a closed peer rather than killing the process with SIGPIPE.
     *
     * Both Linux and Darwin define MSG_NOSIGNAL, so one flag serves both - but
     * only because the value is read from each platform's own header. Writing it
     * out would fail in the worst way available:
     *
     *   Linux   MSG_NOSIGNAL = 0x4000     Darwin  MSG_NOSIGNAL = 0x80000
     *   Linux   MSG_RCVMORE  = undefined  Darwin  MSG_RCVMORE  = 0x4000
     *
     * Linux's value is Darwin's MSG_RCVMORE, a receive-side flag. A leaked literal
     * would not be rejected: send() would accept it, ignore it, and the next write
     * to a closed peer would kill the process.
     *
     * Darwin gates the macro behind __DARWIN_C_LEVEL >= 200809L, the default but
     * not a guarantee, and whether its kernel honours the flag is a runtime
     * question. If it does not, the fallback is SO_NOSIGPIPE - a setsockopt rather
     * than a send flag, so it would go wherever a descriptor is taken ownership
     * of, and it is not inherited by an accepted socket, so try_accept would have
     * to set it on each peer too. SocketTest's send_after_peer_close settles which
     * is needed: the flag works, or the test binary dies rather than failing.
     */
    #if defined(__linux__) || defined(__APPLE__)
    inline constexpr i32 SEND_FLAGS = unix::sys::MSG_NOSIGNAL;
    #else
    #error "a send path without SIGPIPE suppression would let a closed peer kill the process"
    #endif
    #endif

    /**
     * @internal
     * @brief The native address family constant for @p family.
     * @param family The address family.
     * @return AF_INET or AF_INET6.
     */
    [[nodiscard]]
    static i32 native_family(IPAddress::Family family) noexcept {
        return family == IPAddress::Family::IPV4 ? NATIVE_AF_INET : NATIVE_AF_INET6;
    }

    /**
     * @internal
     * @brief The error code left behind by the last failed socket call.
     * @return The platform error code.
     */
    [[nodiscard]]
    inline i32 last_socket_error() noexcept {
        #ifdef _WIN32
        return static_cast<i32>(win32::WSAGetLastError());
        #else
        return unix::errnov();
        #endif
    }

    /**
     * @internal
     * @brief Whether @p error means "nothing to do right now" on a non-blocking socket.
     * @param error The platform error code.
     * @return true if the call should be retried once the socket is ready, false otherwise.
     */
    [[nodiscard]]
    inline bool is_would_block(i32 error) noexcept {
        #ifdef _WIN32
        return error == win32::WSAEWOULDBLOCK;
        #else
        return error == unix::EAGAIN || error == unix::EWOULDBLOCK;
        #endif
    }

    /**
     * @internal
     * @brief Whether @p error means a signal interrupted the call before it did anything.
     * @param error The platform error code.
     * @return true if the call should simply be reissued, false otherwise.
     */
    [[nodiscard]]
    inline bool is_interrupted(i32 error) noexcept {
        #ifdef _WIN32
        return error == win32::WSAEINTR;
        #else
        return error == unix::EINTR;
        #endif
    }

    /**
     * @internal
     * @brief Whether @p error means a non-blocking connect is still under way.
     * @param error The platform error code.
     * @return true if the connection is pending, false otherwise.
     */
    [[nodiscard]]
    inline bool is_in_progress(i32 error) noexcept {
        #ifdef _WIN32
        // A non-blocking Winsock connect reports WSAEWOULDBLOCK, not WSAEINPROGRESS.
        return error == win32::WSAEWOULDBLOCK || error == win32::WSAEINPROGRESS || error == win32::WSAEALREADY;
        #else
        return error == unix::EINPROGRESS || error == unix::EALREADY;
        #endif
    }

    /**
     * @internal
     * @brief A human-readable description of @p error.
     * @param error The platform error code.
     * @return The system's message for @p error.
     */
    [[nodiscard]]
    inline String describe_socket_error(i32 error) {
        return ErrorCode(error, Ops::system_category()).message();
    }

    /**
     * @internal
     * @brief Converts a port from host to network byte order.
     * @param port The port in host order.
     * @return The port in network order.
     */
    [[nodiscard]]
    inline u16 to_network_order(u16 port) noexcept {
        #ifdef _WIN32
        return win32::htons(port);
        #else
        return unix::arpa::htons(port);
        #endif
    }

    /**
     * @internal
     * @brief Converts a port from network to host byte order.
     * @param port The port in network order.
     * @return The port in host order.
     */
    [[nodiscard]]
    inline u16 from_network_order(u16 port) noexcept {
        #ifdef _WIN32
        return win32::ntohs(port);
        #else
        return unix::arpa::ntohs(port);
        #endif
    }

    /**
     * @internal
     * @brief Throws the exception that best describes @p error.
     * @param error The platform error code.
     * @param operation The name of the call that failed, for the message.
     *
     * Defined out-of-line so the thrown types are emitted once, inside the
     * module, rather than in every consumer that inlines a socket operation.
     */
    [[noreturn]]
    void raise_socket_error(i32 error, StringView operation) {
        const String message = Ops::fmt("{} failed: {}", operation, describe_socket_error(error));
        #ifdef _WIN32
        switch (error) {
            case win32::WSAEADDRINUSE:
            case win32::WSAEADDRNOTAVAIL:
                throw BindException(message);
            case win32::WSAECONNREFUSED:
                throw ConnectException(message);
            case win32::WSAENETUNREACH:
            case win32::WSAEHOSTUNREACH:
                throw NoRouteToHostException(message);
            case win32::WSAETIMEDOUT:
                throw SocketTimeoutException(message);
            default:
                throw SocketException(message);
        }
        #else
        if (error == unix::EADDRINUSE || error == unix::EADDRNOTAVAIL || error == unix::EACCES) {
            throw BindException(message);
        }
        if (error == unix::ECONNREFUSED) {
            throw ConnectException(message);
        }
        if (error == unix::ENETUNREACH || error == unix::EHOSTUNREACH) {
            throw NoRouteToHostException(message);
        }
        if (error == unix::ECONNREFUSED || error == unix::EMSGSIZE) {
            throw PortUnreachableException(message);
        }
        // A blocking socket carrying SO_RCVTIMEO reports a lapsed deadline as
        // EAGAIN, which is a timeout rather than the retry-later of a
        // non-blocking socket; callers that can block reach this path.
        if (error == unix::ETIMEDOUT || is_would_block(error)) {
            throw SocketTimeoutException(message);
        }
        throw SocketException(message);
        #endif
    }

    /**
     * @internal
     * @brief Brings the platform's socket layer up, once per process.
     *
     * Only Winsock needs this. The matching WSACleanup is deliberately never
     * issued: the library has no way to know when the last socket user is done,
     * and the OS reclaims the reservation when the process exits.
     */
    inline void start_socket_layer() {
        #ifdef _WIN32
        static OnceFlag flag;
        stdx::sync::call_once(flag, [] -> void {
            win32::WsaData data{};
            if (win32::WSAStartup(static_cast<u16>(win32::WINSOCK_VERSION_2_2), &data) != 0) {
                raise_socket_error(last_socket_error(), "WSAStartup");
            }
        });
        #endif
    }

    /**
     * @internal
     * @brief Writes @p endpoint into @p storage in the platform's layout.
     * @param endpoint The endpoint to convert.
     * @param storage The storage to fill; it is fully overwritten.
     * @return The number of bytes of @p storage that @p endpoint occupies.
     */
    inline SocketLength to_socket_address(const Endpoint& endpoint, SocketAddressStorage& storage) noexcept {
        storage = SocketAddressStorage{};

        if (const Optional<IPv4Address> v4 = endpoint.address().to_v4()) {
            InternetSocketAddress address {
                .sin_family = NATIVE_AF_INET,
                .sin_port = to_network_order(endpoint.port()),
            };
            const Array<u8, 4> octets = v4->octets();
            stdx::mem::memcpy(&address.sin_addr, octets.data(), octets.size());
            stdx::mem::memcpy(&storage, &address, sizeof(address));
            return static_cast<SocketLength>(sizeof(address));
        }

        const IPv6Address v6 = *endpoint.address().to_v6();
        Internet6SocketAddress address {
            .sin6_family = NATIVE_AF_INET6,
            .sin6_port = to_network_order(endpoint.port()),
            .sin6_scope_id = v6.scope_id(),
        };
        const Array<u8, 16> bytes = v6.octets();
        stdx::mem::memcpy(&address.sin6_addr, bytes.data(), bytes.size());
        stdx::mem::memcpy(&storage, &address, sizeof(address));
        return static_cast<SocketLength>(sizeof(address));
    }

    /**
     * @internal
     * @brief Reads an endpoint back out of the platform's layout.
     * @param storage The storage to read.
     * @return The endpoint, or an empty Optional if the family is neither IPv4 nor IPv6.
     */
    [[nodiscard]]
    inline Optional<Endpoint> from_socket_address(const SocketAddressStorage& storage) noexcept {
        const i32 family = static_cast<i32>(reinterpret_cast<const SocketAddress&>(storage).sa_family);

        if (family == NATIVE_AF_INET) {
            InternetSocketAddress address{};
            stdx::mem::memcpy(&address, &storage, sizeof(address));
            Array<u8, 4> octets = {};
            stdx::mem::memcpy(octets.data(), &address.sin_addr, octets.size());
            return Endpoint(IPv4Address(octets), from_network_order(address.sin_port));
        }

        if (family == NATIVE_AF_INET6) {
            Internet6SocketAddress address{};
            stdx::mem::memcpy(&address, &storage, sizeof(address));
            Array<u8, 16> bytes = {};
            stdx::mem::memcpy(bytes.data(), &address.sin6_addr, bytes.size());
            return Endpoint(
                IPv6Address(bytes, static_cast<u32>(address.sin6_scope_id)),
                from_network_order(address.sin6_port)
            );
        }

        return nullopt;
    }
}

export namespace stdx::net {
    /**
     * @class Socket
     * @brief An owning handle to a TCP or UDP socket.
     *
     * A Socket owns its descriptor and closes it on destruction; it moves but
     * does not copy. Every operation comes in two forms: a throwing one for
     * straight-line blocking code, and a `try_` one that reports "not ready yet"
     * as an empty Optional so a reactor can drive the same socket without
     * treating a would-block as failure. Set @ref set_blocking to false and hand
     * @ref native_handle to a poller to build the latter.
     *
     * The class is a thin, honest wrapper: it does not buffer, does not retry
     * short writes (except in @ref send_all), and does not resolve names - see
     * Resolver for that.
     */
    class [[nodiscard]] Socket {
    public:
        /**
         * @brief The platform's socket descriptor type.
         */
        #ifdef _WIN32
        using NativeHandle = win32::Socket;
        #else
        using NativeHandle = i32;
        #endif

        /**
         * @brief The value a NativeHandle never takes when it refers to a live socket.
         */
        #ifdef _WIN32
        static constexpr NativeHandle INVALID_HANDLE = static_cast<NativeHandle>(~static_cast<NativeHandle>(0));
        #else
        static constexpr NativeHandle INVALID_HANDLE = -1;
        #endif

        /**
         * @enum Type
         * @brief The transport a socket speaks.
         */
        enum class Type: u8 {
            STREAM, ///< TCP
            DATAGRAM, ///< UDP
        };

        /**
         * @enum ShutdownMode
         * @brief Which half of a connection to shut down.
         */
        enum class ShutdownMode: u8 {
            READ, ///< Refuse further reads; the peer may still receive.
            WRITE, ///< Send a FIN; the peer sees end-of-stream but may still send.
            BOTH, ///< Shut both halves down.
        };

        /**
         * @struct Received
         * @brief The outcome of a datagram receive.
         */
        struct Received {
            usize length = 0; ///< The number of bytes written into the buffer.
            Endpoint from{}; ///< The peer the datagram came from.
        };
    private:
        /**
         * @brief The native handle to the underlying socket.
         */
        NativeHandle _handle = INVALID_HANDLE; ///< The owned descriptor, or INVALID_HANDLE when closed.

        /**
         * @brief Throws unless this socket is open.
         * @throws SocketException if the socket has been closed or moved from.
         */
        THROWS(SocketException)
        void require_open() const {
            if (!is_open()) {
                throw SocketException("socket is closed");
            }
        }

        /**
         * @brief Sets an integer-valued socket option.
         * @param level The protocol level the option belongs to.
         * @param option The option to set.
         * @param value The value to set it to.
         * @throws SocketException if the option cannot be set.
         */
        THROWS(SocketException)
        void set_option(i32 level, i32 option, i32 value) {
            require_open();
            #ifdef _WIN32
            const i32 result = win32::setsockopt(
                _handle, level, option, reinterpret_cast<const char*>(&value), static_cast<i32>(sizeof(value))
            );
            #else
            const i32 result = unix::sys::setsockopt(
                _handle, level, option, &value, static_cast<SocketLength>(sizeof(value))
            );
            #endif
            if (result != 0) {
                raise_socket_error(last_socket_error(), "setsockopt");
            }
        }

        /**
         * @brief Reads an integer-valued socket option.
         * @param level The protocol level the option belongs to.
         * @param option The option to read.
         * @return The option's value.
         * @throws SocketException if the option cannot be read.
         */
        [[nodiscard]]
        THROWS(SocketException)
        i32 option(i32 level, i32 option_name) const {
            require_open();
            i32 value = 0;
            SocketLength length = static_cast<SocketLength>(sizeof(value));
            #ifdef _WIN32
            const i32 result = win32::getsockopt(
                _handle, level, option_name, reinterpret_cast<char*>(&value), &length
            );
            #else
            const i32 result = unix::sys::getsockopt(_handle, level, option_name, &value, &length);
            #endif
            if (result != 0) {
                raise_socket_error(last_socket_error(), "getsockopt");
            }
            return value;
        }

        /**
         * @brief Sets one of the two timeout options.
         * @param option SO_RCVTIMEO or SO_SNDTIMEO.
         * @param timeout The timeout; a zero duration removes the deadline.
         * @throws SocketException if the option cannot be set.
         */
        THROWS(SocketException)
        void set_timeout(i32 option, Milliseconds timeout) {
            require_open();
            #ifdef _WIN32
            const u32 value = static_cast<u32>(timeout.count());
            const i32 result = win32::setsockopt(
                _handle,
                win32::SOL_SOCKET,
                option,
                reinterpret_cast<const char*>(&value),
                static_cast<i32>(sizeof(value))
            );
            #else
            // Both members are cast to whatever the platform declared them as
            // rather than assumed to be long. Darwin's tv_usec is
            // __darwin_suseconds_t, a 32-bit type, and a narrowing conversion from
            // a non-constant expression inside braces is ill-formed - so leaving
            // these bare is not a portability wart but a build failure there.
            using TimeValue = unix::sys::TimeValue;
            TimeValue value{
                .tv_sec = static_cast<decltype(TimeValue::tv_sec)>(timeout.count() / 1000),
                .tv_usec = static_cast<decltype(TimeValue::tv_usec)>((timeout.count() % 1000) * 1000),
            };
            const i32 result = unix::sys::setsockopt(
                _handle, unix::sys::SOL_SOCKET, option, &value, static_cast<SocketLength>(sizeof(value))
            );
            #endif
            if (result != 0) {
                raise_socket_error(last_socket_error(), "setsockopt");
            }
        }
    public:
        /**
         * @brief Constructs a closed socket, owning nothing.
         */
        constexpr Socket() noexcept = default;

        /**
         * @brief Opens a socket of the given family and transport.
         * @param family The address family to open the socket in.
         * @param type The transport the socket speaks.
         * @throws SocketException if the socket cannot be opened.
         */
        THROWS(SocketException)
        Socket(IPAddress::Family family, Type type) {
            start_socket_layer();

            const i32 native_type = (type == Type::STREAM)
                ? NATIVE_SOCK_STREAM
                : NATIVE_SOCK_DGRAM;
            #ifdef _WIN32
            _handle = win32::socket(native_family(family), native_type, 0);
            #else
            _handle = unix::sys::socket(native_family(family), native_type, 0);
            #endif
            if (_handle == INVALID_HANDLE) {
                raise_socket_error(last_socket_error(), "socket");
            }
        }

        /**
         * @brief Adopts an already-open descriptor.
         * @param handle The descriptor to take ownership of.
         */
        constexpr explicit Socket(NativeHandle handle) noexcept:
            _handle{handle} {}

        Socket(const Socket&) = delete;

        Socket& operator=(const Socket&) = delete;

        Socket(Socket&& other) noexcept:
            _handle{Ops::exchange(other._handle, INVALID_HANDLE)} {}

        Socket& operator=(Socket&& other) noexcept {
            if (this != &other) {
                close();
                _handle = Ops::exchange(other._handle, INVALID_HANDLE);
            }
            return *this;
        }

        ~Socket() {
            close();
        }

        /**
         * @brief Whether this socket owns a descriptor.
         * @return true if the socket is open, false if it is closed or was moved from.
         */
        [[nodiscard]]
        constexpr bool is_open() const noexcept {
            return _handle != INVALID_HANDLE;
        }

        /**
         * @brief The owned descriptor, for handing to a poller or a foreign API.
         * @return The descriptor, or INVALID_HANDLE if the socket is closed.
         */
        [[nodiscard]]
        constexpr NativeHandle native_handle() const noexcept {
            return _handle;
        }

        /**
         * @brief Gives up ownership of the descriptor without closing it.
         * @return The descriptor, which the caller must now close.
         */
        [[nodiscard]]
        constexpr NativeHandle release() noexcept {
            return Ops::exchange(_handle, INVALID_HANDLE);
        }

        /**
         * @brief Closes the socket if it is open.
         *
         * Idempotent, and never throws: a close that fails leaves nothing the
         * caller could usefully do, and this runs from the destructor.
         */
        void close() noexcept {
            if (!is_open()) {
                return;
            }
            const NativeHandle handle = Ops::exchange(_handle, INVALID_HANDLE);
            #ifdef _WIN32
            static_cast<void>(win32::closesocket(handle));
            #else
            static_cast<void>(unix::close(handle));
            #endif
        }

        /**
         * @brief Binds the socket to a local endpoint.
         * @param endpoint The local endpoint; port 0 asks the OS to pick one.
         * @throws BindException if the address is in use or unavailable.
         * @throws SocketException if the socket is closed or the bind otherwise fails.
         */
        THROWS(BindException)
        void bind(const Endpoint& endpoint) {
            require_open();
            SocketAddressStorage storage{};
            const SocketLength length = to_socket_address(endpoint, storage);
            #ifdef _WIN32
            const i32 result = win32::bind(_handle, reinterpret_cast<const SocketAddress*>(&storage), length);
            #else
            const i32 result = unix::sys::bind(_handle, reinterpret_cast<const SocketAddress*>(&storage), length);
            #endif
            if (result != 0) {
                raise_socket_error(last_socket_error(), "bind");
            }
        }

        /**
         * @brief Marks the socket as accepting connections.
         * @param backlog The number of pending connections the OS should queue.
         * @throws SocketException if the socket cannot be made to listen.
         */
        THROWS(SocketException)
        void listen(i32 backlog = 128) {
            require_open();
            #ifdef _WIN32
            const i32 result = win32::listen(_handle, backlog);
            #else
            const i32 result = unix::sys::listen(_handle, backlog);
            #endif
            if (result != 0) {
                raise_socket_error(last_socket_error(), "listen");
            }
        }

        /**
         * @brief Accepts one pending connection, waiting for one if the socket blocks.
         * @return The connected socket.
         * @throws SocketException if no connection could be accepted.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Socket accept() {
            Optional<Socket> accepted = try_accept();
            if (!accepted.has_value()) {
                raise_socket_error(last_socket_error(), "accept");
            }
            return Ops::move(*accepted);
        }

        /**
         * @brief Accepts one pending connection if there is one.
         * @return The connected socket, or an empty Optional if none is pending.
         * @throws SocketException if the accept fails for a reason other than emptiness.
         *
         * On a blocking socket this waits, so the empty Optional only ever means
         * "would block" - that is, the socket is non-blocking and idle.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Optional<Socket> try_accept() {
            require_open();
            while (true) {
                SocketAddressStorage storage{};
                SocketLength length = static_cast<SocketLength>(sizeof(storage));
                #ifdef _WIN32
                const NativeHandle peer = win32::accept(
                    _handle, reinterpret_cast<SocketAddress*>(&storage), &length
                );
                #else
                const NativeHandle peer = unix::sys::accept(
                    _handle, reinterpret_cast<SocketAddress*>(&storage), &length
                );
                #endif
                if (peer != INVALID_HANDLE) {
                    Socket accepted(peer);
                    accepted.set_blocking(true);
                    return accepted;
                }
                const i32 error = last_socket_error();
                if (is_interrupted(error)) {
                    continue;
                }
                if (is_would_block(error)) {
                    return nullopt;
                }
                raise_socket_error(error, "accept");
            }
        }

        /**
         * @brief Connects the socket to a peer, waiting for the handshake if the socket blocks.
         * @param endpoint The peer to connect to.
         * @throws ConnectException if the peer refused the connection.
         * @throws NoRouteToHostException if the peer could not be reached.
         * @throws SocketException if the connect otherwise fails.
         */
        THROWS(ConnectException)
        void connect(const Endpoint& endpoint) {
            if (!try_connect(endpoint)) {
                throw SocketException("connect is still in progress on a non-blocking socket");
            }
        }

        /**
         * @brief Starts connecting the socket to a peer.
         * @param endpoint The peer to connect to.
         * @return true if the connection is established, false if it is still being set up.
         * @throws ConnectException if the peer refused the connection.
         * @throws NoRouteToHostException if the peer could not be reached.
         * @throws SocketException if the connect otherwise fails.
         *
         * A false return only happens on a non-blocking socket; wait for it to
         * become writable and then check @ref take_error to learn the outcome.
         */
        THROWS(ConnectException)
        bool try_connect(const Endpoint& endpoint) {
            require_open();
            SocketAddressStorage storage{};
            const SocketLength length = to_socket_address(endpoint, storage);

            while (true) {
                #ifdef _WIN32
                const i32 result = win32::connect(
                    _handle, reinterpret_cast<const SocketAddress*>(&storage), length
                );
                #else
                const i32 result = unix::sys::connect(
                    _handle, reinterpret_cast<const SocketAddress*>(&storage), length
                );
                #endif
                if (result == 0) {
                    return true;
                }
                const i32 error = last_socket_error();
                if (is_interrupted(error)) {
                    continue;
                }
                if (is_in_progress(error)) {
                    return false;
                }
                #ifdef _WIN32
                if (error == win32::WSAEISCONN) {
                    return true;
                }
                #else
                if (error == unix::EISCONN) {
                    return true;
                }
                #endif
                raise_socket_error(error, "connect");
            }
        }

        /**
         * @brief Sends as much of @p buffer as the socket will take.
         * @param buffer The bytes to send.
         * @return The number of bytes sent, which may be fewer than requested.
         * @throws SocketException if the send fails.
         */
        [[nodiscard]]
        THROWS(SocketException)
        usize send(Span<const byte> buffer) {
            const Optional<usize> sent = try_send(buffer);
            if (!sent.has_value()) {
                raise_socket_error(last_socket_error(), "send");
            }
            return *sent;
        }

        /**
         * @brief Sends as much of @p buffer as the socket will take, if it will take any.
         * @param buffer The bytes to send.
         * @return The number of bytes sent, or an empty Optional if the socket would block.
         * @throws SocketException if the send fails.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Optional<usize> try_send(Span<const byte> buffer) {
            require_open();
            while (true) {
                #ifdef _WIN32
                const isize result = win32::send(
                    _handle, reinterpret_cast<const char*>(buffer.data()), static_cast<i32>(buffer.size()), SEND_FLAGS
                );
                #else
                const isize result = unix::sys::send(_handle, buffer.data(), buffer.size(), SEND_FLAGS);
                #endif
                if (result >= 0) {
                    return static_cast<usize>(result);
                }
                const i32 error = last_socket_error();
                if (is_interrupted(error)) {
                    continue;
                }
                if (is_would_block(error)) {
                    return nullopt;
                }
                raise_socket_error(error, "send");
            }
        }

        /**
         * @brief Sends every byte of @p buffer, reissuing the send as needed.
         * @param buffer The bytes to send.
         * @throws SocketException if the send fails, or if the socket would block.
         *
         * Only useful on a blocking socket: there is nothing sensible to do about
         * a partial write on a non-blocking one but hand it back to the caller,
         * so a would-block here is reported as a failure.
         */
        THROWS(SocketException)
        void send_all(Span<const byte> buffer) {
            usize offset = 0;
            while (offset < buffer.size()) {
                const Optional<usize> sent = try_send(buffer.subspan(offset));
                if (!sent.has_value()) {
                    throw SocketException("send would block before the whole buffer was written");
                }
                if (*sent == 0) {
                    throw SocketException("send made no progress");
                }
                offset += *sent;
            }
        }

        /**
         * @brief Receives whatever the socket has, up to the size of @p buffer.
         * @param buffer The buffer to fill.
         * @return The number of bytes received; zero means the peer closed its end.
         * @throws SocketException if the receive fails.
         */
        [[nodiscard]]
        THROWS(SocketException)
        usize receive(Span<byte> buffer) {
            const Optional<usize> received = try_receive(buffer);
            if (!received.has_value()) {
                raise_socket_error(last_socket_error(), "recv");
            }
            return *received;
        }

        /**
         * @brief Receives whatever the socket has ready, up to the size of @p buffer.
         * @param buffer The buffer to fill.
         * @return The number of bytes received, or an empty Optional if the socket would block.
         * @throws SocketException if the receive fails.
         *
         * A returned zero is end-of-stream, not emptiness: the peer has shut its
         * sending half down and no further data will arrive.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Optional<usize> try_receive(Span<byte> buffer) {
            require_open();
            while (true) {
                #ifdef _WIN32
                const isize result = win32::recv(
                    _handle, reinterpret_cast<char*>(buffer.data()), static_cast<i32>(buffer.size()), 0
                );
                #else
                const isize result = unix::sys::recv(_handle, buffer.data(), buffer.size(), 0);
                #endif
                if (result >= 0) {
                    return static_cast<usize>(result);
                }
                const i32 error = last_socket_error();
                if (is_interrupted(error)) {
                    continue;
                }
                if (is_would_block(error)) {
                    return nullopt;
                }
                raise_socket_error(error, "recv");
            }
        }

        /**
         * @brief Fills @p buffer completely, reissuing the receive as needed.
         * @param buffer The buffer to fill.
         * @return true if the buffer was filled, false if the peer closed first.
         * @throws SocketException if the receive fails, or if the socket would block.
         */
        [[nodiscard]]
        THROWS(SocketException)
        bool receive_exactly(Span<byte> buffer) {
            usize offset = 0;
            while (offset < buffer.size()) {
                const Optional<usize> received = try_receive(buffer.subspan(offset));
                if (!received.has_value()) {
                    throw SocketException("recv would block before the whole buffer was filled");
                }
                if (*received == 0) {
                    return false;
                }
                offset += *received;
            }
            return true;
        }

        /**
         * @brief Sends one datagram to @p endpoint.
         * @param buffer The bytes to send.
         * @param endpoint The peer to send them to.
         * @return The number of bytes sent.
         * @throws SocketException if the send fails, or if the socket would block.
         */
        [[nodiscard]]
        THROWS(SocketException)
        usize send_to(Span<const byte> buffer, const Endpoint& endpoint) {
            const Optional<usize> sent = try_send_to(buffer, endpoint);
            if (!sent.has_value()) {
                raise_socket_error(last_socket_error(), "sendto");
            }
            return *sent;
        }

        /**
         * @brief Sends one datagram to @p endpoint if the socket can take it now.
         * @param buffer The bytes to send.
         * @param endpoint The peer to send them to.
         * @return The number of bytes sent, or an empty Optional if the socket would block.
         * @throws SocketException if the send fails.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Optional<usize> try_send_to(Span<const byte> buffer, const Endpoint& endpoint) {
            require_open();
            SocketAddressStorage storage{};
            const SocketLength length = to_socket_address(endpoint, storage);

            while (true) {
                #ifdef _WIN32
                const isize result = win32::sendto(
                    _handle,
                    reinterpret_cast<const char*>(buffer.data()),
                    static_cast<i32>(buffer.size()),
                    SEND_FLAGS,
                    reinterpret_cast<const SocketAddress*>(&storage),
                    length
                );
                #else
                const isize result = unix::sys::sendto(
                    _handle,
                    buffer.data(),
                    buffer.size(),
                    SEND_FLAGS,
                    reinterpret_cast<const SocketAddress*>(&storage),
                    length
                );
                #endif
                if (result >= 0) {
                    return static_cast<usize>(result);
                }
                const i32 error = last_socket_error();
                if (is_interrupted(error)) {
                    continue;
                }
                if (is_would_block(error)) {
                    return nullopt;
                }
                raise_socket_error(error, "sendto");
            }
        }

        /**
         * @brief Receives one datagram, along with the endpoint that sent it.
         * @param buffer The buffer to fill.
         * @return The length received and the sender's endpoint.
         * @throws SocketException if the receive fails.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Received receive_from(Span<byte> buffer) {
            const Optional<Received> received = try_receive_from(buffer);
            if (!received.has_value()) {
                raise_socket_error(last_socket_error(), "recvfrom");
            }
            return *received;
        }

        /**
         * @brief Receives one datagram if one is waiting.
         * @param buffer The buffer to fill.
         * @return The length and sender, or an empty Optional if the socket would block.
         * @throws SocketException if the receive fails.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Optional<Received> try_receive_from(Span<byte> buffer) {
            require_open();
            while (true) {
                SocketAddressStorage storage{};
                SocketLength length = static_cast<SocketLength>(sizeof(storage));
                #ifdef _WIN32
                const isize result = win32::recvfrom(
                    _handle,
                    reinterpret_cast<char*>(buffer.data()),
                    static_cast<i32>(buffer.size()),
                    0,
                    reinterpret_cast<SocketAddress*>(&storage),
                    &length
                );
                #else
                const isize result = unix::sys::recvfrom(
                    _handle,
                    buffer.data(),
                    buffer.size(),
                    0,
                    reinterpret_cast<SocketAddress*>(&storage),
                    &length
                );
                #endif
                if (result >= 0) {
                    const Optional<Endpoint> from = from_socket_address(storage);
                    if (!from.has_value()) {
                        throw SocketException("recvfrom reported an unsupported address family");
                    }
                    return Received{static_cast<usize>(result), *from};
                }
                const i32 error = last_socket_error();
                if (is_interrupted(error)) {
                    continue;
                }
                if (is_would_block(error)) {
                    return nullopt;
                }
                raise_socket_error(error, "recvfrom");
            }
        }

        /**
         * @brief Shuts part or all of the connection down without closing the descriptor.
         * @param mode Which half to shut down.
         * @throws SocketException if the socket is closed.
         *
         * A failing shutdown on an already-disconnected socket is ignored: the
         * peer having gone first is the normal case, not an error.
         */
        THROWS(SocketException)
        void shutdown(ShutdownMode mode = ShutdownMode::BOTH) {
            require_open();
            #ifdef _WIN32
            const i32 how = mode == ShutdownMode::READ
                ? win32::SD_RECEIVE
                : (mode == ShutdownMode::WRITE ? win32::SD_SEND : win32::SD_BOTH);
            static_cast<void>(win32::shutdown(_handle, how));
            #else
            const i32 how = mode == ShutdownMode::READ
                ? unix::sys::SHUT_RD
                : (mode == ShutdownMode::WRITE ? unix::sys::SHUT_WR : unix::sys::SHUT_RDWR);
            static_cast<void>(unix::sys::shutdown(_handle, how));
            #endif
        }

        /**
         * @brief The address this socket is bound to.
         * @return The local endpoint.
         * @throws SocketException if the socket is closed or unbound.
         *
         * Call this after binding to port 0 to learn which port the OS chose.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Endpoint local_endpoint() const {
            require_open();
            SocketAddressStorage storage{};
            SocketLength length = static_cast<SocketLength>(sizeof(storage));
            #ifdef _WIN32
            const i32 result = win32::getsockname(_handle, reinterpret_cast<SocketAddress*>(&storage), &length);
            #else
            const i32 result = unix::sys::getsockname(_handle, reinterpret_cast<SocketAddress*>(&storage), &length);
            #endif
            if (result != 0) {
                raise_socket_error(last_socket_error(), "getsockname");
            }
            const Optional<Endpoint> endpoint = from_socket_address(storage);
            if (!endpoint.has_value()) {
                throw SocketException("the socket is bound to an unsupported address family");
            }
            return *endpoint;
        }

        /**
         * @brief The address of the connected peer.
         * @return The peer's endpoint, or an empty Optional if the socket is not connected.
         * @throws SocketException if the socket is closed.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Optional<Endpoint> remote_endpoint() const {
            require_open();
            SocketAddressStorage storage{};
            SocketLength length = static_cast<SocketLength>(sizeof(storage));
            #ifdef _WIN32
            const i32 result = win32::getpeername(_handle, reinterpret_cast<SocketAddress*>(&storage), &length);
            #else
            const i32 result = unix::sys::getpeername(_handle, reinterpret_cast<SocketAddress*>(&storage), &length);
            #endif
            if (result != 0) {
                return nullopt;
            }
            return from_socket_address(storage);
        }

        /**
         * @brief Puts the socket into blocking or non-blocking mode.
         * @param blocking Whether operations should wait for readiness.
         * @throws SocketException if the mode cannot be changed.
         */
        THROWS(SocketException)
        void set_blocking(bool blocking) {
            require_open();
            #ifdef _WIN32
            win32::ULong mode = blocking ? 0ul : 1ul;
            if (win32::ioctlsocket(_handle, static_cast<i32>(win32::FIONBIO), &mode) != 0) {
                raise_socket_error(last_socket_error(), "ioctlsocket");
            }
            #else
            const i32 flags = unix::fcntl(_handle, unix::F_GETFL, 0);
            if (flags == -1) {
                raise_socket_error(last_socket_error(), "fcntl");
            }
            const i32 updated = blocking ? (flags & ~unix::O_NONBLOCK) : (flags | unix::O_NONBLOCK);
            if (unix::fcntl(_handle, unix::F_SETFL, updated) == -1) {
                raise_socket_error(last_socket_error(), "fcntl");
            }
            #endif
        }

        /**
         * @brief Allows the socket to bind an address still held in TIME_WAIT.
         * @param enable Whether to allow it.
         * @throws SocketException if the option cannot be set.
         */
        THROWS(SocketException)
        void set_reuse_address(bool enable) {
            #ifdef _WIN32
            set_option(win32::SOL_SOCKET, win32::SO_REUSEADDR, enable ? 1 : 0);
            #else
            set_option(unix::sys::SOL_SOCKET, unix::sys::SO_REUSEADDR, enable ? 1 : 0);
            #endif
        }

        /**
         * @brief Sends segments as soon as they are written, rather than coalescing them.
         * @param enable Whether to disable Nagle's algorithm.
         * @throws SocketException if the option cannot be set.
         */
        THROWS(SocketException)
        void set_no_delay(bool enable) {
            #ifdef _WIN32
            set_option(win32::IPPROTO_TCP, win32::TCP_NODELAY, enable ? 1 : 0);
            #else
            set_option(unix::netinet::IPPROTO_TCP, unix::netinet::TCP_NODELAY, enable ? 1 : 0);
            #endif
        }

        /**
         * @brief Probes an idle connection so a dead peer is eventually noticed.
         * @param enable Whether to send keepalive probes.
         * @throws SocketException if the option cannot be set.
         */
        THROWS(SocketException)
        void set_keep_alive(bool enable) {
            #ifdef _WIN32
            set_option(win32::SOL_SOCKET, win32::SO_KEEPALIVE, enable ? 1 : 0);
            #else
            set_option(unix::sys::SOL_SOCKET, unix::sys::SO_KEEPALIVE, enable ? 1 : 0);
            #endif
        }

        /**
         * @brief Permits datagrams to a broadcast address.
         * @param enable Whether to permit them.
         * @throws SocketException if the option cannot be set.
         */
        THROWS(SocketException)
        void set_broadcast(bool enable) {
            #ifdef _WIN32
            set_option(win32::SOL_SOCKET, win32::SO_BROADCAST, enable ? 1 : 0);
            #else
            set_option(unix::sys::SOL_SOCKET, unix::sys::SO_BROADCAST, enable ? 1 : 0);
            #endif
        }

        /**
         * @brief Whether an IPv6 socket refuses IPv4-mapped peers.
         * @param enable true to serve IPv6 only, false to accept IPv4 peers as IPv4-mapped addresses.
         * @throws SocketException if the option cannot be set.
         *
         * Set this to false before binding to IPv6Address::ANY to serve both
         * families from one listener; the IPv4 peers then show up as
         * IPv4-mapped addresses, which IPAddress::to_v4 unwraps.
         */
        THROWS(SocketException)
        void set_v6_only(bool enable) {
            #ifdef _WIN32
            set_option(win32::IPPROTO_IPV6, win32::IPV6_V6ONLY, enable ? 1 : 0);
            #else
            set_option(unix::netinet::IPPROTO_IPV6, unix::netinet::IPV6_V6ONLY, enable ? 1 : 0);
            #endif
        }

        /**
         * @brief Sets the size of the kernel's receive buffer.
         * @param bytes The requested size; the OS may round or cap it.
         * @throws SocketException if the option cannot be set.
         */
        THROWS(SocketException)
        void set_receive_buffer_size(i32 bytes) {
            #ifdef _WIN32
            set_option(win32::SOL_SOCKET, win32::SO_RCVBUF, bytes);
            #else
            set_option(unix::sys::SOL_SOCKET, unix::sys::SO_RCVBUF, bytes);
            #endif
        }

        /**
         * @brief Sets the size of the kernel's send buffer.
         * @param bytes The requested size; the OS may round or cap it.
         * @throws SocketException if the option cannot be set.
         */
        THROWS(SocketException)
        void set_send_buffer_size(i32 bytes) {
            #ifdef _WIN32
            set_option(win32::SOL_SOCKET, win32::SO_SNDBUF, bytes);
            #else
            set_option(unix::sys::SOL_SOCKET, unix::sys::SO_SNDBUF, bytes);
            #endif
        }

        /**
         * @brief The size of the kernel's receive buffer.
         * @return The size in bytes, as the OS reports it.
         * @throws SocketException if the option cannot be read.
         */
        [[nodiscard]]
        THROWS(SocketException)
        i32 receive_buffer_size() const {
            #ifdef _WIN32
            return option(win32::SOL_SOCKET, win32::SO_RCVBUF);
            #else
            return option(unix::sys::SOL_SOCKET, unix::sys::SO_RCVBUF);
            #endif
        }

        /**
         * @brief The size of the kernel's send buffer.
         * @return The size in bytes, as the OS reports it.
         * @throws SocketException if the option cannot be read.
         */
        [[nodiscard]]
        THROWS(SocketException)
        i32 send_buffer_size() const {
            #ifdef _WIN32
            return option(win32::SOL_SOCKET, win32::SO_SNDBUF);
            #else
            return option(unix::sys::SOL_SOCKET, unix::sys::SO_SNDBUF);
            #endif
        }

        /**
         * @brief Gives up on a receive that has waited too long.
         * @param timeout How long to wait; a zero duration waits forever.
         * @throws SocketException if the option cannot be set.
         *
         * A lapsed deadline surfaces as SocketTimeoutException from the
         * receiving call.
         */
        THROWS(SocketException)
        void set_receive_timeout(Milliseconds timeout) {
            #ifdef _WIN32
            set_timeout(win32::SO_RCVTIMEO, timeout);
            #else
            set_timeout(unix::sys::SO_RCVTIMEO, timeout);
            #endif
        }

        /**
         * @brief Gives up on a send that has waited too long.
         * @param timeout How long to wait; a zero duration waits forever.
         * @throws SocketException if the option cannot be set.
         */
        THROWS(SocketException)
        void set_send_timeout(Milliseconds timeout) {
            #ifdef _WIN32
            set_timeout(win32::SO_SNDTIMEO, timeout);
            #else
            set_timeout(unix::sys::SO_SNDTIMEO, timeout);
            #endif
        }

        /**
         * @brief Fetches and clears the socket's pending error.
         * @return The pending error, or an empty Optional if there is none.
         * @throws SocketException if the option cannot be read.
         *
         * This is how a reactor learns whether a non-blocking connect succeeded
         * once the socket reports itself writable.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Optional<ErrorCode> take_error() {
            #ifdef _WIN32
            const i32 error = option(win32::SOL_SOCKET, win32::SO_ERROR);
            #else
            const i32 error = option(unix::sys::SOL_SOCKET, unix::sys::SO_ERROR);
            #endif
            if (error == 0) {
                return nullopt;
            }
            return ErrorCode(error, Ops::system_category());
        }
    };

    /**
     * @class SocketView
     * @brief The settings a socket has whatever it is being used for.
     *
     * TcpStream, TcpListener and UdpSocket each own a Socket and each keeps it to
     * itself, for the same reason those types exist at all: a socket's whole
     * surface is wrong for any one of them. accept() on a datagram socket and
     * send_to() on a listener are both nonsense, and nonsense the compiler ought
     * to be rejecting rather than the kernel. Handing out the Socket gave all of
     * it back.
     *
     * What survives the removal of the role-specific operations is this - blocking
     * mode, buffer sizes, timeouts, and the pending error - and none of it stops
     * being meaningful when the descriptor turns out to be a listener rather than
     * a connection. A reactor sets non-blocking mode on everything it drives; a
     * server tunes buffers on both halves of a proxy; take_error is how the
     * outcome of a non-blocking connect is read. That set is what a wrapper can
     * expose without lying about what it is.
     *
     * The view borrows. It does not own the socket, will not close it, and must
     * not outlive it - the same terms as a Span over someone else's buffer. There
     * is deliberately no way here to bind, listen, connect, transfer bytes, or
     * close: the wrapper owns the descriptor's lifetime and the order things
     * happen to it, and a view able to rebind a bound socket or close one its
     * owner still believes in could only be used to break it.
     */
    class SocketView {
        Socket* _socket; ///< The borrowed socket; never null.
    public:
        SocketView() = DELETE_METHOD("A view always refers to a socket; construct it from the socket it views.");

        /**
         * @brief Views @p socket.
         * @param socket The socket to borrow, which must outlive the view.
         *
         * The conversion is implicit so that `SocketView` works as a parameter
         * type: a function that tunes a socket takes one of these and accepts a
         * bare Socket, a TcpStream's view or a UdpSocket's view without the caller
         * spelling anything out.
         */
        constexpr SocketView(Socket& socket) noexcept:
            _socket{&socket} {}

        /**
         * @brief Puts the socket into blocking or non-blocking mode.
         * @param blocking Whether operations should wait for readiness.
         * @throws SocketException if the mode cannot be changed.
         */
        THROWS(SocketException)
        void set_blocking(bool blocking) {
            _socket->set_blocking(blocking);
        }

        /**
         * @brief Sets the size of the kernel's receive buffer.
         * @param bytes The requested size; the OS may round or cap it.
         * @throws SocketException if the option cannot be set.
         */
        THROWS(SocketException)
        void set_receive_buffer_size(i32 bytes) {
            _socket->set_receive_buffer_size(bytes);
        }

        /**
         * @brief Sets the size of the kernel's send buffer.
         * @param bytes The requested size; the OS may round or cap it.
         * @throws SocketException if the option cannot be set.
         */
        THROWS(SocketException)
        void set_send_buffer_size(i32 bytes) {
            _socket->set_send_buffer_size(bytes);
        }

        /**
         * @brief The size of the kernel's receive buffer.
         * @return The size in bytes, as the OS reports it.
         * @throws SocketException if the option cannot be read.
         */
        [[nodiscard]]
        THROWS(SocketException)
        i32 receive_buffer_size() const {
            return _socket->receive_buffer_size();
        }

        /**
         * @brief The size of the kernel's send buffer.
         * @return The size in bytes, as the OS reports it.
         * @throws SocketException if the option cannot be read.
         */
        [[nodiscard]]
        THROWS(SocketException)
        i32 send_buffer_size() const {
            return _socket->send_buffer_size();
        }

        /**
         * @brief Gives up on a receive that has waited too long.
         * @param timeout How long to wait; a zero duration waits forever.
         * @throws SocketException if the option cannot be set.
         */
        THROWS(SocketException)
        void set_receive_timeout(Milliseconds timeout) {
            _socket->set_receive_timeout(timeout);
        }

        /**
         * @brief Gives up on a send that has waited too long.
         * @param timeout How long to wait; a zero duration waits forever.
         * @throws SocketException if the option cannot be set.
         */
        THROWS(SocketException)
        void set_send_timeout(Milliseconds timeout) {
            _socket->set_send_timeout(timeout);
        }

        /**
         * @brief Fetches and clears the socket's pending error.
         * @return The pending error, or an empty Optional if there is none.
         * @throws SocketException if the option cannot be read.
         *
         * This is how a reactor learns whether a non-blocking connect succeeded
         * once the socket reports itself writable.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Optional<ErrorCode> take_error() {
            return _socket->take_error();
        }

        /**
         * @brief Whether the socket still holds a descriptor.
         * @return false once its owner closed it or was moved from.
         *
         * Worth checking before setting anything: every other operation here
         * throws on a closed socket, and a view cannot tell on its own that the
         * wrapper it borrowed from has since been closed.
         */
        [[nodiscard]]
        constexpr bool is_open() const noexcept {
            return _socket->is_open();
        }

        /**
         * @brief The descriptor, to register with a poller.
         * @return The descriptor, or Socket::INVALID_HANDLE if the socket is closed.
         *
         * Borrowed, like everything else here: the socket still owns it and will
         * still close it.
         */
        [[nodiscard]]
        constexpr Socket::NativeHandle native_handle() const noexcept {
            return _socket->native_handle();
        }
    };
}
