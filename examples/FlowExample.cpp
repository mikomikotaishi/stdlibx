#include <version>

#include "Macros.hpp"

import stdx;

#ifdef STDLIBX_EXECUTION_AVAILABLE
using stdx::exec::Flow;
using stdx::exec::Just;
using stdx::exec::Task;
using stdx::thread::Thread;

// A Task<void> has no value channel: it co_returns nothing and is co_awaited
// purely for its side effects.
Task<void> announce(StringView label) {
    System::out.println("[flow]   void step: {}", label);
    co_return;
}

// A driver task that just co_awaits two void tasks in sequence. Awaiting a
// Task<void> yields nothing - there is no value to bind.
Task<void> run_steps() {
    co_await announce("first");
    co_await announce("second");
}
#endif

int main(int argc, char* argv[]) {
    #ifdef STDLIBX_EXECUTION_AVAILABLE
    // A simple value pipeline: wrap a value sender, map it, unwrap the result.
    // value() yields the lone value directly instead of a Tuple.
    Optional<i32> answer = Flow(Just(6))
        .then([](i32 x) -> i32 { return x * 7; })
        .value();
    System::out.println("[flow] 6 * 7 = {}", *answer);

    // Offload work onto the system parallel scheduler, then continue on it.
    // on_schedule() starts the pipeline on a worker thread.
    Optional<i32> offloaded = Flow<>::on_schedule()
        .then([] -> i32 { return 40; })
        .then([](i32 x) -> i32 { return x + 2; })
        .value();
    System::out.println("[flow] offloaded 40 + 2 = {}", *offloaded);

    // Run two pipelines concurrently and collect both results.
    // Multi-value pipelines use wait() + destructuring rather than value().
    Optional<Tuple<i32, i32>> both = Flow(Just(3))
        .when_all(Just(4))
        .wait();
    auto [a, b] = *both;
    System::out.println("[flow] when_all -> ({}, {})", a, b);

    // Materialize a pipeline as a coroutine Task, then run it. as_task() is the
    // hand-off point: the Task can be co_awaited or composed under when_all.
    Task<i32> task = Flow(Just(20))
        .then([](i32 x) -> i32 { return x + 1; })
        .as_task();
    auto [t] = *Thread::sync_wait(Ops::move(task));
    System::out.println("[flow] as_task -> {}", t);

    // Recover from an error raised on the value channel.
    Optional<i32> recovered = Flow(Just(10))
        .then([](i32 _) -> i32 { throw RuntimeException("boom"); })
        .catch_error([](ExceptionPointer _) -> i32 { return -1; })
        .value();
    System::out.println("[flow] recovered from error -> {}", *recovered);

    // co_await is legal only inside a coroutine, which main() cannot be
    Optional<Tuple<>> _ = Thread::sync_wait(run_steps());
    System::out.println("[flow] void task sequence complete");
    #else
    System::out.println(
        "[flow] execution backend not available "
        "(enable STDLIBX_EXTENSIONS_COMPILE_NVIDIA_STDEXEC_LIBRARY)."
    );
    #endif
    return 0;
}
