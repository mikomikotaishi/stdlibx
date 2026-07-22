import stdx;

using stdx::collections::Vector;
using stdx::net::BindException;
using stdx::net::ConnectException;
using stdx::net::Endpoint;
using stdx::net::IPAddress;
using stdx::net::IPv4Address;
using stdx::net::IPv6Address;
using stdx::net::Resolver;
using stdx::net::Socket;
using stdx::net::SocketException;
using stdx::net::SocketTimeoutException;
using stdx::net::UnknownHostException;
using stdx::net::Uri;

using namespace stdx::test;

/**
 * @brief A read-only byte view over @p text, for handing to a socket.
 * @param text The text to view.
 * @return A span of bytes over @p text.
 */
[[nodiscard]]
static Span<const byte> bytes_of(StringView text) noexcept {
    return Span<const byte>(reinterpret_cast<const byte*>(text.data()), text.size());
}

/**
 * @brief The bytes of @p buffer read back as text.
 * @param buffer The bytes to read.
 * @return A string of the bytes in @p buffer.
 */
[[nodiscard]]
static String text_of(Span<const byte> buffer) {
    return String(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}

/**
 * @brief A loopback endpoint on a port the OS picks.
 * @param family The address family to use.
 * @return An endpoint on the loopback address with port 0.
 */
[[nodiscard]]
static Endpoint loopback_any_port(IPAddress::Family family) {
    return Endpoint(IPAddress::loopback(family), 0);
}

/**
 * @brief An open TCP socket, bound to nothing and connected to nothing.
 * @param family The address family to open in.
 * @return The open socket.
 */
[[nodiscard]]
static Socket tcp_socket(IPAddress::Family family = IPAddress::Family::IPV4) {
    return Socket(family, Socket::Type::STREAM);
}

/**
 * @brief An open datagram socket, bound to nothing.
 * @param family The address family to open in.
 * @return The open socket.
 */
[[nodiscard]]
static Socket udp_socket(IPAddress::Family family = IPAddress::Family::IPV4) {
    return Socket(family, Socket::Type::DATAGRAM);
}

/**
 * @brief A TCP socket bound to @p endpoint and listening on it.
 * @param endpoint The local endpoint to accept on; port 0 asks the OS to pick one.
 * @param backlog How many pending connections the OS should queue.
 * @return The listening socket.
 *
 * The same sequence TcpListener::bind performs, spelled out here on purpose:
 * these tests exercise the raw socket, so they must not reach the syscalls
 * through the wrapper that is meant to be checked separately.
 */
[[nodiscard]]
static Socket listening_socket(const Endpoint& endpoint, i32 backlog = 128) {
    Socket socket(endpoint.family(), Socket::Type::STREAM);
    socket.set_reuse_address(true);
    socket.bind(endpoint);
    socket.listen(backlog);
    return socket;
}

/**
 * @brief A TCP socket connected to @p endpoint.
 * @param endpoint The peer to connect to.
 * @return The connected socket.
 */
[[nodiscard]]
static Socket connected_socket(const Endpoint& endpoint) {
    Socket socket(endpoint.family(), Socket::Type::STREAM);
    socket.connect(endpoint);
    return socket;
}

void test_socket_lifecycle() {
    Socket closed;
    expect(!closed.is_open(), "a default-constructed socket owns nothing");
    expect(closed.native_handle() == Socket::INVALID_HANDLE, "a closed socket holds no descriptor");

    Socket socket = tcp_socket();
    expect(socket.is_open(), "constructing a socket opens a descriptor");
    const Socket::NativeHandle handle = socket.native_handle();

    Socket moved = Ops::move(socket);
    expect(moved.is_open(), "the move target owns the descriptor");
    expect(!socket.is_open(), "the moved-from socket owns nothing");
    expect(moved.native_handle() == handle, "the descriptor survives the move unchanged");

    moved.close();
    expect(!moved.is_open(), "close() releases the descriptor");
    expect_no_throw([&moved] -> void { moved.close(); }, "close() is idempotent");

    // release() hands ownership back to the caller, who must close it; adopting
    // it into another Socket is the tidiest way to prove it is still live.
    Socket released = tcp_socket();
    const Socket::NativeHandle taken = released.release();
    expect(!released.is_open(), "release() gives up ownership");
    Socket adopted(taken);
    expect(adopted.is_open(), "the released descriptor is still usable");
    expect_no_throw([&adopted] -> void { (void)adopted.local_endpoint(); }, "the adopted socket works");
}

void test_socket_closed_operations() {
    Socket closed;
    Array<byte, 4> buffer{};

    expect_throws<SocketException>(
        [&closed] -> void { closed.listen(); }, "listen on a closed socket throws"
    );
    expect_throws<SocketException>(
        [&closed] -> void { (void)closed.local_endpoint(); }, "local_endpoint on a closed socket throws"
    );
    expect_throws<SocketException>(
        [&closed, &buffer] -> void { (void)closed.receive(buffer); }, "receive on a closed socket throws"
    );
    expect_throws<SocketException>(
        [&closed] -> void { closed.set_no_delay(true); }, "setting an option on a closed socket throws"
    );
}

void test_tcp_roundtrip() {
    Socket listener = listening_socket(loopback_any_port(IPAddress::Family::IPV4));

    const Endpoint bound = listener.local_endpoint();
    expect(bound.port() != 0, "binding to port 0 makes the OS pick a real port");
    expect(bound.address() == IPAddress(IPv4Address::LOOPBACK), "the listener is bound to loopback");

    // The connection completes into the accept queue, so a single thread can
    // connect first and accept afterwards.
    Socket client = connected_socket(bound);
    Socket server = listener.accept();

    expect(server.is_open(), "accept() yields an open socket");
    expect(client.remote_endpoint() == bound, "the client is connected to the listener");
    expect(server.remote_endpoint() == client.local_endpoint(), "the server sees the client's address");
    expect(server.local_endpoint() == bound, "the accepted socket keeps the listener's address");

    client.send_all(bytes_of("ping"));
    Array<byte, 16> buffer{};
    const usize received = server.receive(buffer);
    expect_eq(received, 4u, "the server reads what the client wrote");
    expect_eq(text_of(Span<const byte>(buffer.data(), received)), "ping", "the bytes arrive intact");

    server.send_all(bytes_of("pong"));
    const usize echoed = client.receive(buffer);
    expect_eq(text_of(Span<const byte>(buffer.data(), echoed)), "pong", "the reply arrives intact");

    // A shutdown of the writing half shows up at the peer as end-of-stream.
    client.shutdown(Socket::ShutdownMode::WRITE);
    expect_eq(server.receive(buffer), 0u, "a shut-down peer reads as end-of-stream");
}

void test_tcp_bulk_transfer() {
    Socket listener = listening_socket(loopback_any_port(IPAddress::Family::IPV4));
    Socket client = connected_socket(listener.local_endpoint());
    Socket server = listener.accept();

    // Large enough to need several writes, small enough to fit the loopback
    // buffers without a reader draining concurrently.
    Vector<byte> payload(64 * 1024);
    for (usize i = 0; i < payload.size(); ++i) {
        payload[i] = static_cast<byte>(i & 0xff);
    }

    client.send_all(Span<const byte>(payload.data(), payload.size()));
    client.shutdown(Socket::ShutdownMode::WRITE);

    Vector<byte> echo(payload.size());
    expect(
        server.receive_exactly(Span<byte>(echo.data(), echo.size())),
        "receive_exactly fills the whole buffer"
    );
    expect(echo == payload, "every byte survives the round trip");
    expect_eq(server.receive(Span<byte>(echo.data(), 1)), 0u, "the stream ends after the payload");
}

void test_udp_roundtrip() {
    Socket receiver = udp_socket();
    receiver.bind(loopback_any_port(IPAddress::Family::IPV4));
    const Endpoint bound = receiver.local_endpoint();
    expect(bound.port() != 0, "the OS picks a port for the datagram socket too");

    Socket sender = udp_socket();
    sender.bind(loopback_any_port(IPAddress::Family::IPV4));

    expect_eq(sender.send_to(bytes_of("datagram"), bound), 8u, "send_to writes the whole datagram");

    Array<byte, 32> buffer{};
    const Socket::Received received = receiver.receive_from(buffer);
    expect_eq(received.length, 8u, "the datagram arrives whole");
    expect_eq(
        text_of(Span<const byte>(buffer.data(), received.length)),
        "datagram",
        "the payload arrives intact"
    );
    expect_eq(received.from, sender.local_endpoint(), "the sender's endpoint is reported");
}

void test_socket_options() {
    Socket socket = tcp_socket();

    expect_no_throw([&socket] -> void { socket.set_reuse_address(true); }, "SO_REUSEADDR is settable");
    expect_no_throw([&socket] -> void { socket.set_no_delay(true); }, "TCP_NODELAY is settable");
    expect_no_throw([&socket] -> void { socket.set_keep_alive(true); }, "SO_KEEPALIVE is settable");
    expect_no_throw(
        [&socket] -> void { socket.set_receive_timeout(250ms); }, "SO_RCVTIMEO is settable"
    );
    expect_no_throw(
        [&socket] -> void { socket.set_send_timeout(250ms); }, "SO_SNDTIMEO is settable"
    );

    // The kernel is free to round a buffer request up, and Linux doubles it, so
    // the only safe assertion is that the request was not ignored.
    socket.set_receive_buffer_size(64 * 1024);
    socket.set_send_buffer_size(64 * 1024);
    expect(socket.receive_buffer_size() >= 64 * 1024, "the receive buffer honours the request");
    expect(socket.send_buffer_size() >= 64 * 1024, "the send buffer honours the request");

    expect(!socket.take_error().has_value(), "a fresh socket has no pending error");

    Socket datagram = udp_socket();
    expect_no_throw([&datagram] -> void { datagram.set_broadcast(true); }, "SO_BROADCAST is settable");
}

void test_non_blocking() {
    Socket listener = listening_socket(loopback_any_port(IPAddress::Family::IPV4));
    listener.set_blocking(false);
    expect(!listener.try_accept().has_value(), "an idle non-blocking listener accepts nothing");

    Socket client = connected_socket(listener.local_endpoint());
    Optional<Socket> server = listener.try_accept();
    require(server.has_value(), "a pending connection is accepted without blocking");

    server->set_blocking(false);
    Array<byte, 8> buffer{};
    expect(!server->try_receive(buffer).has_value(), "a non-blocking receive with no data reports would-block");

    client.send_all(bytes_of("x"));
    // The loopback delivery is synchronous enough that the byte is already
    // queued, but a blocking read is the honest way to wait for it.
    server->set_blocking(true);
    expect_eq(server->receive(buffer), 1u, "the byte arrives once the socket blocks again");
}

void test_receive_timeout() {
    Socket listener = listening_socket(loopback_any_port(IPAddress::Family::IPV4));
    Socket client = connected_socket(listener.local_endpoint());
    Socket server = listener.accept();

    server.set_receive_timeout(50ms);
    Array<byte, 8> buffer{};
    expect_throws<SocketTimeoutException>(
        [&server, &buffer] -> void { (void)server.receive(buffer); },
        "a receive that outlives its deadline throws SocketTimeoutException"
    );
}

void test_connection_errors() {
    // Nothing listens on a port we bind and immediately drop, so connecting to
    // it is refused rather than left hanging.
    u16 dead_port = 0;
    {
        Socket probe = tcp_socket();
        probe.bind(loopback_any_port(IPAddress::Family::IPV4));
        dead_port = probe.local_endpoint().port();
    }
    const Endpoint dead(IPAddress(IPv4Address::LOOPBACK), dead_port);
    expect_throws<ConnectException>(
        [&dead] -> void { (void)connected_socket(dead); }, "connecting to a dead port is refused"
    );

    Socket listener = listening_socket(loopback_any_port(IPAddress::Family::IPV4));
    const Endpoint taken = listener.local_endpoint();
    expect_throws<BindException>(
        [&taken] -> void {
            Socket second = tcp_socket();
            second.bind(taken);
        },
        "binding a port that is already listening throws BindException"
    );
}

void test_send_after_peer_close() {
    Socket listener = listening_socket(loopback_any_port(IPAddress::Family::IPV4));
    Socket client = connected_socket(listener.local_endpoint());
    Socket server = listener.accept();

    server.close();

    // The first write can still land in the send buffer, before the peer's RST
    // has been processed, so this keeps writing until the failure surfaces.
    // Exhausting the loop would mean the process took a SIGPIPE and died
    // instead - which is exactly what MSG_NOSIGNAL is set to prevent.
    bool reported = false;
    for (usize attempt = 0; attempt < 1000 && !reported; ++attempt) {
        try {
            client.send_all(bytes_of("orphaned"));
        } catch (const SocketException& _) {
            reported = true;
        }
    }
    expect(reported, "writing to a closed peer raises an exception rather than SIGPIPE");
}

void test_ipv6_roundtrip() {
    Optional<Socket> listener;
    try {
        listener = listening_socket(loopback_any_port(IPAddress::Family::IPV6));
    } catch (const SocketException& _) {
        skip("IPv6 is not available on this host");
    }

    const Endpoint bound = listener->local_endpoint();
    expect(bound.address() == IPAddress(IPv6Address::LOOPBACK), "the listener is bound to ::1");
    expect(bound.family() == IPAddress::Family::IPV6, "the endpoint reports the IPv6 family");

    Socket client = connected_socket(bound);
    Socket server = listener->accept();

    client.send_all(bytes_of("v6"));
    Array<byte, 8> buffer{};
    const usize received = server.receive(buffer);
    expect_eq(text_of(Span<const byte>(buffer.data(), received)), "v6", "IPv6 carries the payload");
    expect(server.remote_endpoint()->address().is_loopback(), "the peer is on the IPv6 loopback");
}

void test_dual_stack() {
    Optional<Socket> listener;
    try {
        Socket socket(IPAddress::Family::IPV6, Socket::Type::STREAM);
        socket.set_reuse_address(true);
        socket.set_v6_only(false);
        socket.bind(Endpoint(IPAddress(IPv6Address::ANY), 0));
        socket.listen();
        listener = Ops::move(socket);
    } catch (const SocketException& _) {
        skip("this host does not serve both families from one socket");
    }

    const u16 port = listener->local_endpoint().port();
    Socket client = connected_socket(Endpoint(IPAddress(IPv4Address::LOOPBACK), port));
    Socket server = listener->accept();

    // An IPv4 peer on an AF_INET6 socket shows up as an IPv4-mapped address.
    const IPAddress peer = server.remote_endpoint()->address();
    expect(peer.is_v6(), "the accepted peer is reported in the socket's family");
    const Optional<IPv6Address> mapped = peer.to_v6();
    require(mapped.has_value(), "the peer address is an IPv6 address");
    expect(mapped->is_v4_mapped(), "the IPv4 peer arrives IPv4-mapped");
    expect(mapped->to_ipv4() == IPv4Address::LOOPBACK, "the mapped address unwraps to the IPv4 peer");

    client.send_all(bytes_of("dual"));
    Array<byte, 8> buffer{};
    expect_eq(server.receive(buffer), 4u, "the dual-stack socket carries IPv4 traffic");
}

void test_resolve_literals() {
    const Resolver resolver{{.numeric_host = true, .numeric_service = true}};

    const Vector<Endpoint> v4 = resolver.resolve("127.0.0.1", 8080);
    require(!v4.empty(), "an IPv4 literal resolves to itself");
    expect_eq(v4.front(), Endpoint(IPAddress(IPv4Address::LOOPBACK), 8080), "the literal keeps its port");

    const Vector<Endpoint> v6 = resolver.resolve("::1", 443);
    require(!v6.empty(), "an IPv6 literal resolves to itself");
    expect_eq(v6.front(), Endpoint(IPAddress(IPv6Address::LOOPBACK), 443), "the IPv6 literal keeps its port");

    // With numeric_host set, a name is not merely unresolved - it is rejected
    // without the resolver ever being consulted.
    expect_throws<UnknownHostException>(
        [&resolver] -> void { (void)resolver.resolve("localhost", 80); },
        "a name is rejected when only literals are accepted"
    );
}

void test_resolve_names() {
    const Resolver resolver;

    Vector<Endpoint> endpoints;
    try {
        endpoints = resolver.resolve("localhost", 80);
    } catch (const UnknownHostException& _) {
        skip("this host cannot resolve localhost");
    }

    require(!endpoints.empty(), "localhost resolves to at least one endpoint");
    for (const Endpoint& endpoint: endpoints) {
        expect(endpoint.address().is_loopback(), "every localhost endpoint is a loopback address");
        expect_eq(endpoint.port(), 80, "every endpoint carries the requested port");
    }

    const Optional<Endpoint> first = resolver.resolve_one("localhost", 80);
    expect(first == endpoints.front(), "resolve_one returns the first result");

    // A resolved endpoint is directly usable, which is the whole point.
    Socket listener = listening_socket(loopback_any_port(IPAddress::Family::IPV4));
    const Optional<Endpoint> target = resolver.resolve_one("127.0.0.1", listener.local_endpoint().port());
    require(target.has_value(), "the listener's own address resolves");
    expect_no_throw(
        [&target] -> void { (void)connected_socket(*target); }, "a resolved endpoint can be connected to"
    );
}

void test_resolve_services_and_uris() {
    const Resolver resolver;

    const Optional<u16> http = resolver.service_port("http");
    if (!http) {
        skip("this host has no service database to look http up in");
    }
    expect_eq(*http, 80, "http is port 80");

    const Optional<u16> unknown = resolver.service_port("definitely-not-a-service");
    expect(!unknown.has_value(), "an unknown service resolves to nothing");

    // A URI with no explicit port falls back to its scheme as the service name.
    const Vector<Endpoint> implied = resolver.resolve(Uri("http://127.0.0.1/index.html"));
    require(!implied.empty(), "a URI authority resolves");
    expect_eq(implied.front().port(), 80, "the scheme supplies the port");

    const Vector<Endpoint> explicit_port = resolver.resolve(Uri("http://127.0.0.1:8080/"));
    require(!explicit_port.empty(), "a URI with a port resolves");
    expect_eq(explicit_port.front().port(), 8080, "an explicit port wins over the scheme");

    expect_throws<InvalidArgumentException>(
        [&resolver] -> void { (void)resolver.resolve(Uri("/just/a/path")); },
        "a URI with no authority cannot be resolved"
    );
}

void test_resolve_reverse_and_host_name() {
    const Resolver resolver;

    const String name = Resolver::host_name();
    expect(!name.empty(), "the local host has a name");

    // A reverse record is optional, so this checks shape rather than content.
    const Optional<String> reverse = resolver.reverse(IPAddress(IPv4Address::LOOPBACK));
    if (reverse) {
        expect(!reverse->empty(), "a reverse lookup that succeeds returns a name");
    }

    const Optional<String> undocumented = resolver.reverse(IPAddress(IPv4Address(192, 0, 2, 1)));
    expect(
        !undocumented.has_value() || !undocumented->empty(),
        "an address with no reverse record resolves to nothing rather than an empty name"
    );
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Socket.lifecycle", test_socket_lifecycle},
        {"Socket.closed_operations", test_socket_closed_operations},
        {"Socket.tcp_roundtrip", test_tcp_roundtrip},
        {"Socket.tcp_bulk_transfer", test_tcp_bulk_transfer},
        {"Socket.udp_roundtrip", test_udp_roundtrip},
        {"Socket.options", test_socket_options},
        {"Socket.non_blocking", test_non_blocking},
        {"Socket.receive_timeout", test_receive_timeout},
        {"Socket.connection_errors", test_connection_errors},
        {"Socket.send_after_peer_close", test_send_after_peer_close},
        {"Socket.ipv6_roundtrip", test_ipv6_roundtrip},
        {"Socket.dual_stack", test_dual_stack},
        {"Resolver.literals", test_resolve_literals},
        {"Resolver.names", test_resolve_names},
        {"Resolver.services_and_uris", test_resolve_services_and_uris},
        {"Resolver.reverse_and_host_name", test_resolve_reverse_and_host_name},
    });
}
