import stdx;

// Matches the guard on Poller itself. The Windows backend is written but has
// never been executed - no Windows host, no CI runner - so this file is the
// thing that would produce the first evidence either way.
#if defined(_WIN32) || defined(__linux__)
using stdx::collections::Vector;
using stdx::net::Endpoint;
using stdx::net::Event;
using stdx::net::IPAddress;
using stdx::net::Interest;
using stdx::net::Poller;
using stdx::net::PollerException;
using stdx::net::Socket;
using stdx::time::Instant;
using stdx::time::Milliseconds;
using stdx::time::SteadyClock;

using namespace stdx::test;

/**
 * @brief A connected loopback pair, plus the listener that produced it.
 *
 * The listener is kept alive alongside the pair: closing it early is harmless
 * to an established connection, but keeping it makes the accepted socket's
 * lifetime obvious at each call site.
 */
struct LoopbackPair {
    Socket listener; ///< The listening socket the connection came from.
    Socket client; ///< The connecting end.
    Socket server; ///< The accepted end.
};

/**
 * @brief Opens a connected TCP pair on the loopback address.
 * @return The pair, both ends non-blocking.
 *
 * Non-blocking is how a Poller is meant to be used - a `try_` call that finds
 * nothing reports an empty Optional rather than parking the reactor thread - so
 * the tests exercise that pairing rather than a blocking shortcut.
 */
[[nodiscard]]
static LoopbackPair connected_pair() {
    Socket listener(IPAddress::Family::IPV4, Socket::Type::STREAM);
    listener.set_reuse_address(true);
    listener.bind(Endpoint(IPAddress::loopback(IPAddress::Family::IPV4), 0));
    listener.listen();

    Socket client(IPAddress::Family::IPV4, Socket::Type::STREAM);
    client.connect(listener.local_endpoint());

    Socket server = listener.accept();
    client.set_blocking(false);
    server.set_blocking(false);
    return LoopbackPair{Ops::move(listener), Ops::move(client), Ops::move(server)};
}

/**
 * @brief A read-only byte view over @p text.
 * @param text The text to view.
 * @return A span of bytes over @p text.
 */
[[nodiscard]]
static Span<const byte> bytes_of(StringView text) noexcept {
    return Span<const byte>(reinterpret_cast<const byte*>(text.data()), text.size());
}

/**
 * @brief The event carrying @p token, if @p events holds one.
 * @param events The events a wait reported.
 * @param token The token to look for.
 * @return The matching event, or an empty Optional.
 */
[[nodiscard]]
static Optional<Event> find(Span<const Event> events, u64 token) noexcept {
    for (const Event& event: events) {
        if (event.token == token) {
            return event;
        }
    }
    return nullopt;
}

/**
 * @brief Polls until @p token is reported, or @p budget runs out.
 * @param poller The poller to wait on.
 * @param token The token to wait for.
 * @param budget How long to keep trying.
 * @return The matching event, or an empty Optional if it never arrived.
 *
 * Loopback delivery is prompt but not instantaneous, and a wait can end early
 * for reasons of its own. Retrying inside a budget keeps the tests honest about
 * that without making them depend on a single wait catching the event.
 */
[[nodiscard]]
static Optional<Event> poll_for(Poller& poller, u64 token, Milliseconds budget) {
    const Instant<SteadyClock> deadline = SteadyClock::now() + budget;
    do {
        if (const Optional<Event> found = find(poller.wait(20ms), token)) {
            return found;
        }
    } while (SteadyClock::now() < deadline);
    return nullopt;
}

/**
 * @brief How long @p work takes.
 * @param work The callable to time.
 * @return The elapsed wall time.
 */
template <typename Work>
[[nodiscard]]
static Milliseconds elapsed_of(Work&& work) {
    const Instant<SteadyClock> started = SteadyClock::now();
    work();
    return stdx::time::duration_cast<Milliseconds>(SteadyClock::now() - started);
}

void test_poller_lifecycle() {
    Poller poller;
    expect(poller.is_open(), "a fresh poller owns its backend");
    #ifndef _WIN32
    // Only epoll has a descriptor to expose; WSAPoll keeps no kernel state.
    expect(poller.native_handle() >= 0, "a fresh poller exposes its descriptor");
    #endif
    expect(poller.registered() == 0, "a fresh poller watches nothing");

    LoopbackPair pair = connected_pair();
    poller.add(pair.server.native_handle(), Interest::READ, 1);
    expect(poller.registered() == 1, "add() counts the descriptor");

    poller.remove(pair.server.native_handle());
    expect(poller.registered() == 0, "remove() uncounts the descriptor");

    Poller moved = Ops::move(poller);
    expect(moved.is_open(), "the move target owns the epoll instance");
    expect(!poller.is_open(), "the moved-from poller owns nothing");
}

