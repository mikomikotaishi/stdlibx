#include <version>

#include "Macros.hpp"

import stdx;

#if defined(STDLIBX_EXECUTION_AVAILABLE) && defined(STDLIBX_NET_ASYNC_TASKS)
using stdx::collections::Vector;
using stdx::exec::Scheduler;
using stdx::exec::Sender;
using stdx::exec::Task;
using stdx::net::Endpoint;
using stdx::net::IPAddress;
using stdx::net::Interest;
using stdx::net::Reactor;
using stdx::net::ReactorScheduler;
using stdx::net::Readiness;
using stdx::net::TcpListener;
using stdx::net::TcpStream;
using stdx::thread::Thread;

using namespace stdx::test;

namespace {
    [[nodiscard]]
    Endpoint loopback_any_port() {
        return Endpoint(IPAddress::loopback(IPAddress::Family::IPV4), 0);
    }

    /**
     * @brief A listener bound to an ephemeral loopback port, ready for the reactor.
     *
     * Non-blocking is not a detail here: every async operation is built on
     * try_*() answering "not yet" with an empty Optional, which a blocking
     * descriptor never does.
     */
    [[nodiscard]]
    TcpListener async_listener() {
        TcpListener listener = TcpListener::bind(loopback_any_port());
        listener.socket().set_blocking(false);
        return listener;
    }

    /**
     * @brief Accepts one connection, echoes the first message back, and reports its size.
     */
    Task<usize> echo_once(Reactor& reactor, TcpListener& listener) {
        TcpStream peer = co_await reactor.accept(listener);
        peer.socket().set_blocking(false);

        Array<byte, 128> buffer = {};
        const usize received = co_await reactor.receive(peer, buffer);
        const usize sent = co_await reactor.send_all(
            peer, Span<const byte>(buffer.data(), received)
        );
        co_return sent;
    }

    /**
     * @brief A blocking client on another thread: connects, sends, reads the echo.
     *
     * The reactor is single-threaded by design, so the peer cannot share it.
     * A plain blocking socket is the honest counterpart, and it also proves the
     * async side is talking to an ordinary TCP peer rather than to itself.
     */
    [[nodiscard]]
    Thread spawn_client(Endpoint server, StringView message, String& echoed) {
        return Thread([server, message, &echoed] -> void {
            TcpStream client = TcpStream::connect(server);
            static_cast<void>(client.send(Span<const byte>(
                reinterpret_cast<const byte*>(message.data()), message.size()
            )));

            Array<byte, 128> buffer = {};
            const usize read = client.receive(buffer);
            echoed.assign(reinterpret_cast<const char*>(buffer.data()), read);
        });
    }
}

void test_readiness_is_a_sender() {
    // The Task promise only transforms senders, so an awaitable that is not one
    // cannot be co_awaited inside a Task at all - this is what makes the whole
    // layer composable with WhenAll and Then rather than just with co_await.
    static_assert(
        Sender<Readiness>,
        "Readiness models the sender concept"
    );
    static_assert(
        Scheduler<ReactorScheduler>,
        "ReactorScheduler models the scheduler concept"
    );
    expect(true, "the concepts above are the assertion");
}

void test_async_accept_receive_and_send() {
    Reactor reactor;
    TcpListener listener = async_listener();
    const Endpoint bound = listener.local_endpoint();

    String echoed;
    Thread client = spawn_client(bound, "hello async", echoed);

    const usize sent = reactor.run_until(echo_once(reactor, listener));
    client.join();

    expect_eq(sent, 11uz, "the echo wrote back everything it read");
    expect_eq(echoed, "hello async", "and the client got its own message back");
}

void test_the_reactor_is_empty_again_afterwards() {
    // Every await registers and then deregisters. If it did not, a second
    // operation on the same descriptor would be refused by Reactor::add.
    Reactor reactor;
    TcpListener listener = async_listener();

    String echoed;
    Thread client = spawn_client(listener.local_endpoint(), "twice", echoed);
    static_cast<void>(reactor.run_until(echo_once(reactor, listener)));
    client.join();

    expect_eq(reactor.registered(), 0uz, "no descriptor is left registered");
    expect_eq(reactor.pending_timers(), 0uz, "and no scheduler post is left pending");
}

void test_a_task_resumes_on_the_reactor_thread() {
    // The scheduler exists so continuations land on the thread driving the
    // reactor; a Reactor touched from anywhere else would be a data race.
    Reactor reactor;
    TcpListener listener = async_listener();

    const Thread::Id driver = Thread::current_id();
    Thread::Id resumed{};

    String echoed;
    Thread client = spawn_client(listener.local_endpoint(), "thread", echoed);

    const auto observe = [&](Reactor& loop, TcpListener& source) -> Task<usize> {
        TcpStream peer = co_await loop.accept(source);
        peer.socket().set_blocking(false);
        resumed = Thread::current_id();
        Array<byte, 32> buffer = {};
        co_return co_await loop.receive(peer, buffer);
    };

    const usize read = reactor.run_until(observe(reactor, listener));
    client.join();

    expect_eq(read, 6uz, "the message arrived");
    expect(resumed == driver, "the coroutine resumed on the thread driving the reactor");
}
#endif

int main(int argc, char* argv[]) {
    #if defined(STDLIBX_EXECUTION_AVAILABLE) && defined(STDLIBX_NET_ASYNC_TASKS)
    return run(argc, argv, {
        {"NetAsync.readiness_is_a_sender", test_readiness_is_a_sender},
        {"NetAsync.accept_receive_and_send", test_async_accept_receive_and_send},
        {"NetAsync.reactor_is_empty_again_afterwards", test_the_reactor_is_empty_again_afterwards},
        {"NetAsync.a_task_resumes_on_the_reactor_thread", test_a_task_resumes_on_the_reactor_thread},
    });
    #else
    System::out.println("[test] Test disabled (needs STDLIBX_NET_ASYNC_TASKS; see net/async.inl).");
    return System::EXIT_SUCCESS;
    #endif
}
