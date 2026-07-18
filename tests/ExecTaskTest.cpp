#include <version>

#include "Macros.hpp"

import stdx;

#ifdef STDLIBX_EXECUTION_AVAILABLE
using stdx::exec::SyncWait;
using stdx::exec::Task;
using stdx::exec::Just;
using stdx::exec::WhenAll;

using namespace stdx::test;
#endif

#ifdef __GNUC__
using namespace stdx::core;
#endif

#ifdef STDLIBX_EXECUTION_AVAILABLE
// A leaf coroutine task: co_return a value.
Task<i32> answer() {
    co_return 42;
}

Task<i32> increment(i32 x) {
    co_return x + 1;
}

// Composition: a task that co_awaits other tasks.
Task<i32> composed() {
    i32 a = co_await answer();
    i32 b = co_await increment(a);
    co_return a + b;
}

// Sender interop: co_await a plain sender from inside a coroutine task
// (via the task promise's with_awaitable_senders support).
Task<i32> awaits_sender() {
    i32 v = co_await Just(20);
    co_return v + 1;
}

void test_task_basic() {
    Optional<Tuple<i32>> result = SyncWait(answer());
    require(result.has_value(), "sync_wait over a task yields a value");
    auto [v] = *result;
    expect_eq(v, 42, "co_return round-trips through sync_wait");
}

void test_task_composition() {
    Optional<Tuple<i32>> result = SyncWait(composed());
    require(result.has_value(), "composed task completes");
    auto [v] = *result;
    expect_eq(v, 85, "42 + (42 + 1) == 85 via a co_await chain");
}

void test_task_awaits_sender() {
    Optional<Tuple<i32>> result = SyncWait(awaits_sender());
    require(result.has_value(), "a task awaiting a sender completes");
    auto [v] = *result;
    expect_eq(v, 21, "co_await Just(20) then + 1 == 21");
}

void test_when_all_senders() {
    Optional<Tuple<i32, i32>> result = SyncWait(WhenAll(Just(3), Just(4)));
    require(result.has_value(), "when_all completes");
    auto [a, b] = *result;
    expect_eq(a + b, 7, "when_all merged both sender values");
}
#endif

int main(int argc, char* argv[]) {
    #ifdef STDLIBX_EXECUTION_AVAILABLE
    return run(argc, argv, {
        {"ExecTask.basic", test_task_basic},
        {"ExecTask.composition", test_task_composition},
        {"ExecTask.awaits_sender", test_task_awaits_sender},
        {"ExecTask.when_all_senders", test_when_all_senders},
    });
    #else
    System::out.println("[test] Test disabled (enable with STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY).");
    return 0;
    #endif
}