/**
 * @brief Tests that a wait with nothing to report honours its deadline.
 *
 * The zero case matters as much as the blocking one: a reactor polls with no
 * timeout when it has queued work to get back to, and a zero that blocked even
 * briefly would turn every such turn into a stall.
 */
void test_poller_timeout() {
    Poller poller;

    const Milliseconds immediate = elapsed_of([&poller] -> void {
        expect(poller.wait(0ms).empty(), "a zero-timeout wait on an idle poller reports nothing");
    });
    expect(immediate < 50ms, "a zero timeout returns without blocking");

    const Milliseconds waited = elapsed_of([&poller] -> void {
        expect(poller.wait(80ms).empty(), "an expired wait reports nothing");
    });
    expect(waited >= 50ms, "a wait with a deadline blocks until it lapses");
}

/**
 * @brief Tests that data arriving on a watched socket is reported under its token.
 */
void test_poller_readable() {
    LoopbackPair pair = connected_pair();
    Poller poller;
    poller.add(pair.server.native_handle(), Interest::READ, 42);

    expect(
        !find(poller.wait(20ms), 42).has_value(),
        "a quiet connection reports no readiness"
    );

    pair.client.send(bytes_of("ping"));

    const Optional<Event> event = poll_for(poller, 42, 500ms);
    expect(event.has_value(), "data on the peer makes the socket readable");
    if (event) {
        expect(event->readable, "the event reports readability");
        expect(!event->writable, "an Interest in reading alone reports no writability");
        expect(!event->error, "an ordinary read carries no error");
    }

    Array<byte, 8> buffer{};
    const Optional<usize> read = pair.server.try_receive(Span<byte>(buffer));
    expect(read.has_value() && *read == 4, "the readiness was real - the bytes are there");
}

/**
 * @brief Tests that readiness is level-triggered rather than edge-triggered.
 *
 * This is the difference between a forgiving reactor and a hanging one. Under
 * edge triggering a caller that stops reading before EWOULDBLOCK is never told
 * again, and the connection stalls with data sitting in the kernel.
 */
void test_poller_level_triggered() {
    LoopbackPair pair = connected_pair();
    Poller poller;
    poller.add(pair.server.native_handle(), Interest::READ, 5);

    pair.client.send(bytes_of("payload"));
    expect(poll_for(poller, 5, 500ms).has_value(), "the first wait reports the data");

    // Deliberately no read in between.
    expect(
        poll_for(poller, 5, 500ms).has_value(),
        "undrained data is reported again rather than only on arrival"
    );
}

/**
 * @brief Tests that writability is reported, and that modify() changes both interest and token.
 */
void test_poller_writable_and_modify() {
    LoopbackPair pair = connected_pair();
    Poller poller;
    poller.add(pair.client.native_handle(), Interest::READ, 10);

    expect(
        !find(poller.wait(20ms), 10).has_value(),
        "an idle socket registered for reading reports nothing, though it could be written"
    );

    poller.modify(pair.client.native_handle(), Interest::WRITE, 11);

    const Optional<Event> event = poll_for(poller, 11, 500ms);
    expect(event.has_value(), "modify() takes effect, under the new token");
    expect(!find(poller.wait(20ms), 10).has_value(), "the old token is not reported again");
    if (event) {
        expect(event->writable, "an empty send buffer is writable");
    }
}

/**
 * @brief Tests that a peer's half-close is distinguished from a full hang-up.
 *
 * RD_HUP is only delivered when it was registered for, which is why the poller
 * folds it into every read interest rather than exposing it as a choice. A
 * reactor that cannot see a half-close treats the endlessly readable socket as
 * live and never closes it.
 */
void test_poller_read_hangup() {
    LoopbackPair pair = connected_pair();
    Poller poller;
    poller.add(pair.server.native_handle(), Interest::READ, 77);

    pair.client.shutdown(Socket::ShutdownMode::WRITE);

    const Optional<Event> event = poll_for(poller, 77, 500ms);
    expect(event.has_value(), "a peer's half-close wakes the wait");
    if (event) {
        #ifdef _WIN32
        // WSAPoll has no EPOLLRDHUP, so read_hangup is documented as always
        // false there. The half-close still has to surface as readability, or a
        // reactor would never learn the request ended.
        expect(!event->read_hangup, "Windows cannot distinguish a half-close");
        #else
        expect(event->read_hangup, "the half-close is reported as a read hang-up");
        #endif
        expect(event->readable, "the socket is readable, and will read as end-of-stream");
        expect(!event->hangup, "one direction closing is not a full hang-up");
    }

    Array<byte, 8> buffer{};
    const Optional<usize> read = pair.server.try_receive(Span<byte>(buffer));
    expect(read.has_value() && *read == 0, "the read that follows reports end-of-stream");
}

