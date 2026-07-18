#include <version>

#include "Macros.hpp"

import stdx;

#ifdef STDLIBX_EXECUTION_AVAILABLE
using stdx::exec::SyncWait;
using stdx::exec::Task;
using stdx::exec::WhenAll;
using stdx::thread::Thread;

using namespace stdx::test;
#endif

#ifdef __GNUC__
using namespace stdx::core;
#endif

#ifdef STDLIBX_EXECUTION_AVAILABLE
void test_async_returns_value() {
    Optional<Tuple<i32>> result = SyncWait(Async::offload_sender([] -> i32 { return 6 * 7; }));
    require(result.has_value(), "async completed");
    auto [v] = *result;
    expect_eq(v, 42, "async yields the callable's result");
}

void test_async_offloads_to_another_thread() {
    Thread::Id caller = Thread::current_id();
    Optional<Tuple<Thread::Id>> result = SyncWait(Async::offload_sender([] -> Thread::Id { return Thread::current_id(); }));
    require(result.has_value(), "async completed");
    auto [ran_on] = *result;
    expect(ran_on != caller, "the callable ran off the calling thread");
}

void test_async_overlaps_under_when_all() {
    Optional<Tuple<i32, i32>> result = SyncWait(WhenAll(
        Async::offload([] -> i32 { return 10; }),
        Async::offload([] -> i32 { return 20; })
    ));
    require(result.has_value(), "when_all of two offloads completed");
    auto [a, b] = *result;
    expect_eq(a + b, 30, "both offloaded results merged");
}

Task<i32> use_async_in_coroutine() {
    i32 a = co_await Async::offload_sender([] -> i32 { return 3; });
    i32 b = co_await Async::offload_sender([] -> i32 { return 4; });
    co_return a * b;
}

void test_async_awaited_in_task() {
    Optional<Tuple<i32>> result = SyncWait(use_async_in_coroutine());
    require(result.has_value(), "task awaiting async offloads completed");
    auto [v] = *result;
    expect_eq(v, 12, "co_await async(...) chains inside a Task");
}

void test_async_propagates_exceptions() {
    bool threw = false;
    try {
        SyncWait(Async::offload([] -> void { throw RuntimeException("boom"); }));
    } catch (const RuntimeException& _) {
        threw = true;
    }
    expect(threw, "an exception in the offloaded callable surfaces at sync_wait");
}
#endif

int main(int argc, char* argv[]) {
    #ifdef STDLIBX_EXECUTION_AVAILABLE
    return run(argc, argv, {
        {"ExecAsync.returns_value", test_async_returns_value},
        {"ExecAsync.offloads_to_another_thread", test_async_offloads_to_another_thread},
        {"ExecAsync.overlaps_under_when_all", test_async_overlaps_under_when_all},
        {"ExecAsync.awaited_in_task", test_async_awaited_in_task},
        {"ExecAsync.propagates_exceptions", test_async_propagates_exceptions},
    });
    #else
    System::out.println("[test] Test disabled (enable with STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY).");
    return 0;
    #endif
}
