#pragma once

/**
 * @namespace stdx::net
 * @brief Standard library networking operations.
 */
export namespace stdx::net {
    /**
     * @class TcpStream
     * @brief One end of an established TCP connection.
     *
     * A Socket that is known to be connected and known to speak TCP, so the
     * operations that need those things are the only ones it offers. There is no
     * accept() to call on it by mistake and no send_to() to reach for; both were
     * runtime failures on a bare Socket and are now absent from the type.
     *
     * Everything a stream shares with every other socket - blocking mode, buffer
     * sizes, timeouts, the descriptor a Poller wants - is reached through
     * @ref socket rather than mirrored here. One door to the layer below beats
     * twenty forwarding functions that have to be kept in step with it.
     */
    class [[nodiscard]] TcpStream {
    public:
        static constexpr bool IS_BYTE_STREAM = true;
        using ShutdownMode = Socket::ShutdownMode; ///< Which half of the connection to shut down.
    private:
        Socket _socket; ///< The connected descriptor.
    public:
        TcpStream() = DELETE_METHOD("A TcpStream is always connected; take one from connect() or TcpListener::accept().");

        /**
         * @brief Adopts a socket that is already connected.
         * @param socket The connected socket to take over.
         */
        explicit TcpStream(Socket socket) noexcept:
            _socket{Ops::move(socket)} {}

        /**
         * @brief Adopts a descriptor that came from outside this library.
         * @param handle A connected TCP descriptor, which this now owns and will close.
         * @return The stream wrapping it.
         */
        [[nodiscard]]
        static TcpStream from_handle(Socket::NativeHandle handle) noexcept {
            return TcpStream(Socket(handle));
        }

        /**
         * @brief Opens a connection to @p endpoint.
         * @param endpoint The peer to connect to.
         * @return The established connection.
         * @throws ConnectException if the peer refuses or cannot be reached.
         * @throws SocketException if the socket cannot be opened.
         */
        [[nodiscard]]
        THROWS(SocketException, ConnectException)
        static TcpStream connect(const Endpoint& endpoint) {
            Socket socket(endpoint.family(), Socket::Type::STREAM);
            socket.connect(endpoint);
            return TcpStream(Ops::move(socket));
        }

        /**
         * @brief Sends as much of @p buffer as the socket will take.
         * @param buffer The bytes to send.
         * @return The number of bytes sent, which may be fewer than asked.
         * @throws SocketException if the send fails, or if the socket would block.
         */
        [[nodiscard]]
        THROWS(SocketException)
        usize send(Span<const byte> buffer) {
            return _socket.send(buffer);
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
            return _socket.try_send(buffer);
        }

        /**
         * @brief Sends the whole of @p buffer, reissuing the send as needed.
         * @param buffer The bytes to send.
         * @throws SocketException if the send fails, or if the socket would block.
         */
        THROWS(SocketException)
        void send_all(Span<const byte> buffer) {
            _socket.send_all(buffer);
        }

        /**
         * @brief Reads whatever has arrived.
         * @param buffer The buffer to fill.
         * @return The number of bytes read; 0 means the peer has closed its writing half.
         * @throws SocketException if the receive fails, or if the socket would block.
         */
        [[nodiscard]]
        THROWS(SocketException)
        usize receive(Span<byte> buffer) {
            return _socket.receive(buffer);
        }

        /**
         * @brief Reads whatever has arrived, if anything has.
         * @param buffer The buffer to fill.
         * @return The number of bytes read, or an empty Optional if the socket would block.
         * @throws SocketException if the receive fails.
         *
         * A returned 0 is end-of-stream and an empty Optional is "not yet"; they
         * are different answers and a reactor loop has to tell them apart.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Optional<usize> try_receive(Span<byte> buffer) {
            return _socket.try_receive(buffer);
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
            return _socket.receive_exactly(buffer);
        }

        /**
         * @brief Shuts down one or both halves of the connection.
         * @param mode Which half to shut down.
         * @throws SocketException if the shutdown fails.
         *
         * Shutting down the writing half sends a FIN, which is how a peer is told
         * a request is finished without closing the connection it must answer on.
         */
        THROWS(SocketException)
        void shutdown(ShutdownMode mode = ShutdownMode::BOTH) {
            _socket.shutdown(mode);
        }

        /**
         * @brief Whether segments are sent as soon as they are written.
         * @param enable true to disable Nagle's algorithm.
         * @throws SocketException if the option cannot be set.
         */
        THROWS(SocketException)
        void set_no_delay(bool enable) {
            _socket.set_no_delay(enable);
        }

        /**
         * @brief Whether to probe an idle connection to see if the peer is still there.
         * @param enable true to send keepalive probes.
         * @throws SocketException if the option cannot be set.
         */
        THROWS(SocketException)
        void set_keep_alive(bool enable) {
            _socket.set_keep_alive(enable);
        }

        /**
         * @brief This end's address.
         * @return The local endpoint.
         * @throws SocketException if it cannot be read.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Endpoint local_endpoint() const {
            return _socket.local_endpoint();
        }

        /**
         * @brief The peer's address.
         * @return The remote endpoint, or an empty Optional if the stream has been closed or moved from.
         * @throws SocketException if it cannot be read.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Optional<Endpoint> remote_endpoint() const {
            return _socket.remote_endpoint();
        }

        /**
         * @brief The settings the socket has regardless of it being a stream.
         * @return A borrowed view of the owned socket, valid while this stream is.
         */
        [[nodiscard]]
        constexpr SocketView socket() noexcept {
            return SocketView(_socket);
        }

        /**
         * @brief Whether the stream still holds a descriptor.
         * @return false once closed or moved from.
         */
        [[nodiscard]]
        constexpr bool is_open() const noexcept {
            return _socket.is_open();
        }

