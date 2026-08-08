#pragma once

/**
 * @namespace stdx::net
 * @brief Standard library networking operations.
 */
export namespace stdx::net {
    /**
     * @class UdpSocket
     * @brief A datagram socket: messages with boundaries, no connection, no order.
     *
     * Offers the addressed operations a datagram socket actually has - each
     * @ref send_to names its peer, each @ref receive_from reports the peer it
     * heard from - and none of the connection-oriented ones. There is no
     * accept() here to fail at runtime and no shutdown() that would mean nothing.
     *
     * A datagram socket may still be @ref connect ed, which does not open a
     * connection: it fixes a default peer, so @ref send and @ref receive work
     * and datagrams from anyone else are dropped. That is why those two are
     * here beside the addressed pair.
     */
    class [[nodiscard]] UdpSocket {
    public:
        /// What a datagram receive reports: how much arrived, and from whom.
        using Received = Socket::Received;
    private:
        Socket _socket; ///< The datagram descriptor.
    public:
        UdpSocket() = DELETE_METHOD("A UdpSocket always owns a descriptor; take one from bind() or unbound().");

        /**
         * @brief Adopts an existing datagram socket.
         * @param socket The socket to take over.
         */
        explicit UdpSocket(Socket socket) noexcept:
            _socket{Ops::move(socket)} {}

        /**
         * @brief Adopts a descriptor that came from outside this library.
         * @param handle A datagram descriptor, which this now owns and will close.
         * @return The socket wrapping it.
         *
         * Bound or unbound, connected or not - all four are legitimate states for a
         * datagram socket, so there is nothing here to check even in principle.
         */
        [[nodiscard]]
        static UdpSocket from_handle(Socket::NativeHandle handle) noexcept {
            return UdpSocket(Socket(handle));
        }

        /**
         * @brief Opens a datagram socket bound to @p endpoint.
         * @param endpoint The local address to receive on; port 0 asks the OS to pick one.
         * @return The bound socket.
         * @throws BindException if the address is taken or unavailable.
         * @throws SocketException if the socket cannot be opened.
         */
        [[nodiscard]]
        THROWS(SocketException, BindException)
        static UdpSocket bind(const Endpoint& endpoint) {
            Socket socket(endpoint.family(), Socket::Type::DATAGRAM);
            socket.bind(endpoint);
            return UdpSocket(Ops::move(socket));
        }

        /**
         * @brief Opens a datagram socket with no local address of its own.
         * @param family The address family to open in.
         * @return The socket.
         * @throws SocketException if the socket cannot be opened.
         *
         * The first @ref send_to binds it to an ephemeral port. Enough for a
         * client that only sends and reads the replies.
         */
        [[nodiscard]]
        THROWS(SocketException)
        static UdpSocket unbound(IPAddress::Family family = IPAddress::Family::IPV4) {
            return UdpSocket(Socket(family, Socket::Type::DATAGRAM));
        }

        /**
         * @brief Sends one datagram to @p endpoint.
         * @param buffer The bytes to send, sent whole or not at all.
         * @param endpoint The peer to send them to.
         * @return The number of bytes sent.
         * @throws SocketException if the send fails, or if the socket would block.
         */
        [[nodiscard]]
        THROWS(SocketException)
        usize send_to(Span<const byte> buffer, const Endpoint& endpoint) {
            return _socket.send_to(buffer, endpoint);
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
            return _socket.try_send_to(buffer, endpoint);
        }

