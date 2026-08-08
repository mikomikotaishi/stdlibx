import stdx;

using stdx::collections::Vector;
using stdx::net::Endpoint;
using stdx::net::Event;
using stdx::net::IPAddress;
using stdx::net::Interest;
using stdx::net::Reactor;
using stdx::net::ReactorException;
using stdx::net::Socket;
using stdx::net::TimerId;
using stdx::thread::Thread;
using stdx::time::Instant;
using stdx::time::Milliseconds;
using stdx::time::SteadyClock;

using namespace stdx::test;

/**
 * @struct LoopbackPair
 * @brief A connected loopback pair, plus the listener that produced it.
 */
struct LoopbackPair {
    Socket listener; ///< The listening socket the connection came from.
    Socket client; ///< The connecting end.
    Socket server; ///< The accepted end.
};

/**
 * @brief Opens a connected TCP pair on the loopback address.
 * @return The pair, both ends non-blocking.
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
    return LoopbackPair {
        .listener = Ops::move(listener),
        .client = Ops::move(client),
        .server = Ops::move(server)
    };
}

/**
 * @brief A read-only byte view over @p text.
 * @param text The text to view as bytes.
 * @return A span of bytes over the text.
 */
[[nodiscard]]
static Span<const byte> bytes_of(StringView text) noexcept {
    return Span<const byte>(reinterpret_cast<const byte*>(text.data()), text.size());
}

/**
 * @brief Turns the loop until @p done holds, or @p budget runs out.
 * @tparam Pred The predicate type.
 * @param reactor The reactor to drive.
 * @param done The condition being waited for.
 * @param budget How long to keep turning.
 * @return Whether @p done ended up holding.
 *
 * Loopback delivery is prompt but not instantaneous, so a test that ran a
 * single turn would be a race rather than an assertion.
 */
template <typename Pred>
[[nodiscard]]
static bool run_until(Reactor& reactor, Pred done, Milliseconds budget) {
    const Instant<SteadyClock> deadline = SteadyClock::now() + budget;
    while (!done() && SteadyClock::now() < deadline) {
        reactor.run_once(20ms);
    }
    return done();
}

/**
 * @brief How long @p work takes.
 * @tparam Work The callable type.
 * @param work The work to time.
 */
template <typename Work>
[[nodiscard]]
static Milliseconds elapsed_of(Work&& work) {
    const Instant<SteadyClock> started = SteadyClock::now();
    work();
    return stdx::time::duration_cast<Milliseconds>(SteadyClock::now() - started);
}

/**
 * @brief Tests registration bookkeeping on a reactor nobody has driven yet.
 */
void test_reactor_lifecycle() {
    LoopbackPair pair = connected_pair();
    Reactor reactor;

    expect(reactor.registered() == 0, "a fresh reactor watches nothing");
    expect(reactor.pending_timers() == 0, "a fresh reactor has no timers");
    expect(!reactor.is_running(), "a fresh reactor is not running");

    reactor.add(pair.server.native_handle(), Interest::READ, [](const Event& _) -> void {});
    expect(reactor.registered() == 1, "add() counts the descriptor");
    expect(reactor.watches(pair.server.native_handle()), "the descriptor is watched");
    expect(!reactor.watches(pair.client.native_handle()), "an unregistered descriptor is not");

    reactor.remove(pair.server.native_handle());
    expect(reactor.registered() == 0, "remove() uncounts the descriptor");
    expect(!reactor.watches(pair.server.native_handle()), "and stops watching it");
}

/**
 * @brief Tests that data on a watched socket reaches the handler registered with it.
 */
void test_readiness_reaches_its_handler() {
    LoopbackPair pair = connected_pair();
    Reactor reactor;

    usize calls = 0;
    bool readable = false;
    reactor.add(pair.server.native_handle(), Interest::READ, [&](const Event& event) -> void {
        ++calls;
        readable = event.readable;
        Array<byte, 8> buffer = {};
        static_cast<void>(pair.server.try_receive(Span<byte>(buffer)));
    });

    expect(!reactor.run_once(20ms), "a quiet turn dispatches nothing");

    pair.client.send_all(bytes_of("ping"));

    expect(run_until(reactor, [&] -> bool { return calls > 0; }, 500ms), "the handler ran");
    expect(readable, "the event it was handed reports readability");

    reactor.remove(pair.server.native_handle());
}

/**
 * @brief Tests that modify() changes the interest without disturbing the handler.
 */