/**
 * @brief Tests that a removed descriptor stops being reported.
 */
void test_poller_remove() {
    LoopbackPair pair = connected_pair();
    Poller poller;
    poller.add(pair.server.native_handle(), Interest::READ, 3);
    poller.remove(pair.server.native_handle());

    pair.client.send(bytes_of("ignored"));

    expect(
        !poll_for(poller, 3, 100ms).has_value(),
        "a removed descriptor is not reported, even with data waiting"
    );
}

/**
 * @brief Tests that wake() ends a wait early, and that the wake-up is consumed.
 *
 * Deliberately single-threaded. A pending eventfd count wakes the next wait
 * whichever thread wrote it, so this exercises the whole mechanism - the write,
 * the report, and the drain - without a sleep race deciding whether the test
 * passes. The drain half is the part worth pinning down: an undrained eventfd
 * would leave the poller spinning at full speed forever.
 */
void test_poller_wake() {
    Poller poller;
    poller.wake();

    const Milliseconds woken = elapsed_of([&poller] -> void {
        expect(poller.wait(3000ms).empty(), "a wake reports no events of its own");
    });
    expect(woken < 1000ms, "a pending wake ends the wait rather than the deadline");

    const Milliseconds after = elapsed_of([&poller] -> void {
        (void)poller.wait(80ms);
    });
    expect(after >= 50ms, "the wake-up was drained and does not wake every later wait");
}

/**
 * @brief Tests that misuse is refused rather than quietly accepted.
 */
void test_poller_misuse() {
    LoopbackPair pair = connected_pair();
    Poller poller;

    expect_throws<PollerException>(
        [&poller, &pair] -> void {
            poller.add(pair.server.native_handle(), Interest::READ, Poller::WAKE_TOKEN);
        },
        "the reserved wake token cannot be claimed by a caller"
    );

    expect_throws<PollerException>(
        [&poller, &pair] -> void {
            poller.remove(pair.server.native_handle());
        },
        "removing a descriptor that was never added is refused"
    );

    poller.add(pair.server.native_handle(), Interest::READ, 1);
    expect_throws<PollerException>(
        [&poller, &pair] -> void {
            poller.add(pair.server.native_handle(), Interest::READ, 2);
        },
        "adding the same descriptor twice is refused"
    );
}

/**
 * @brief Tests that many descriptors are reported in one wait, each under its own token.
 *
 * The point of a poller in one assertion: eight connections, one thread, one
 * blocking call. It also pushes past the initial event-buffer size so the
 * growth path runs.
 */
void test_poller_many_sockets() {
    constexpr usize COUNT = 24;
    Vector<LoopbackPair> pairs;
    Poller poller;

    for (usize index = 0; index < COUNT; ++index) {
        pairs.push_back(connected_pair());
        poller.add(pairs.back().server.native_handle(), Interest::READ, static_cast<u64>(index));
    }
    expect(poller.registered() == COUNT, "every connection is registered");

    for (LoopbackPair& pair: pairs) {
        pair.client.send(bytes_of("x"));
    }

    Vector<bool> seen(COUNT, false);
    usize remaining = COUNT;
    const auto deadline = SteadyClock::now() + 2000ms;
    while (remaining > 0 && SteadyClock::now() < deadline) {
        for (const Event& event: poller.wait(50ms)) {
            const usize index = static_cast<usize>(event.token);
            if (index < COUNT && !seen[index]) {
                seen[index] = true;
                --remaining;
            }
        }
    }

    expect(remaining == 0, "one thread in one wait loop serves every connection");
}
#endif

int main(int argc, char* argv[]) {
    #if defined(_WIN32) || defined(__linux__)
    return run(argc, argv, {
        {"Poller.lifecycle", test_poller_lifecycle},
        {"Poller.timeout", test_poller_timeout},
        {"Poller.readable", test_poller_readable},
        {"Poller.level_triggered", test_poller_level_triggered},
        {"Poller.writable_and_modify", test_poller_writable_and_modify},
        {"Poller.read_hangup", test_poller_read_hangup},
        {"Poller.remove", test_poller_remove},
        {"Poller.wake", test_poller_wake},
        {"Poller.misuse", test_poller_misuse},
        {"Poller.many_sockets", test_poller_many_sockets},
    });
    #else
    (void)argc;
    (void)argv;
    return 0;
    #endif
}
