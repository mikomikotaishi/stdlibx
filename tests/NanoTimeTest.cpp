import stdx;

using namespace stdx::test;

#ifdef __GNUC__
using namespace stdx::core;
#endif

void test_monotonic() {
    // nano_time() never decreases across many back-to-back samples.
    u64 prev = System::nano_time();
    bool monotonic = true;
    for (i32 i = 0; i < 1'000'000; ++i) {
        const u64 cur = System::nano_time();
        if (cur < prev) {
            monotonic = false;
            break;
        }
        prev = cur;
    }
    expect(monotonic, "nano_time() is monotonic over 1,000,000 samples");
}

void test_advances() {
    // Elapsed time over a busy loop is strictly positive.
    const u64 start = System::nano_time();
    volatile u64 acc = 0;
    for (u64 i = 0; i < 50'000'000ull; ++i) {
        acc += i;
    }
    const u64 elapsed = System::nano_time() - start;
    expect(elapsed > 0, "nano_time() advances across a busy loop");
}

void test_decoupled_from_epoch() {
    // A steady-clock origin (boot time) yields values vastly smaller than
    // wall-clock nanos since 1970. If nano_time() were still on SystemClock
    // these would be ~equal.
    const u64 nano = System::nano_time();
    const u64 wall_nanos_since_epoch = System::current_time_millis() * 1'000'000ull;
    expect(nano < wall_nanos_since_epoch, "nano_time() origin is boot, not the 1970 epoch");
}

int main(int argc, char* argv[]) {
    return run(argc, argv, {
        {"NanoTime.monotonic", test_monotonic},
        {"NanoTime.advances", test_advances},
        {"NanoTime.decoupled_from_epoch", test_decoupled_from_epoch},
    });
}