void test_modify_keeps_the_handler() {
    LoopbackPair pair = connected_pair();
    Reactor reactor;

    usize writable_calls = 0;
    reactor.add(pair.client.native_handle(), Interest::READ, [&](const Event& event) -> void {
        if (event.writable) {
            ++writable_calls;
        }
    });

    expect(!reactor.run_once(20ms), "an idle socket watched for reading reports nothing");

    reactor.modify(pair.client.native_handle(), Interest::WRITE);
    expect(
        run_until(reactor, [&] -> bool { return writable_calls > 0; }, 500ms),
        "the same handler now sees writability"
    );

    reactor.remove(pair.client.native_handle());
}

/**
 * @brief Tests that a handler can be swapped without touching the kernel registration.
 *
 * Handing a connection from one state machine to the next - a request parser to
 * a response writer, plaintext to TLS - is a handler swap. Making it a
 * re-registration would cost two syscalls and a window in which the descriptor
 * is unwatched.
 */
void test_set_handler_swaps_the_callback() {
    LoopbackPair pair = connected_pair();
    Reactor reactor;

    usize first = 0;
    usize second = 0;
    reactor.add(pair.server.native_handle(), Interest::READ, [&](const Event& _) -> void {
        ++first;
    });
    reactor.set_handler(pair.server.native_handle(), [&](const Event& _) -> void {
        ++second;
        Array<byte, 8> buffer = {};
        static_cast<void>(pair.server.try_receive(Span<byte>(buffer)));
    });

    pair.client.send_all(bytes_of("ping"));
    expect(run_until(reactor, [&] -> bool { return second > 0; }, 500ms), "the new handler ran");
    expect(first == 0, "the replaced handler did not");
    expect(reactor.registered() == 1, "the descriptor stayed registered throughout");

    reactor.remove(pair.server.native_handle());
}

/**
 * @brief Tests that a handler may remove its own descriptor and keep running.
 *
 * The slot's release is deferred to the end of the turn precisely so this is
 * legal: freeing it at the point of the call would destroy the callable that is
 * still executing.
 */
void test_a_handler_may_remove_itself() {
    LoopbackPair pair = connected_pair();
    Reactor reactor;

    const Socket::NativeHandle handle = pair.server.native_handle();
    usize calls = 0;
    bool finished = false;
    reactor.add(handle, Interest::READ, [&](const Event& _) -> void {
        ++calls;
        reactor.remove(handle);
        finished = true;
    });

    pair.client.send_all(bytes_of("bye"));
    expect(run_until(reactor, [&] -> bool { return calls > 0; }, 500ms), "the handler ran");
    expect(finished, "it kept running past its own removal");
    expect(reactor.registered() == 0, "the descriptor is gone");

    // The bytes were never read, so the socket stays readable. Were the
    // registration still live, level triggering would re-enter the handler on
    // every subsequent turn.
    reactor.run_once(20ms);
    expect(calls == 1, "a removed handler is not re-entered by undrained data");
}

/**
 * @brief Tests that removing a descriptor cancels an event already collected for it.
 *
 * The hazard the token generation exists for. Both sockets are made readable
 * before a single turn runs, so the turn begins holding an event for each;
 * whichever handler runs first removes the other. Without the generation check
 * the second handler would still be called - on a descriptor its owner has
 * already decided is finished, and may well have closed.
 */
void test_a_removed_peer_does_not_run_in_the_same_turn() {
    LoopbackPair first = connected_pair();
    LoopbackPair second = connected_pair();
    Reactor reactor;

    const Socket::NativeHandle left = first.server.native_handle();
    const Socket::NativeHandle right = second.server.native_handle();
    usize ran = 0;

    const auto handler = [&](Socket::NativeHandle self, Socket::NativeHandle other) {
        return [&, self, other](const Event& _) -> void {
            ++ran;
            if (reactor.watches(other)) {
                reactor.remove(other);
            }
            if (reactor.watches(self)) {
                reactor.remove(self);
            }
        };
    };

    reactor.add(left, Interest::READ, handler(left, right));
    reactor.add(right, Interest::READ, handler(right, left));

    first.client.send_all(bytes_of("x"));
    second.client.send_all(bytes_of("x"));
    // Long enough that both arrivals are certain to be in the same wait.
    Thread::sleep_for(100ms);

    reactor.run_once(200ms);
    expect(ran == 1, "the peer removed mid-turn did not have its collected event dispatched");
    expect(reactor.registered() == 0, "both descriptors were removed");
}

/**
 * @brief Tests that timers run in deadline order rather than scheduling order.
 */
