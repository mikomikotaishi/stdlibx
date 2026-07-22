#include <version>

import stdx;

#ifdef __cpp_impl_reflection
using stdx::collections::Vector;

using namespace stdx::test;

// Every test in this namespace is found by reflection: no list, no registration.
// The reported names are "unit.<function name without its test_ prefix>", except
// where a DisplayName says otherwise.
namespace mmt::unit {
    i32 setups = 0;
    i32 teardowns = 0;
    bool ran_once = false;

    // Hooks are picked up by name, without an annotation.
    void before_all() {
        ran_once = true;
    }

    void before_each() {
        ++setups;
    }

    void after_each() {
        ++teardowns;
    }

    void test_discovered_by_name() {
        expect(true, "a test_* function is registered");
    }

    void test_noexcept_body() noexcept {
        expect_eq(2 + 2, 4, "a noexcept test body is registered");
    }

    [[=TestCase()]]
    void checks_a_differently_named_function() {
        expect(true, "a TestCase-annotated function is registered");
    }

    [[=Tag("slow")]]
    [[=Tag("integration")]]
    void test_carries_its_tags() {
        expect(true, "tags ride along on the discovered test");
    }

    [[=DisplayName("unit.a name of its own")]]
    void test_renamed() {
        expect(true, "DisplayName replaces the derived name");
    }

    [[=Disabled("demonstrates the Disabled annotation")]]
    void test_disabled() {
        expect(false, "a disabled test body never runs");
    }

    void test_hooks_ran() {
        expect(ran_once, "before_all ran once before the first test");
        expect_gt(setups, 0, "before_each ran before this test");
        expect_eq(setups - 1, teardowns, "after_each ran after each prior test");
    }

    // Neither of these is a test: the first is not named test_*, the second takes
    // an argument.
    void helper_is_not_a_test() {}

    void test_with_arguments_is_not_a_test(i32 _) {}
}

namespace mmt::empty {}

// A class works as a discovery target too: its static member functions are scanned
// in an unchecked access context, so private ones are reachable, while non-static
// members are skipped because a Test::Callback has no object to call them on.
class Fixture {
private:
    static void test_private_static() {
        expect(true, "a private static test was reached");
    }
public:
    static void test_public_static() {
        expect(true, "a public static test was reached");
    }

    void test_non_static_member() {
        expect(false, "never discovered, so never run");
    }
};

// What discovery produced is itself checked, from a namespace of its own.
namespace mmt::discovery {
    [[nodiscard]]
    Optional<Test> find(const Vector<Test>& tests, StringView name) {
        for (const Test& test: tests) {
            if (test.name == name) {
                return test;
            }
        }
        return nullopt;
    }

    void test_finds_every_test_and_nothing_else() {
        const Vector<Test> found = discover<^^mmt::unit>();
        expect_eq(found.size(), 7uz, "seven tests, no hooks and no helpers");
        expect(find(found, "unit.discovered_by_name").has_value(), "test_* functions are found");
        expect(find(found, "unit.noexcept_body").has_value(), "noexcept test bodies are found");
        expect(
            find(found, "unit.checks_a_differently_named_function").has_value(),
            "TestCase-annotated functions are found"
        );
        expect(!find(found, "unit.helper_is_not_a_test").has_value(), "plain helpers are skipped");
        expect(!find(found, "unit.before_each").has_value(), "hooks are not registered as tests");
    }

    void test_carries_annotation_metadata() {
        const Vector<Test> found = discover<^^mmt::unit>();
        const Optional<Test> tagged = find(found, "unit.carries_its_tags");
        require(tagged.has_value(), "the tagged test was discovered");
        require_eq(tagged->tags.size(), 2uz, "both tags were carried over");
        expect_eq(tagged->tags[0], "slow", "tags keep their declaration order");
        expect_eq(tagged->tags[1], "integration");

        expect(find(found, "unit.a name of its own").has_value(), "DisplayName is honoured");
        expect(!find(found, "unit.renamed").has_value(), "the derived name is replaced, not added");

        const Optional<Test> disabled = find(found, "unit.disabled");
        require(disabled.has_value(), "a disabled test stays registered");
        require(disabled->disabled.has_value(), "the disabled reason is carried over");
        expect_eq(*disabled->disabled, "demonstrates the Disabled annotation");
    }

    void test_finds_the_suite_hooks() {
        const Suite found = Suite::of<^^mmt::unit>();
        expect_eq(found.name, "unit", "the suite is named after the namespace");
        expect(found.before_all != nullptr, "before_all was wired up by name");
        expect(found.before_each != nullptr, "before_each was wired up by name");
        expect(found.after_each != nullptr, "after_each was wired up by name");
        expect(found.after_all == nullptr, "a hook the namespace omits stays null");
    }

    void test_discovered_bodies_are_callable() {
        const Vector<Test> found = discover<^^mmt::unit>();
        const Optional<Test> test = find(found, "unit.discovered_by_name");
        require(test.has_value(), "the test was discovered");
        expect_no_throw([&] -> void { test->fn(); }, "the discovered pointer calls the real body");
    }

    void test_finds_the_static_members_of_a_class() {
        const Vector<Test> found = discover<^^Fixture>();
        expect_eq(found.size(), 2uz, "both static tests, and nothing else");
        expect(find(found, "Fixture.public_static").has_value(), "named after the class");
        expect(
            find(found, "Fixture.private_static").has_value(),
            "private statics are reachable, the access context is unchecked"
        );
        expect(
            !find(found, "Fixture.non_static_member").has_value(),
            "a non-static member is skipped rather than breaking the build"
        );
    }

    void test_run_over_a_namespace_is_instantiable() {
        // Taken, not called: running a nested suite here would print a second
        // summary into the middle of this one. The address is enough to have the
        // entry point compiled.
        constexpr int (*runner)(int, char*[]) = &run<^^mmt::unit>;
        expect(runner != nullptr, "run<Nsp> resolves to the discovery entry point");
    }

    void test_an_empty_namespace_yields_nothing() {
        expect(discover<^^mmt::empty>().empty(), "a namespace without tests yields none");
        expect(Suite::of<^^mmt::empty>().tests.empty(), "and so does its suite");
    }
}
#endif

int main(int argc, char* argv[]) {
    #ifdef __cpp_impl_reflection
    // Try `--verbose` to see the reported names, or `--tag slow` to run only the
    // tests tagged by annotation.
    return run_suites(argc, argv, {Suite::of<^^mmt::discovery>(), Suite::of<^^mmt::unit>()});
    #else
    System::out.println("[test] Test disabled (compiler does not support reflection).");
    return System::EXIT_SUCCESS;
    #endif
}