        /**
         * @brief Receives one datagram, and reports who sent it.
         * @param buffer The buffer to fill.
         * @return The length received and the sender's endpoint.
         * @throws SocketException if the receive fails, or if the socket would block.
         *
         * A datagram longer than @p buffer is truncated and the rest is lost -
         * there is no second read that returns the remainder.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Received receive_from(Span<byte> buffer) {
            return _socket.receive_from(buffer);
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
            return _socket.try_receive_from(buffer);
        }

        /**
         * @brief Fixes the peer this socket talks to.
         * @param endpoint The peer to address by default.
         * @throws SocketException if the association fails.
         *
         * Opens no connection and sends nothing. It sets the peer @ref send uses
         * and tells the kernel to drop datagrams from anywhere else, which also
         * lets an ICMP port-unreachable be reported back as an error rather than
         * vanishing.
         */
        THROWS(SocketException)
        void connect(const Endpoint& endpoint) {
            _socket.connect(endpoint);
        }

        /**
         * @brief Sends one datagram to the peer set by @ref connect.
         * @param buffer The bytes to send.
         * @return The number of bytes sent.
         * @throws SocketException if no peer has been set, or the send fails.
         */
        [[nodiscard]]
        THROWS(SocketException)
        usize send(Span<const byte> buffer) {
            return _socket.send(buffer);
        }

        /**
         * @brief Sends one datagram to the peer set by @ref connect, if the socket can take it now.
         * @param buffer The bytes to send.
         * @return The number of bytes sent, or an empty Optional if the socket would block.
         * @throws SocketException if no peer has been set, or the send fails.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Optional<usize> try_send(Span<const byte> buffer) {
            return _socket.try_send(buffer);
        }

        /**
         * @brief Receives one datagram from the peer set by @ref connect.
         * @param buffer The buffer to fill.
         * @return The number of bytes read.
         * @throws SocketException if the receive fails, or if the socket would block.
         */
        [[nodiscard]]
        THROWS(SocketException)
        usize receive(Span<byte> buffer) {
            return _socket.receive(buffer);
        }

        /**
         * @brief Receives one datagram from the peer set by @ref connect, if one is waiting.
         * @param buffer The buffer to fill.
         * @return The number of bytes read, or an empty Optional if the socket would block.
         * @throws SocketException if the receive fails.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Optional<usize> try_receive(Span<byte> buffer) {
            return _socket.try_receive(buffer);
        }

        /**
         * @brief Whether this socket may address a broadcast address.
         * @param enable true to permit broadcast sends.
         * @throws SocketException if the option cannot be set.
         *
         * Datagram-specific, which is why it is here and not on Socket's shared
         * surface.
         */
        THROWS(SocketException)
        void set_broadcast(bool enable) {
            _socket.set_broadcast(enable);
        }

        /**
         * @brief This socket's address.
         * @return The local endpoint, with the port the OS chose if 0 was asked for.
         * @throws SocketException if it cannot be read.
         */
        [[nodiscard]]
        THROWS(SocketException)
        Endpoint local_endpoint() const {
            return _socket.local_endpoint();
        }

        /**
         * @brief The settings the socket has regardless of it carrying datagrams.
         * @return A borrowed view of the owned socket, valid while this socket is.
         *
         * Blocking mode, buffer sizes and timeouts. The receive buffer matters
         * more here than it does for a stream: a datagram that arrives with no
         * room for it is dropped outright rather than delayed. See SocketView.
         */
        [[nodiscard]]
        constexpr SocketView socket() noexcept {
            return SocketView(_socket);
        }

        /**
         * @brief Whether the socket still holds a descriptor.
         * @return false once closed or moved from.
         */
        [[nodiscard]]
        constexpr bool is_open() const noexcept {
            return _socket.is_open();
        }

        /**
         * @brief The descriptor, to register with a Poller.
         * @return The descriptor, or Socket::INVALID_HANDLE if the socket is closed.
         *
         * Borrowed, not given: this socket still owns the descriptor and still
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
         * The counterpart to @ref from_handle. Whatever state the socket was in -
         * bound, connected, neither - travels with the descriptor, since all of it
         * lives in the kernel rather than here.
         */
        [[nodiscard]]
        constexpr Socket::NativeHandle release() noexcept {
            return _socket.release();
        }

        /**
         * @brief Closes the socket.
         */
        void close() noexcept {
            _socket.close();
        }
    };
}
