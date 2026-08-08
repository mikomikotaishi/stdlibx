import stdx;

using stdx::collections::HashMap;

using namespace stdx::test;

namespace {
    constexpr StringView NAME = "STDLIBX_ENVIRONMENT_TEST_VAR";
    constexpr StringView VALUE = "a-test-value";

    /**
     * @brief Sets NAME to @p value and returns the environment as a map.
     * @param name The variable to set.
     * @param value The value to assign to it.
     * @return A HashMap of the current environment, after the set.
     */
    HashMap<String, String> set_then_read(StringView name, StringView value) {
        require(Environment::set(name, value), "set() succeeds");
        return Environment::variables();
    }
}

void test_variables_contains_a_set_variable() {
    const HashMap<String, String> vars = set_then_read(NAME, VALUE);

    expect(!vars.empty(), "variables() returns a non-empty environment");

    const auto entry = vars.find(String(NAME));
    require(entry != vars.end(), "a variable set just before the call is present");
    expect(entry->second == VALUE, "its value is the one that was set");

    Environment::unset(NAME);
}

void test_variables_agrees_with_get() {
    // variables() walks the environment block; get() calls getenv. They read the
    // same store by two different routes, so they must agree.
    const HashMap<String, String> vars = set_then_read(NAME, VALUE);

    const auto entry = vars.find(String(NAME));
    require(entry != vars.end(), "the variable is in the map");

    const Optional<String> direct = Environment::get(NAME);
    require(direct.has_value(), "get() also sees it");
    expect(entry->second == *direct, "variables() and get() report the same value");

    Environment::unset(NAME);
}

void test_variables_reflects_unset() {
    const HashMap<String, String> before = set_then_read(NAME, VALUE);
    require(before.find(String(NAME)) != before.end(), "present before the unset");

    require(Environment::unset(NAME), "unset() succeeds");

    const HashMap<String, String> after = Environment::variables();
    expect(after.find(String(NAME)) == after.end(), "gone from variables() after unset()");
    expect(!Environment::get(NAME).has_value(), "gone from get() after unset()");
}

void test_variables_splits_on_the_first_equals() {
    // An entry is "name=value" and the value may itself contain '=', so the
    // split has to be on the first one only.
    static constexpr StringView EMBEDDED = "b=c=d";
    const HashMap<String, String> vars = set_then_read(NAME, EMBEDDED);

    const auto entry = vars.find(String(NAME));
    require(entry != vars.end(), "a value containing '=' still parses");
    expect(entry->second == EMBEDDED, "the value keeps every '=' after the first");

    Environment::unset(NAME);
}

void test_variables_is_an_independent_snapshot() {
    // Mutating the environment afterwards cannot change or invalidate what the
    // map holds. On a CRT that frees or reuses the replaced buffer (the Windows
    // UCRT, musl) reading a map of views here would be a use-after-free; the
    // static_asserts above are what enforce it on glibc, which merely leaks.
    const HashMap<String, String> snapshot = set_then_read(NAME, VALUE);

    for (i32 i = 0; i < 300; ++i) {
        Environment::set(Ops::fmt("STDLIBX_ENVIRONMENT_TEST_FILLER_{}", i), "filler");
    }
    require(Environment::set(NAME, "a-much-longer-replacement-value-than-before"), "overwrite succeeds");
    require(Environment::unset(NAME), "unset succeeds");

    const auto entry = snapshot.find(String(NAME));
    require(entry != snapshot.end(), "the snapshot still holds the entry after an unset");
    expect(entry->second == VALUE, "and still holds the original value after an overwrite");

    for (i32 i = 0; i < 300; ++i) {
        Environment::unset(Ops::fmt("STDLIBX_ENVIRONMENT_TEST_FILLER_{}", i));
    }
}

void test_set_respects_no_overwrite() {
    require(Environment::set(NAME, VALUE), "the initial set() succeeds");

    expect(Environment::set(NAME, "replacement", false), "set(overwrite=false) reports success");
    const Optional<String> kept = Environment::get(NAME);
    require(kept.has_value(), "the variable still exists");
    expect(*kept == VALUE, "set(overwrite=false) leaves an existing value alone");

    expect(Environment::set(NAME, "replacement"), "set(overwrite=true) succeeds");
    const Optional<String> replaced = Environment::get(NAME);
    require(replaced.has_value(), "the variable still exists");
    expect(*replaced == "replacement", "set(overwrite=true) does replace it");

    Environment::unset(NAME);
}

