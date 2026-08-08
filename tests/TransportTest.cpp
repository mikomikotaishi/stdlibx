import stdx;

using stdx::collections::Vector;
using stdx::meta::IsSameValue;
using stdx::net::ByteReader;
using stdx::net::ByteStream;
using stdx::net::ByteWriter;
using stdx::net::Endpoint;
using stdx::net::Event;
using stdx::net::IPAddress;
using stdx::net::IPv4Address;
using stdx::net::IPv6Address;
using stdx::net::Interest;
// Poller exists only where a backend does, so a platform without one must not
// name it. Darwin belongs in the list now that kqueue is implemented; it was
// excluded when the macOS build was first made to work, and only epoll and
// WSAPoll existed.
#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)
using stdx::net::Poller;
#endif
using stdx::net::Socket;
using stdx::net::SocketException;
using stdx::net::SocketTimeoutException;
using stdx::net::SocketView;
using stdx::net::TcpListener;
using stdx::net::TcpStream;
using stdx::net::UdpSocket;

using namespace stdx::test;

/**
 * @brief A loopback endpoint on a port the OS picks.
 */
[[nodiscard]]
static Endpoint loopback_any_port(IPAddress::Family family = IPAddress::Family::IPV4) {
    return Endpoint(IPAddress::loopback(family), 0);
}

/**
 * @brief A read-only byte view over @p text.
 */
[[nodiscard]]
static Span<const byte> bytes_of(StringView text) noexcept {
    return Span<const byte>(reinterpret_cast<const byte*>(text.data()), text.size());
}

/**
 * @brief The bytes of @p buffer read back as text.
 */
