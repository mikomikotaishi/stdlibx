#include <version>

#include "Macros.hpp"

import stdx;

#ifdef STDLIBX_EXECUTION_AVAILABLE
using stdx::exec::Flow;
using stdx::exec::Just;
using stdx::exec::SyncWait;
using stdx::exec::Task;

using namespace stdx::test;
#endif

#ifdef __GNUC__
using namespace stdx::core;
#endif

#ifdef STDLIBX_EXECUTION_AVAILABLE
// A coroutine task that awaits a Flow's raw sender via unwrap() - the
// co_await-in-Task-safe endpoint.
Task<i32> via_unwrap() {
    i32 v = co_await Flow(Just(10))
        .then([](i32 x) -> i32 { return x * 2; })
        .unwrap();
    co_return v + 1;
}

void test_then_value() {
    Optional<i32> result = Flow(Just(6))
        .then([](i32 x) -> i32 { return x * 7; })
        .value();
    require(result.has_value(), "value() over a completed pipeline yields a value");
    expect_eq(*result, 42, "then() maps the value channel");
}

void test_chained_then() {
    Optional<i32> result = Flow(Just(1))
        .then([](i32 x) -> i32 { return x + 10; })
        .then([](i32 x) -> i32 { return x * 2; })
        .value();
    require(result.has_value(), "chained pipeline completes");
    expect_eq(*result, 22, "(1 + 10) * 2 == 22");
}

void test_on_schedule() {
    Optional<i32> result = Flow<>::on_schedule()
        .then([]() -> i32 { return 40; })
        .then([](i32 x) -> i32 { return x + 2; })
        .value();
    require(result.has_value(), "offloaded pipeline completes");
    expect_eq(*result, 42, "work ran on the parallel scheduler and returned 42");
}

void test_let() {
    Optional<i32> result = Flow(Just(41))
        .let([](i32 x) { return Just(x + 1); })
        .value();
    require(result.has_value(), "let() pipeline completes");
    expect_eq(*result, 42, "let_value continued into a child sender");
}

void test_when_all() {
    Optional<Tuple<i32, i32>> result = Flow(Just(3))
        .when_all(Just(4))
        .wait();
    require(result.has_value(), "when_all pipeline completes");
    auto [a, b] = *result;
    expect_eq(a + b, 7, "when_all merged both pipeline values");
}

void test_catch_error() {
    Optional<i32> result = Flow(Just(10))
        .then([](i32 _) -> i32 { throw RuntimeException("boom"); })
        .catch_error([](ExceptionPointer _) -> i32 { return -1; })
        .value();
    require(result.has_value(), "catch_error recovers into a value");
    expect_eq(*result, -1, "the error channel was mapped back to a value");
}

void test_as_task() {
    // Regression guard: as_task() returns a lazy Task that outlives the temporary
    // Flow. It must own its sender (by-value coroutine parameter), or resuming it
    // here reads freed stack - caught by AddressSanitizer as stack-use-after-scope.
    Task<i32> task = Flow(Just(20))
        .then([](i32 x) -> i32 { return x + 1; })
        .as_task();
    Optional<Tuple<i32>> result = SyncWait(Ops::move(task));
    require(result.has_value(), "as_task materializes a runnable Task");
    auto [v] = *result;
    expect_eq(v, 21, "the Task completed with the pipeline's value");
}

void test_unwrap_in_task() {
    Optional<Tuple<i32>> result = SyncWait(via_unwrap());
    require(result.has_value(), "a Task awaiting an unwrapped Flow completes");
    auto [v] = *result;
    expect_eq(v, 21, "co_await flow.unwrap() then + 1 == 21");
}
#endif

int main(int argc, char* argv[]) {
    #ifdef STDLIBX_EXECUTION_AVAILABLE
    return run(argc, argv, {
        {"Flow.then_value", test_then_value},
        {"Flow.chained_then", test_chained_then},
        {"Flow.on_schedule", test_on_schedule},
        {"Flow.let", test_let},
        {"Flow.when_all", test_when_all},
        {"Flow.catch_error", test_catch_error},
        {"Flow.as_task", test_as_task},
        {"Flow.unwrap_in_task", test_unwrap_in_task},
    });
    #else
    System::out.println("[test] Test disabled (enable with STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY).");
    return 0;
    #endif
}