void test_get_returns_nullopt_when_unset() {
    Environment::unset(NAME);
    expect(!Environment::get(NAME).has_value(), "get() on an unset variable is nullopt");

    const HashMap<String, String> vars = Environment::variables();
    expect(vars.find(String(NAME)) == vars.end(), "and it is absent from variables()");
}

void test_system_info_reports_identity() {
    const Optional<Environment::SystemInfo> info = Environment::system_info();
    require(info.has_value(), "system_info() succeeds on a supported platform");

    expect(!info->name.empty(), "name is always populated");
    // On Windows this walks win32::ProcessorArchitecture, so an empty machine
    // would mean those constants no longer match wProcessorArchitecture.
    expect(!info->machine.empty(), "machine names the CPU architecture");
}

void test_args_reports_the_command_line() {
    // The test runner is invoked by ctest with no extra arguments, so argv[0] is
    // all that can be relied on - but it is the part that proves the capture ran at
    // all, which on Linux means a .init_array constructor inside the module.
    const Span<const StringView> args = Environment::args();

    require(!args.empty(), "args() is non-empty, so the platform capture worked");
    expect(
        args.front().find("EnvironmentTest") != StringView::npos,
        "argv[0] names this program, matching Rust's args() in including it"
    );
    expect(Environment::program_name() == args.front(), "program_name() is argv[0]");
}

void test_args_is_stable_across_calls() {
    // The table is built once, so two calls must span the same storage rather than
    // one of them handing back views into a temporary.
    const Span<const StringView> first = Environment::args();
    const Span<const StringView> second = Environment::args();

    expect_eq(first.size(), second.size(), "both calls report the same count");
    require(!first.empty(), "there is something to compare");
    expect(first.data() == second.data(), "and span the very same array");
    expect(first.front().data() == second.front().data(), "with the same backing bytes");
}

void test_args_survives_environment_mutation() {
    // argv lives in the process's initial stack region, which no environment call
    // touches. This is the difference that lets args() hand out views where
    // variables() has to copy.
    const Span<const StringView> before = Environment::args();
    require(!before.empty(), "arguments are available");
    const String snapshot(before.front());

    for (i32 i = 0; i < 100; ++i) {
        Environment::set(Ops::fmt("STDLIBX_ARGS_TEST_FILLER_{}", i), "filler");
    }
    for (i32 i = 0; i < 100; ++i) {
        Environment::unset(Ops::fmt("STDLIBX_ARGS_TEST_FILLER_{}", i));
    }

    expect(Environment::args().front() == snapshot, "argv views are unaffected by environment churn");
}

void test_pid_and_operating_system() {
    expect(Environment::pid() != 0, "pid() reports a real process id");
    expect(
        Environment::operating_system() != Environment::OperatingSystem::UNKNOWN,
        "operating_system() identifies a supported platform"
    );
}

// No argc/argv: run() takes the command line from Environment::args(), which is
// also what these tests exercise. Every other test binary still uses the
// argc/argv overload, so both paths stay covered.
int main() {
    return run({
        {"Environment.variables_contains_a_set_variable", test_variables_contains_a_set_variable},
        {"Environment.variables_agrees_with_get", test_variables_agrees_with_get},
        {"Environment.variables_reflects_unset", test_variables_reflects_unset},
        {"Environment.variables_splits_on_the_first_equals", test_variables_splits_on_the_first_equals},
        {"Environment.variables_is_an_independent_snapshot", test_variables_is_an_independent_snapshot},
        {"Environment.set_respects_no_overwrite", test_set_respects_no_overwrite},
        {"Environment.get_returns_nullopt_when_unset", test_get_returns_nullopt_when_unset},
        {"Environment.args_reports_the_command_line", test_args_reports_the_command_line},
        {"Environment.args_is_stable_across_calls", test_args_is_stable_across_calls},
        {"Environment.args_survives_environment_mutation", test_args_survives_environment_mutation},
        {"Environment.system_info_reports_identity", test_system_info_reports_identity},
        {"Environment.pid_and_operating_system", test_pid_and_operating_system},
    });
}