void test_timers_fire_in_deadline_order() {
    Reactor reactor;
    Vector<i32> order;

    static_cast<void>(reactor.after(120ms, [&] -> void { order.push_back(3); }));
    static_cast<void>(reactor.after(40ms, [&] -> void { order.push_back(1); }));
    static_cast<void>(reactor.after(80ms, [&] -> void { order.push_back(2); }));
    expect(reactor.pending_timers() == 3, "three timers are pending");

    reactor.run();

    expect(reactor.pending_timers() == 0, "the loop ran them all");
    expect(order.size() == 3, "every timer fired");
    if (order.size() == 3) {
        expect(order[0] == 1 && order[1] == 2 && order[2] == 3, "they fired earliest deadline first");
    }
}

/**
 * @brief Tests that a pending timer can be cancelled, and that cancelling twice is harmless.
 */
void test_cancel_stops_a_timer() {
    Reactor reactor;
    bool fired = false;

    const TimerId id = reactor.after(30ms, [&] -> void { fired = true; });
    expect(id.is_valid(), "a scheduled timer has an id");
    expect(reactor.cancel(id), "cancelling a pending timer succeeds");
    expect(!reactor.cancel(id), "cancelling it again answers false rather than throwing");
    expect(reactor.pending_timers() == 0, "nothing is left pending");

    reactor.run();
    expect(!fired, "a cancelled timer never fires");
    expect(!reactor.cancel(TimerId()), "a default-constructed id cancels nothing");
}

/**
 * @brief Tests that an id held past its firing cannot cancel the timer that reuses its slot.
 *
 * Timer slots are recycled, so an id that was only an index would silently name
 * somebody else's timer after its own had fired. This is the case that makes
 * "cancel the read timeout once the response arrives" safe to write without
 * first proving the timeout has not just expired.
 */
void test_a_spent_timer_id_cannot_cancel_its_successor() {
    Reactor reactor;

    bool first_fired = false;
    static_cast<void>(reactor.after(10ms, [&] -> void { first_fired = true; }));
    const TimerId spent = reactor.after(10ms, [&] -> void {});
    reactor.run();
    expect(first_fired, "the first round of timers fired");

    bool second_fired = false;
    static_cast<void>(reactor.after(10ms, [&] -> void { second_fired = true; }));
    expect(!reactor.cancel(spent), "the spent id cancels nothing");
    expect(reactor.pending_timers() == 1, "the new timer is untouched");

    reactor.run();
    expect(second_fired, "the timer that reused the slot still fired");
}

/**
 * @brief Tests that a deadline bounds a wait that would otherwise block forever.
 *
 * The whole reason timers live in the reactor rather than beside it: with
 * nothing registered and no caller timeout, the wait's only limit is the
 * earliest deadline.
 */
void test_a_timer_bounds_an_unbounded_wait() {
    Reactor reactor;
    bool fired = false;
    static_cast<void>(reactor.after(60ms, [&] -> void { fired = true; }));

    const Milliseconds waited = elapsed_of([&] -> void {
        expect(reactor.run_once(), "the turn reported that something came due");
    });

    expect(fired, "the timer fired");
    expect(waited >= 40ms, "the wait blocked rather than spinning");
    expect(waited < 1000ms, "and ended at the deadline rather than hanging");
}

/**
 * @brief Tests that a timer scheduled from a callback waits for a later turn.
 *
 * Every due timer is taken off the heap before any of them runs, which is what
 * stops a self-rescheduling callback from being re-collected in the turn that
 * just ran it and starving the loop of its next wait.
 */
void test_a_timer_scheduled_from_a_callback_waits_for_the_next_turn() {
    Reactor reactor;
    usize fired = 0;

    static_cast<void>(reactor.after(10ms, [&] -> void {
        ++fired;
        static_cast<void>(reactor.after(0ms, [&] -> void { ++fired; }));
    }));

    reactor.run_once(500ms);
    expect(fired == 1, "the timer scheduled during the turn did not also run in it");
    expect(reactor.pending_timers() == 1, "it is pending instead");

    reactor.run();
    expect(fired == 2, "and runs on a later turn");
}

/**
 * @brief Tests that a loop with nothing left to wait for returns instead of blocking.
 */
void test_run_returns_when_nothing_is_left() {
    Reactor reactor;
    const Milliseconds waited = elapsed_of([&] -> void { reactor.run(); });
    expect(waited < 200ms, "a loop with no descriptors and no timers returns at once");
    expect(!reactor.is_running(), "and reports that it is not running");
}

/**
 * @brief Tests that stop() breaks a loop out of a wait from another thread.
 */
