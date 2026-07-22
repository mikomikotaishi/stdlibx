#pragma once

/**
 * @namespace stdx::test
 * @brief Minimal assertion-based unit-testing framework.
 */
export namespace stdx::test {
    #ifdef __cpp_impl_reflection
    /**
     * @struct TestCase
     * @brief An annotation marking a function as a test whatever its name.
     *
     * Discovery already picks up every nullary void function whose name starts
     * with {@code test_}; apply {@code [[=TestCase()]]} to register one that is
     * named differently:
     * @code
     * [[=TestCase()]]
     * void a_socket_round_trip() { ... }
     * @endcode
     */
    struct TestCase {};

    /**
     * @struct Tag
     * @brief An annotation adding a tag to a discovered test, for --tag filtering.
     *
     * Repeatable; every Tag on the function ends up in Test::tags.
     * @code
     * [[=Tag("slow")]]
     * [[=Tag("net")]]
     * void test_resolves_every_record() { ... }
     * @endcode
     */
    struct Tag {
        const char* value; ///< The tag text, in static storage.

        consteval Tag(StringView tag) noexcept:
            value{Ops::define_static_string(tag)} {}
    };

    /**
     * @struct DisplayName
     * @brief An annotation replacing a discovered test's reported name.
     *
     * Without it the reported name is the enclosing namespace, a dot, and the
     * function name with its {@code test_} prefix removed.
     * @code
     * [[=DisplayName("Uri.parses an authority with a port")]]
     * void test_authority_port() { ... }
     * @endcode
     */
    struct DisplayName {
        const char* value; ///< The display name, in static storage.

        consteval DisplayName(StringView name) noexcept:
            value{Ops::define_static_string(name)} {}
    };

    /**
     * @struct Disabled
     * @brief An annotation marking a discovered test as skipped without running it.
     *
     * The test is still registered, listed by --list and reported as SKIP, so a
     * disabled test stays visible instead of quietly disappearing.
     * @code
     * [[=Disabled("flaky until the resolver timeout is configurable")]]
     * void test_resolver_timeout() { ... }
     * @endcode
     */
    struct Disabled {
        const char* reason; ///< The reason shown next to SKIP, in static storage.

        consteval Disabled(StringView reason = "") noexcept:
            reason{Ops::define_static_string(reason)} {}
    };

    /**
     * @struct BeforeEach
     * @brief An annotation naming the discovered suite's per-test setup hook.
     *
     * A function named {@code before_each} is picked up without the annotation.
     */
    struct BeforeEach {};

    /**
     * @struct AfterEach
     * @brief An annotation naming the discovered suite's per-test teardown hook.
     *
     * A function named {@code after_each} is picked up without the annotation.
     */
    struct AfterEach {};

    /**
     * @struct BeforeAll
     * @brief An annotation naming the discovered suite's one-time setup hook.
     *
     * A function named {@code before_all} is picked up without the annotation.
     */
    struct BeforeAll {};

    /**
     * @struct AfterAll
     * @brief An annotation naming the discovered suite's one-time teardown hook.
     *
     * A function named {@code after_all} is picked up without the annotation.
     */
    struct AfterAll {};
    #endif
}
