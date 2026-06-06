import stdx;

#ifdef __GNUC__
using namespace stdx::core;
#endif

void test_monotonic() {
    // Never decreases across many back-to-back samples.
    u64 prev = System::nano_time();
    bool monotonic = true;
    for (int i = 0; i < 1'000'000; ++i) {
        u64 cur = System::nano_time();
        if (cur < prev) {
            monotonic = false;
            break;
        }
        prev = cur;
    }
    System::out.println("Monotonic over 1,000,000 samples: {}", monotonic);
}

void test_advances() {
    // Elapsed time over a busy loop is strictly positive.
    u64 start = System::nano_time();
    volatile u64 acc = 0;
    for (u64 i = 0; i < 50'000'000ULL; ++i) {
        acc += i;
    }
    u64 elapsed = System::nano_time() - start;
    System::out.println("Elapsed ns over busy loop is > 0: {} (ns={})", elapsed > 0, elapsed);
}

void test_decoupled_from_epoch() {
    // A steady-clock origin (boot time) yields values vastly smaller than wall-clock
    // nanos since 1970. If nano_time() were still on SystemClock these would be ~equal.
    u64 nano = System::nano_time();
    u64 wall_nanos_since_epoch = System::current_time_millis() * 1'000'000ULL;
    System::out.println("nano_time() = {}", nano);
    System::out.println("wall nanos since 1970 = {}", wall_nanos_since_epoch);
    System::out.println("nano_time() << wall epoch = {}", nano < wall_nanos_since_epoch);
}

int main() {
    test_monotonic();
    test_advances();
    test_decoupled_from_epoch();
}