[[nodiscard]]
static String text_of(Span<const byte> buffer) {
    return String(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}

/**
 * @brief A ByteStream backed by a buffer instead of a socket.
 *
 * The reason the concept exists. A protocol codec written against ByteStream can
 * be driven by this in a unit test - no port, no peer, no timing - and the same
 * code then runs over a TcpStream unchanged. It also demonstrates that
 * conformance is reachable by types this library does not own.
 */
class MemoryStream {
public:
    static constexpr bool IS_BYTE_STREAM = true;
private:
    String _incoming; ///< Bytes waiting to be read.
    String _outgoing; ///< Bytes that have been written.
    usize _read = 0; ///< How much of _incoming has been handed out.
    bool _stalled = false; ///< Whether the next send should report "not now".
public:
    explicit MemoryStream(StringView incoming):
        _incoming{incoming} {}

    void stall(bool stalled) noexcept {
        _stalled = stalled;
    }

    [[nodiscard]]
    StringView written() const noexcept {
        return StringView(_outgoing);
    }

    Optional<usize> try_send(Span<const byte> buffer) {
        if (_stalled) {
            return nullopt;
        }
        _outgoing += text_of(buffer);
        return buffer.size();
    }

    Optional<usize> try_receive(Span<byte> buffer) {
        const usize remaining = _incoming.size() - _read;
        if (remaining == 0) {
            return 0uz;
        }
        const usize count = remaining < buffer.size() ? remaining : buffer.size();
        for (usize index = 0uz; index < count; ++index) {
            buffer[index] = static_cast<byte>(_incoming[_read + index]);
        }
        _read += count;
        return count;
    }
};

// The claims the split was made for, checked at compile time.
static_assert(ByteStream<TcpStream>, "a connected TCP socket is a byte stream");
static_assert(ByteStream<MemoryStream>, "so is a buffer that says it is one");
static_assert(!ByteStream<UdpSocket>, "a datagram socket is not, though it has the shape of one");
static_assert(!ByteStream<Socket>, "nor is a bare Socket, which might be either or neither");

// A full stream satisfies each half, and the opt-in gates both halves rather
// than the pair: a UdpSocket has try_send and try_receive and is neither.
static_assert(ByteReader<TcpStream> && ByteWriter<TcpStream>, "a stream reads and writes");
static_assert(!ByteReader<UdpSocket> && !ByteWriter<UdpSocket>, "a datagram socket is neither half");

static_assert(!DefaultInitializable<TcpStream>, "a TcpStream is always connected");
static_assert(!DefaultInitializable<TcpListener>, "a TcpListener is always listening");
static_assert(Movable<TcpStream>, "streams move into containers and Optionals");

/**
 * @brief Reads a stream to exhaustion, whatever kind of stream it is.
 *
 * Written against the concept rather than against a class, which is what the
 * HTTP and WebSocket codecs will do. It asks for ByteReader rather than
 * ByteStream because reading is all it does, so it serves a stream whose
 * writing half has already been shut down as well as one that is still open in
 * both directions.
 */
template <ByteReader S>
[[nodiscard]]
static String drain(S& stream) {
    String out;
    Array<byte, 64> buffer{};
    while (true) {
        const Optional<usize> read = stream.try_receive(Span<byte>(buffer));
        if (!read || *read == 0) {
            return out;
        }
        out += text_of(Span<const byte>(buffer.data(), *read));
    }
}

/**
 * @brief Tests that one generic function serves a socket and a test double alike.
 *
 * This is the whole return on the split: the codecs above the transport can be
 * exercised without a network, and the code that passes those tests is the code
 * that runs over a real connection.
 */
void test_byte_stream_is_substitutable() {
    MemoryStream memory("hello from memory");
    expect_eq(drain(memory), "hello from memory", "the generic reader drains a buffer");

    TcpListener listener = TcpListener::bind(loopback_any_port());
    TcpStream client = TcpStream::connect(listener.local_endpoint());
    TcpStream server = listener.accept();

    client.send_all(bytes_of("hello from a socket"));
    client.shutdown(TcpStream::ShutdownMode::WRITE);

    expect_eq(drain(server), "hello from a socket", "the same reader drains a socket");

    // A stall is "not now", not end-of-stream, and the reader must not treat the
    // two alike - which is why try_receive reports 0 and nullopt differently.
    MemoryStream stalled("");
    stalled.stall(true);
    expect(!stalled.try_send(bytes_of("x")).has_value(), "a stalled stream reports no progress");
}

/**
 * @brief Tests the accept path, and that it hands back the right type.
 */
void test_tcp_accept_yields_a_stream() {
    TcpListener listener = TcpListener::bind(loopback_any_port());
    const Endpoint bound = listener.local_endpoint();
    expect(bound.port() != 0, "binding to port 0 yields a real port");
    expect(listener.is_open(), "a bound listener is open");

    listener.socket().set_blocking(false);
    expect(!listener.try_accept().has_value(), "an idle listener accepts nothing");

    TcpStream client = TcpStream::connect(bound);
    Optional<TcpStream> server = nullopt;
    for (i32 attempt = 0; attempt < 100 && !server; ++attempt) {
        server = listener.try_accept();
    }

    expect(server.has_value(), "a pending connection is accepted");
    static_assert(
        IsSameValue<decltype(listener.accept()), TcpStream>,
        "accept() is typed as the connection it returns, not as a bare Socket"
    );

    if (server) {
        expect(server->is_open(), "the accepted stream is open");
        const Optional<Endpoint> peer = client.remote_endpoint();
        expect(peer.has_value() && *peer == bound, "the client is connected to the listener");
        expect(server->local_endpoint() == bound, "the accepted stream keeps the listener's address");
    }
}

/**
 * @brief Tests a TCP round trip and the half-close that ends a request.
 */
void test_tcp_round_trip() {
    TcpListener listener = TcpListener::bind(loopback_any_port());
    TcpStream client = TcpStream::connect(listener.local_endpoint());
    TcpStream server = listener.accept();

    client.set_no_delay(true);
    client.send_all(bytes_of("ping"));

    Array<byte, 16> buffer{};
    const usize read = server.receive(Span<byte>(buffer));
    expect_eq(text_of(Span<const byte>(buffer.data(), read)), "ping", "bytes arrive");

    server.send_all(bytes_of("pong"));
    const usize back = client.receive(Span<byte>(buffer));
    expect_eq(text_of(Span<const byte>(buffer.data(), back)), "pong", "and go back");

    server.shutdown(TcpStream::ShutdownMode::WRITE);
    expect(client.receive(Span<byte>(buffer)) == 0, "a half-close reads as end-of-stream");

    client.close();
    expect(!client.is_open(), "a closed stream owns nothing");
}

/**
 * @brief Tests the addressed datagram operations.
 */
void test_udp_round_trip() {
    UdpSocket listener = UdpSocket::bind(loopback_any_port());
    UdpSocket sender = UdpSocket::unbound();
    const Endpoint bound = listener.local_endpoint();

    sender.send_to(bytes_of("datagram"), bound);

    Array<byte, 32> buffer{};
    const UdpSocket::Received received = listener.receive_from(Span<byte>(buffer));
    expect_eq(
        text_of(Span<const byte>(buffer.data(), received.length)),
        "datagram",
        "the datagram arrives whole"
    );
    expect(received.from.port() != 0, "the sender's ephemeral port is reported");

    listener.send_to(bytes_of("reply"), received.from);
    const UdpSocket::Received back = sender.receive_from(Span<byte>(buffer));
    expect_eq(
        text_of(Span<const byte>(buffer.data(), back.length)),
        "reply",
        "and the reply reaches the sender it named"
    );
}

/**
 * @brief Tests that a connected datagram socket uses send/receive.
 *
 * Connecting a datagram socket opens nothing; it fixes a default peer. Keeping
 * send/receive on UdpSocket for this case is why the type is not simply "the
 * addressed operations".
 */
void test_udp_connected() {
    UdpSocket server = UdpSocket::bind(loopback_any_port());
    UdpSocket client = UdpSocket::bind(loopback_any_port());
    const Endpoint server_address = server.local_endpoint();

    client.connect(server_address);
    client.send(bytes_of("bound"));

    Array<byte, 16> buffer{};
    const UdpSocket::Received received = server.receive_from(Span<byte>(buffer));
    expect_eq(
        text_of(Span<const byte>(buffer.data(), received.length)),
        "bound",
        "a connected datagram socket sends without naming the peer each time"
    );

    server.send_to(bytes_of("back"), received.from);
    const usize read = client.receive(Span<byte>(buffer));
    expect_eq(text_of(Span<const byte>(buffer.data(), read)), "back", "and receives from it");
}

/**
 * @brief Tests that the typed layer still reaches the poller underneath.
 *
 * The reason socket() and native_handle() exist. A reactor needs the descriptor
 * and needs non-blocking mode, and the typed wrappers must not hide either -
 * otherwise the split would have bought type safety by making the layer below
 * unreachable.
 */
void test_typed_sockets_drive_a_poller() {
    TcpListener listener = TcpListener::bind(loopback_any_port());
    listener.socket().set_blocking(false);

    Poller poller;
    poller.add(listener.native_handle(), Interest::READ, 1);

    TcpStream client = TcpStream::connect(listener.local_endpoint());

    bool accepted = false;
    for (i32 attempt = 0; attempt < 50 && !accepted; ++attempt) {
        for (const Event& event: poller.wait(50ms)) {
            if (event.token == 1 && event.readable) {
                Optional<TcpStream> server = listener.try_accept();
                if (server) {
                    accepted = true;
                    client.send_all(bytes_of("through the reactor"));
                    Array<byte, 32> buffer{};
                    const usize read = server->receive(Span<byte>(buffer));
                    expect_eq(
                        text_of(Span<const byte>(buffer.data(), read)),
                        "through the reactor",
                        "a poller-driven accept yields a working stream"
                    );
                }
            }
        }
    }
    expect(accepted, "the listener's readiness reached the poller through socket()");
}

/**
 * @brief Tests that one listener on the IPv6 wildcard serves an IPv4 client.
 *
 * The gap bind_dual_stack was added to close: IPV6_V6ONLY has to be cleared
 * between opening the socket and binding it, and plain bind() exposes no point in
 * that sequence where a caller could do it.
 */
void test_dual_stack_listener() {
    Optional<TcpListener> listener = nullopt;
    try {
        listener = TcpListener::bind_dual_stack(0);
    } catch (const SocketException& _) {
        skip("this host will not serve both families from one socket");
    }

    const u16 port = listener->local_endpoint().port();
    expect(port != 0, "port 0 yields a real port on a dual-stack listener");

    // An IPv4 client, against a socket opened in the IPv6 family.
    TcpStream client = TcpStream::connect(Endpoint(IPAddress(IPv4Address::LOOPBACK), port));
    TcpStream server = listener->accept();

    const Optional<Endpoint> peer = server.remote_endpoint();
    require(peer.has_value(), "the accepted peer has an address");
    expect(peer->address().is_v6(), "the peer is reported in the socket's family");

    const Optional<IPv6Address> mapped = peer->address().to_v6();
    require(mapped.has_value(), "the peer address is an IPv6 address");
    expect(mapped->is_v4_mapped(), "an IPv4 peer arrives IPv4-mapped");
    expect(mapped->to_ipv4() == IPv4Address::LOOPBACK, "the mapping unwraps to the IPv4 client");

    client.send_all(bytes_of("dual"));
    Array<byte, 8> buffer{};
    expect_eq(server.receive(buffer), 4u, "the dual-stack listener carries IPv4 traffic");
}

/**
 * @brief Tests that a descriptor from outside the library can be adopted.
 *
 * from_handle is what a consumer holding only a file descriptor uses - socket
 * activation, an inherited fd, a socketpair - and it is the door that would have
 * to exist before Socket could stop being part of the public surface.
 */
void test_adopting_a_foreign_descriptor() {
    TcpListener listener = TcpListener::bind(loopback_any_port());
    const Endpoint bound = listener.local_endpoint();

    // release() hands the descriptor over exactly as a supervisor would. It is
    // on the wrapper rather than reached through socket(), so handing a
    // connection onwards needs nothing from the raw layer.
    TcpStream connected = TcpStream::connect(bound);
    const Socket::NativeHandle handle = connected.release();
    expect(!connected.is_open(), "the original stream has given up the descriptor");

    TcpStream adopted = TcpStream::from_handle(handle);
    expect(adopted.is_open(), "the adopted stream owns the descriptor");
    expect(adopted.remote_endpoint() == bound, "the adopted stream is still connected");

    TcpStream server = listener.accept();
    adopted.send_all(bytes_of("adopted"));
    Array<byte, 16> buffer{};
    expect_eq(server.receive(buffer), 7u, "the adopted descriptor still carries traffic");

    // The listener's own descriptor round-trips the same way.
    const Socket::NativeHandle listening = listener.release();
    TcpListener readopted = TcpListener::from_handle(listening);
    expect(readopted.local_endpoint() == bound, "the readopted listener keeps its address");

    UdpSocket datagram = UdpSocket::bind(loopback_any_port());
    const Endpoint datagram_bound = datagram.local_endpoint();
    UdpSocket readopted_datagram = UdpSocket::from_handle(datagram.release());
    expect_eq(
        readopted_datagram.local_endpoint(), datagram_bound,
        "a readopted datagram socket keeps its address"
    );
}

/**
 * @brief Tunes a socket, whatever it turned out to be.
 *
 * What SocketView is for. Written once, called below with a listener, a
 * connection, a datagram socket and a bare Socket, and by construction unable to
 * do anything to any of them that depends on which one it got.
 */
static void tune(SocketView socket) {
    socket.set_blocking(false);
    socket.set_receive_buffer_size(64 * 1024);
    expect(socket.is_open(), "a viewed socket is open");
    expect(!socket.take_error().has_value(), "a freshly opened socket has no pending error");
}

// The substance of the narrowing, and the part no runtime test can reach: the
// view has no way to end a descriptor's life, hand it away, or redo any of the
// setup its owner already did. Each of these compiled while socket() still handed
// out the Socket itself.
//
// Named rather than written inline, because a requires-expression only softens
// errors for dependent expressions - `requires { view.close(); }` on a concrete
// type is a hard error, not a false answer. Every one is asserted against Socket
// too, so a misspelled member fails loudly here instead of quietly passing the
// negative assertion for the wrong reason.
template <typename S>
concept Closes = requires (S socket) { socket.close(); };

template <typename S>
concept Releases = requires (S socket) { socket.release(); };

template <typename S>
concept Binds = requires (S socket, const Endpoint& endpoint) { socket.bind(endpoint); };

template <typename S>
concept Connects = requires (S socket, const Endpoint& endpoint) { socket.connect(endpoint); };

template <typename S>
concept Listens = requires (S socket) { socket.listen(128); };

template <typename S>
concept Accepts = requires (S socket) { socket.accept(); };

template <typename S>
concept Sends = requires (S socket, Span<const byte> buffer) { socket.send(buffer); };

static_assert(Closes<Socket> && !Closes<SocketView>, "a view must not close a descriptor it does not own");
static_assert(Releases<Socket> && !Releases<SocketView>, "a view must not take the descriptor from its owner");
static_assert(Binds<Socket> && !Binds<SocketView>, "a view must not rebind an already-bound socket");
static_assert(Connects<Socket> && !Connects<SocketView>, "a view must not reconnect an established socket");
static_assert(Listens<Socket> && !Listens<SocketView>, "a view must not turn a connection into a listener");
static_assert(Accepts<Socket> && !Accepts<SocketView>, "a view must not accept on a possible datagram socket");
static_assert(Sends<Socket> && !Sends<SocketView>, "a view must not move bytes past the wrapper owning the protocol");
static_assert(sizeof(SocketView) == sizeof(void*), "a view costs a pointer");

/**
 * @brief Tests that the socket view tunes a socket without being able to break it.
 *
 * The three wrappers hand out a narrowed view rather than the Socket, so the
 * settings that mean the same thing for a listener, a connection and a datagram
 * socket stay reachable while the operations that would contradict the wrapper's
 * own type do not.
 */
void test_the_socket_view_borrows_without_owning() {
    TcpListener listener = TcpListener::bind(loopback_any_port());
    TcpStream client = TcpStream::connect(listener.local_endpoint());
    TcpStream server = listener.accept();
    UdpSocket datagram = UdpSocket::bind(loopback_any_port());
    Socket bare(IPAddress::Family::IPV4, Socket::Type::STREAM);

    // One function, four kinds of socket, including one that never went through
    // a wrapper - the implicit conversion from Socket& is what makes the last
    // one work.
    tune(listener.socket());
    tune(client.socket());
    tune(datagram.socket());
    tune(bare);

    // Borrowed, not taken: the views above are gone and every owner is intact.
    expect(listener.is_open(), "the listener still owns its descriptor after being viewed");
    expect(client.is_open(), "the stream still owns its descriptor after being viewed");
    expect(datagram.is_open(), "the datagram socket still owns its descriptor after being viewed");
    expect_eq(
        client.socket().native_handle(), client.native_handle(),
        "the view and the wrapper report the same descriptor"
    );

    const i32 before = server.socket().receive_buffer_size();
    expect(before > 0, "a socket reports a receive buffer size through the view");
    server.socket().set_receive_buffer_size(before * 2);
    expect(
        server.socket().receive_buffer_size() >= before,
        "a larger receive buffer asked for through the view is not a smaller one"
    );

    // A setting made through the view reaches the kernel, not just the wrapper.
    server.socket().set_receive_timeout(50ms);
    Array<byte, 8> buffer{};
    expect_throws<SocketTimeoutException>(
        [&server, &buffer] -> void { (void)server.receive(buffer); },
        "a deadline set through the view is the one the receive obeys"
    );
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Transport.byte_stream_is_substitutable", test_byte_stream_is_substitutable},
        {"Transport.dual_stack_listener", test_dual_stack_listener},
        {"Transport.adopting_a_foreign_descriptor", test_adopting_a_foreign_descriptor},
        {"Transport.the_socket_view_borrows_without_owning", test_the_socket_view_borrows_without_owning},
        {"Transport.tcp_accept_yields_a_stream", test_tcp_accept_yields_a_stream},
        {"Transport.tcp_round_trip", test_tcp_round_trip},
        {"Transport.udp_round_trip", test_udp_round_trip},
        {"Transport.udp_connected", test_udp_connected},
        {"Transport.typed_sockets_drive_a_poller", test_typed_sockets_drive_a_poller},
    });
}
