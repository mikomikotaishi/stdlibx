#include <version>

#include "Macros.hpp"

import stdx;

#if defined(__linux__) && defined(STDLIBX_EXECUTION_AVAILABLE) && defined(STDLIBX_NET_ASYNC_TASKS)
using stdx::net::Reactor;
using stdx::sys::ExitStatus;
using stdx::sys::Process;
using stdx::sys::Stdio;

using namespace stdx::test;

namespace {
    /**
     * @brief Spawn a child on the reactor and await its exit.
     */
    Reactor::Task<ExitStatus> await_exit(Reactor& reactor, Process::Builder builder) {
        Expected<Process, ErrorCode> child = reactor.spawn(Ops::move(builder));
        if (!child.has_value()) {
            throw RuntimeException("reactor.spawn failed");
        }
        // The child owns a pidfd now; prove it.
        require(child->exit_pidfd() != -1, "reactor-spawned child carries a pidfd");
        co_return co_await child->wait_async(reactor);
    }
}

void test_await_true_succeeds() {
    Reactor reactor;
    ExitStatus status = reactor.run_until(
        await_exit(reactor, Process::Builder("true").stdout(Stdio::NULL_DEV))
    );
    expect(status.success(), "awaited `true` exits successfully");
}

void test_await_false_fails() {
    Reactor reactor;
    ExitStatus status = reactor.run_until(
        await_exit(reactor, Process::Builder("false").stdout(Stdio::NULL_DEV))
    );
    expect(!status.success(), "awaited `false` exits non-zero");
}

void test_await_sleep_actually_suspends() {
    // sleep is still running when wait_async first polls the pidfd, so this
    // exercises the suspend-and-resume path rather than an already-exited child.
    Reactor reactor;
    ExitStatus status = reactor.run_until(
        await_exit(reactor, Process::Builder("sleep").arg("0.15"))
    );
    expect(status.success(), "awaited `sleep 0.15` completes successfully");
}

void test_reactor_is_empty_after_await() {
    Reactor reactor;
    static_cast<void>(reactor.run_until(
        await_exit(reactor, Process::Builder("true").stdout(Stdio::NULL_DEV))
    ));
    expect_eq(reactor.registered(), 0uz, "the pidfd is deregistered after exit");
    expect_eq(reactor.pending_timers(), 0uz, "no timer is left pending");
    expect_eq(reactor.pending_ready(), 0uz, "no continuation is left queued");
}
#endif

int main(int argc, char* argv[]) {
    #if defined(__linux__) && defined(STDLIBX_EXECUTION_AVAILABLE) && defined(STDLIBX_NET_ASYNC_TASKS)
    return run(argc, argv, {
        {"ProcessAsync.await_true_succeeds", test_await_true_succeeds},
        {"ProcessAsync.await_false_fails", test_await_false_fails},
        {"ProcessAsync.await_sleep_actually_suspends", test_await_sleep_actually_suspends},
        {"ProcessAsync.reactor_is_empty_after_await", test_reactor_is_empty_after_await},
    });
    #else
    System::out.println("[test] Test disabled (needs Linux + STDLIBX_EXECUTION_AVAILABLE + STDLIBX_NET_ASYNC_TASKS).");
    return System::EXIT_SUCCESS;
    #endif
}