void test_stop_ends_a_run_from_another_thread() {
    LoopbackPair pair = connected_pair();
    Reactor reactor;

    // Something registered, so the loop has a reason to keep waiting and cannot
    // fall out on its own.
    reactor.add(pair.server.native_handle(), Interest::READ, [](const Event& _) -> void {});

    Thread stopper([&reactor] -> void {
        Thread::sleep_for(80ms);
        reactor.stop();
    });

    const Milliseconds waited = elapsed_of([&] -> void { reactor.run(); });
    stopper.join();

    expect(waited >= 40ms, "the loop was genuinely waiting");
    expect(waited < 3000ms, "stop() broke it out of the wait");
    expect(!reactor.is_running(), "the loop is no longer running");

    reactor.remove(pair.server.native_handle());
}

/**
 * @brief Tests that a stop issued while no loop is running does not end the next one.
 */
void test_a_stray_stop_does_not_end_a_later_run() {
    Reactor reactor;
    reactor.stop();

    bool fired = false;
    static_cast<void>(reactor.after(20ms, [&] -> void { fired = true; }));
    reactor.run();
    expect(fired, "the next run cleared the stale stop rather than obeying it");
}

/**
 * @brief Tests that incoherent registration is refused rather than half-applied.
 */
void test_misuse_is_refused() {
    LoopbackPair pair = connected_pair();
    Reactor reactor;

    const Socket::NativeHandle handle = pair.server.native_handle();
    const Socket::NativeHandle stranger = pair.client.native_handle();

    reactor.add(handle, Interest::READ, [](const Event& _) -> void {});

    expect_throws<ReactorException>(
        [&] -> void { reactor.add(handle, Interest::READ, [](const Event& _) -> void {}); },
        "registering the same descriptor twice is refused"
    );
    expect_throws<ReactorException>(
        [&] -> void { reactor.remove(stranger); },
        "removing an unregistered descriptor is refused"
    );
    expect_throws<ReactorException>(
        [&] -> void { reactor.modify(stranger, Interest::WRITE); },
        "modifying an unregistered descriptor is refused"
    );
    expect_throws<ReactorException>(
        [&] -> void { reactor.set_handler(stranger, [](const Event& _) -> void {}); },
        "swapping the handler of an unregistered descriptor is refused"
    );

    expect(reactor.registered() == 1, "a refused call left the registration alone");
    reactor.remove(handle);
}

/**
 * @brief Tests that driving the loop from inside a handler is refused.
 *
 * A nested turn would call wait() while the outer turn still holds a view of
 * the events it is dispatching, and that storage is reused by every wait.
 */
void test_the_loop_refuses_re_entry() {
    Reactor reactor;
    bool refused = false;

    static_cast<void>(reactor.after(10ms, [&] -> void {
        try {
            reactor.run_once(0ms);
        } catch (const ReactorException& _) {
            refused = true;
        }
    }));

    reactor.run();
    expect(refused, "a turn started from inside a turn is refused");
    expect(!reactor.is_running(), "and the refusal left the loop consistent");
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"Reactor.lifecycle", test_reactor_lifecycle},
        {"Reactor.readiness_reaches_its_handler", test_readiness_reaches_its_handler},
        {"Reactor.modify_keeps_the_handler", test_modify_keeps_the_handler},
        {"Reactor.set_handler_swaps_the_callback", test_set_handler_swaps_the_callback},
        {"Reactor.a_handler_may_remove_itself", test_a_handler_may_remove_itself},
        {"Reactor.a_removed_peer_does_not_run", test_a_removed_peer_does_not_run_in_the_same_turn},
        {"Reactor.timers_fire_in_deadline_order", test_timers_fire_in_deadline_order},
        {"Reactor.cancel_stops_a_timer", test_cancel_stops_a_timer},
        {"Reactor.spent_id_cannot_cancel_successor", test_a_spent_timer_id_cannot_cancel_its_successor},
        {"Reactor.a_timer_bounds_an_unbounded_wait", test_a_timer_bounds_an_unbounded_wait},
        {"Reactor.timer_from_callback_waits", test_a_timer_scheduled_from_a_callback_waits_for_the_next_turn},
        {"Reactor.run_returns_when_nothing_is_left", test_run_returns_when_nothing_is_left},
        {"Reactor.stop_ends_a_run", test_stop_ends_a_run_from_another_thread},
        {"Reactor.a_stray_stop_does_not_end_a_later_run", test_a_stray_stop_does_not_end_a_later_run},
        {"Reactor.misuse_is_refused", test_misuse_is_refused},
        {"Reactor.refuses_re_entry", test_the_loop_refuses_re_entry},
    });
}