        /**
         * @brief The descriptor, to register with a Poller.
         * @return The descriptor, or Socket::INVALID_HANDLE if the stream is closed.
         */
        [[nodiscard]]
        constexpr Socket::NativeHandle native_handle() const noexcept {
            return _socket.native_handle();
        }

        /**
         * @brief Gives up the descriptor without closing it.
         * @return The descriptor, which the caller must now close.
         */
        [[nodiscard]]
        constexpr Socket::NativeHandle release() noexcept {
            return _socket.release();
        }

        /**
         * @brief Closes the connection.
         */
        void close() noexcept {
            _socket.close();
        }
    };

    /**
     * @class TcpListener
     * @brief A bound TCP socket that accepts connections and does nothing else.
     */
    class [[nodiscard]] TcpListener {
    private:
        Socket _socket; ///< The bound, listening descriptor.
    public:
        /// What @ref try_accept yields. Naming it is how a listener opts in to Acceptor.
        using Stream = TcpStream;

        TcpListener() = DELETE_METHOD("A TcpListener is always bound and listening; take one from bind().");

        /**
         * @brief Adopts a socket that is already bound and listening.
         * @param socket The listening socket to take over.
         */
        explicit TcpListener(Socket socket) noexcept:
            _socket{Ops::move(socket)} {}

        /**
         * @brief Adopts a descriptor that is already bound and listening.
         * @param handle A listening TCP descriptor, which this now owns and will close.
         * @return The listener wrapping it.
         */
        [[nodiscard]]
        static TcpListener from_handle(Socket::NativeHandle handle) noexcept {
            return TcpListener(Socket(handle));
        }

        /**
         * @brief Binds to @p endpoint and starts listening.
         * @param endpoint The local address to accept on; port 0 asks the OS to pick one.
         * @param backlog How many pending connections the OS should queue.
         * @return The listening socket.
         * @throws BindException if the address is taken or unavailable.
         * @throws SocketException if the socket cannot be opened or listened on.
         */
        [[nodiscard]]
        THROWS(SocketException, BindException)
        static TcpListener bind(const Endpoint& endpoint, i32 backlog = 128) {
            Socket socket(endpoint.family(), Socket::Type::STREAM);
            socket.set_reuse_address(true);
            socket.bind(endpoint);
            socket.listen(backlog);
            return TcpListener(Ops::move(socket));
        }

        /**
         * @brief Binds to every address in both families and starts listening.
         * @param port The local port to accept on; 0 asks the OS to pick one.
         * @param backlog How many pending connections the OS should queue.
         * @return The listening socket, serving IPv4 and IPv6 alike.
         * @throws BindException if the port is taken or unavailable.
         * @throws SocketException if the host will not serve both families from one socket.
         */
        [[nodiscard]]
        THROWS(SocketException, BindException)
        static TcpListener bind_dual_stack(u16 port, i32 backlog = 128) {
            Socket socket(IPAddress::Family::IPV6, Socket::Type::STREAM);
            socket.set_reuse_address(true);
            socket.set_v6_only(false);
            socket.bind(Endpoint(IPAddress(IPv6Address::ANY), port));
            socket.listen(backlog);
            return TcpListener(Ops::move(socket));
        }

        /**
         * @brief Waits for a connection and takes it.
         * @return The accepted connection.
         * @throws SocketException if the accept fails, or if the socket would block.
         */
        [[nodiscard]]
        THROWS(SocketException)
        TcpStream accept() {
            return TcpStream(_socket.accept());
        }

        /**
         * @brief Takes a connection if one is already waiting.
         * @return The accepted connection, or an empty Optional if none is pending.
         * @throws SocketException if the accept fails.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Optional<TcpStream> try_accept() {
            Optional<Socket> accepted = _socket.try_accept();
            if (!accepted.has_value()) {
                return nullopt;
            }
            return TcpStream(Ops::move(*accepted));
        }

        /**
         * @brief The address being listened on.
         * @return The local endpoint, with the port the OS chose if 0 was asked for.
         * @throws SocketException if it cannot be read.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Endpoint local_endpoint() const {
            return _socket.local_endpoint();
        }

        /**
         * @brief The settings the socket has regardless of it being a listener.
         * @return A borrowed view of the owned socket, valid while this listener is.
         *
         * Non-blocking mode above all, which is what an accept loop inside a
         * reactor needs and the only way @ref try_accept becomes useful. See
         * SocketView.
         */
        [[nodiscard]]
        constexpr SocketView socket() noexcept {
            return SocketView(_socket);
        }

        /**
         * @brief Whether the listener still holds a descriptor.
         * @return false once closed or moved from.
         */
        [[nodiscard]]
        constexpr bool is_open() const noexcept {
            return _socket.is_open();
        }

        /**
         * @brief The descriptor, to register with a Poller.
         * @return The descriptor, or Socket::INVALID_HANDLE if the listener is closed.
         *
         * Borrowed, not given: the listener still owns the descriptor and still
         * closes it. Use @ref release to hand it over for good.
         */
        [[nodiscard]]
        constexpr Socket::NativeHandle native_handle() const noexcept {
            return _socket.native_handle();
        }

        /**
         * @brief Gives up the descriptor without closing it.
         * @return The descriptor, which the caller must now close.
         *
         * The counterpart to @ref from_handle. A supervisor that binds ports and
         * hands them to short-lived workers needs this end of the exchange as much
         * as the workers need from_handle.
         */
        [[nodiscard]]
        constexpr Socket::NativeHandle release() noexcept {
            return _socket.release();
        }

        /**
         * @brief Stops listening.
         */
        void close() noexcept {
            _socket.close();
        }
    };
}
