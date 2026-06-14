import stdx;

using namespace stdx::test;

#ifdef __GNUC__
using namespace stdx::core;
#endif

void test_basic_assertions() {
    expect(1 + 1 == 2, "arithmetic holds");
    expect_eq(2 + 2, 4);
    expect_ne(1, 2);
    expect_lt(1, 2);
    expect_le(2, 2);
    expect_gt(3, 2);
    expect_ge(3, 3);
}

void test_floating_point() {
    expect_near(0.1 + 0.2, 0.3, 1e-9, "0.1 + 0.2 approximates 0.3");
}

void test_exceptions() {
    expect_throws<ArithmeticException>(
        [] [[noreturn]] -> void { throw ArithmeticException("boom"); }
    );
    expect_no_throw(
        [] -> void {
            volatile i32 x = 1 + 1;
            static_cast<void>(x);
        }
    );
}

void test_fatal_assertions() {
    require(true, "precondition holds");
    require_eq(42, 42);
    expect(true);
}

void test_skipped() {
    skip("demonstrates the SKIP path");
}

// Shared fixture state, driven by the suite's before_each/after_each hooks.
i32 setups = 0;
i32 teardowns = 0;

void fixture_setup() {
    ++setups;
}

void fixture_teardown() {
    ++teardowns;
}

void test_uses_fixture() {
    expect_gt(setups, 0, "before_each ran before this test");
    expect_eq(setups - 1, teardowns, "after_each ran after each prior test");
}

int main(int argc, char* argv[]) {
    // Try `--verbose` to see passing tests, `--filter 'Framework.f*'` to select
    // a subset, `--tag fixture` to run tagged tests, or `--list` to list names.
    return run(argc, argv, Suite {
        .name = "Framework",
        .before_each = fixture_setup,
        .after_each = fixture_teardown,
        .tests = {
            {"Framework.basic_assertions", test_basic_assertions},
            {"Framework.floating_point", test_floating_point},
            {"Framework.exceptions", test_exceptions},
            {"Framework.fatal_assertions", test_fatal_assertions},
            {"Framework.skipped", test_skipped},
            {"Framework.uses_fixture", test_uses_fixture, {"fixture"}},
        }
    });
}
