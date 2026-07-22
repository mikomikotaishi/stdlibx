#pragma once

using stdx::debug::SourceLocation;
using stdx::sync::Atomic;
using stdx::sync::MemoryOrder;

namespace stdx::test {
    /**
     * @internal
     * @class Context
     * @brief Per-run state shared by the assertions and the runner.
     */
    class [[nodiscard]] Context {
    private:
        Atomic<usize> total_assertions_count{0}; ///< Assertions evaluated across the whole run.
        Atomic<usize> test_assertions_count{0}; ///< Assertions evaluated in the current test.
        Atomic<usize> test_failures_count{0}; ///< Failed assertions in the current test.
        bool use_color = true; ///< Set once before the run, read-only while it runs.
    
        Context() = default;
    public:
        /**
         * @brief Returns the single shared run context.
         * @return A reference to the process-wide test context.
         */
        [[nodiscard]]
        static Context& context() noexcept {
            static Context instance;
            return instance;
        }

        /**
         * @brief Resets the context to its initial state.
         *
         * This is not thread-safe and should only be used when no tests are running.
         */
        void reset() noexcept {
            total_assertions_count.store(0, MemoryOrder::RELAXED);
            test_assertions_count.store(0, MemoryOrder::RELAXED);
            test_failures_count.store(0, MemoryOrder::RELAXED);
            use_color = true;
        }

        /**
         * @brief Resets the per-test counters before a test runs.
         */
        void begin_test() noexcept {
            test_assertions_count.store(0, MemoryOrder::RELAXED);
            test_failures_count.store(0, MemoryOrder::RELAXED);
        }

        /**
         * @brief Atomically records a passing assertion.
         */
        void record_pass() noexcept {
            total_assertions_count.fetch_add(1, MemoryOrder::RELAXED);
            test_assertions_count.fetch_add(1, MemoryOrder::RELAXED);
        }

        /**
         * @brief Atomically records a failing assertion.
         */
        void record_failure() noexcept {
            total_assertions_count.fetch_add(1, MemoryOrder::RELAXED);
            test_assertions_count.fetch_add(1, MemoryOrder::RELAXED);
            test_failures_count.fetch_add(1, MemoryOrder::RELAXED);
        }

        /**
         * @brief Atomically records a non-assertion test failure (e.g. an uncaught exception).
         */
        void record_error() noexcept {
            test_failures_count.fetch_add(1, MemoryOrder::RELAXED);
        }

        /**
         * @brief Returns the total number of assertions evaluated across the whole run.
         * @return The total assertion count.
         */
        [[nodiscard]]
        usize total_assertions() const noexcept {
            return total_assertions_count.load(MemoryOrder::RELAXED);
        }

        /**
         * @brief Returns the number of assertions evaluated in the current test.
         * @return The current test's assertion count.
         */         
        [[nodiscard]]
        usize test_assertions() const noexcept {
            return test_assertions_count.load(MemoryOrder::RELAXED);
        }

        /**
         * @brief Returns the number of failed assertions in the current test.
         * @return The current test's failure count.
         */
        [[nodiscard]]
        usize test_failures() const noexcept {
            return test_failures_count.load(MemoryOrder::RELAXED);
        }

        /**
         * @brief Returns whether the test run should colorise its output.
         * @return True if color is enabled, false otherwise.
         */
        [[nodiscard]]
        bool color_enabled() const noexcept {
            return use_color;
        }

        /**
         * @brief Enables or disables colorised output for the test run.
         * @param enable True to enable color, false to disable.
         */
        void color(bool enable) noexcept {
            use_color = enable;
        }
    };

    /**
     * @internal
     * @brief Records a passing assertion.
     */
    inline void pass() noexcept {
        Context::context().record_pass();
    }

    /**
     * @internal
     * @brief Records a failing assertion and prints its source location.
     * @param loc The source location of the failed assertion.
     * @param detail The already-formatted failure description.
     */
    inline void fail(const SourceLocation& loc, StringView detail) {
        Context::context().record_failure();
        System::err.println("    {}:{}: {}", loc.file_name(), loc.line(), detail);
    }

    /**
     * @internal
     * @brief Appends an optional user message to a failure description.
     * @param detail The base failure description.
     * @param message The optional user message (ignored when empty).
     * @return The description with the message appended in parentheses.
     */
    [[nodiscard]]
    inline String annotate(String detail, StringView message) {
        if (!message.empty()) {
            detail += Ops::format(" ({})", message);
        }
        return detail;
    }

    /**
     * @internal
     * @brief Shared implementation of the binary comparison assertions.
     * @tparam A The left-hand operand type.
     * @tparam B The right-hand operand type.
     * @param passed Whether the comparison held.
     * @param a The left-hand operand.
     * @param op The comparison operator, for the failure message.
     * @param b The right-hand operand.
     * @param message The optional user message.
     * @param loc The source location of the call.
     */
    template <typename A, typename B>
    inline void compare(
        bool passed,
        const A& a,
        StringView op,
        const B& b,
        StringView message,
        const SourceLocation& loc
    ) {
        if (passed) {
            pass();
            return;
        }
        fail(loc, annotate(Ops::format("expected {} {} {}", a, op, b), message));
    }
}

/**
 * @namespace stdx::test
 * @brief Minimal assertion-based unit-testing framework.
 */
export namespace stdx::test {
    /**
     * @class TestAbort
     * @brief Thrown by a failed require_* assertion to abort the current test.
     * @extends Exception
     */
    class TestAbort: public Exception {
    public:
        using Exception::Exception;
    };

    /**
     * @class TestSkipped
     * @brief Thrown by skip() to mark the current test as skipped.
     * @extends Exception
     */
    class TestSkipped: public Exception {
    private:
        String why; ///< The human-readable reason for skipping.
    public:
        using Exception::Exception;

        TestSkipped(StringView reason):
            why{String(reason)} {}
        
        [[nodiscard]]
        const char* what() const noexcept override {
            return why.c_str();
        }

        [[nodiscard]]
        StringView reason() const noexcept {
            return why;
        }
    };

    /**
     * @brief Throws the signal that unwinds a failed require_* assertion.
     *
     * Defined out-of-line and exported so that consumer-side require_* calls link
     * against it rather than emitting the module-internal signal type themselves;
     * this keeps TestAbort's RTTI inside the module.
     */
    [[noreturn]]
    void abort_test() {
        throw TestAbort();
    }

    /**
     * @brief Throws the signal that marks the current test skipped.
     * @param reason The human-readable reason for skipping.
     */
    [[noreturn]]
    void skip_test(StringView reason) {
        throw TestSkipped(reason);
    }

    /**
     * @brief Records a non-fatal condition; on failure the test continues.
     * @param condition The condition expected to hold.
     * @param message Optional context shown on failure.
     * @param loc The source location of the call (defaulted to the call site).
     */
    inline void expect(
        bool condition,
        StringView message = "",
        SourceLocation loc = SourceLocation::current()
    ) {
        if (condition) {
            pass();
            return;
        }
        fail(loc, annotate("expectation failed", message));
    }

    /**
     * @brief Expects two values to compare equal.
     * @tparam A The actual value type.
     * @tparam B The expected value type.
     * @param actual The value produced by the code under test.
     * @param expected The value it should equal.
     * @param message Optional context shown on failure.
     * @param loc The source location of the call (defaulted to the call site).
     */
    template <typename A, typename B>
    inline void expect_eq(
        const A& actual,
        const B& expected,
        StringView message = "",
        SourceLocation loc = SourceLocation::current()
    ) {
        compare(actual == expected, actual, "==", expected, message, loc);
    }

    /**
     * @brief Expects two values to compare unequal.
     * @tparam A The left-hand operand type.
     * @tparam B The right-hand operand type.
     * @param actual The first value.
     * @param unexpected The value it should differ from.
     * @param message Optional context shown on failure.
     * @param loc The source location of the call (defaulted to the call site).
     */
    template <typename A, typename B>
    inline void expect_ne(
        const A& actual,
        const B& unexpected,
        StringView message = "",
        SourceLocation loc = SourceLocation::current()
    ) {
        compare(actual != unexpected, actual, "!=", unexpected, message, loc);
    }

    /**
     * @brief Expects the left operand to be less than the right.
     * @tparam A The left-hand operand type.
     * @tparam B The right-hand operand type.
     * @param a The left-hand operand.
     * @param b The right-hand operand.
     * @param message Optional context shown on failure.
     * @param loc The source location of the call (defaulted to the call site).
     */
    template <typename A, typename B>
    inline void expect_lt(
        const A& a,
        const B& b,
        StringView message = "",
        SourceLocation loc = SourceLocation::current()
    ) {
        compare(a < b, a, "<", b, message, loc);
    }

    /**
     * @brief Expects the left operand to be less than or equal to the right.
     * @tparam A The left-hand operand type.
     * @tparam B The right-hand operand type.
     * @param a The left-hand operand.
     * @param b The right-hand operand.
     * @param message Optional context shown on failure.
     * @param loc The source location of the call (defaulted to the call site).
     */
    template <typename A, typename B>
    inline void expect_le(
        const A& a,
        const B& b,
        StringView message = "",
        SourceLocation loc = SourceLocation::current()
    ) {
        compare(a <= b, a, "<=", b, message, loc);
    }

    /**
     * @brief Expects the left operand to be greater than the right.
     * @tparam A The left-hand operand type.
     * @tparam B The right-hand operand type.
     * @param a The left-hand operand.
     * @param b The right-hand operand.
     * @param message Optional context shown on failure.
     * @param loc The source location of the call (defaulted to the call site).
     */
    template <typename A, typename B>
    inline void expect_gt(
        const A& a,
        const B& b,
        StringView message = "",
        SourceLocation loc = SourceLocation::current()
    ) {
        compare(a > b, a, ">", b, message, loc);
    }

    /**
     * @brief Expects the left operand to be greater than or equal to the right.
     * @tparam A The left-hand operand type.
     * @tparam B The right-hand operand type.
     * @param a The left-hand operand.
     * @param b The right-hand operand.
     * @param message Optional context shown on failure.
     * @param loc The source location of the call (defaulted to the call site).
     */
    template <typename A, typename B>
    inline void expect_ge(
        const A& a,
        const B& b,
        StringView message = "",
        SourceLocation loc = SourceLocation::current()
    ) {
        compare(a >= b, a, ">=", b, message, loc);
    }

    /**
     * @brief Expects a floating-point value to lie within epsilon of another.
     * @tparam F The floating-point type.
     * @param actual The value produced by the code under test.
     * @param expected The value it should approximate.
     * @param epsilon The maximum tolerated absolute difference.
     * @param message Optional context shown on failure.
     * @param loc The source location of the call (defaulted to the call site).
     */
    template <FloatingPoint F>
    inline void expect_near(
        F actual,
        F expected,
        F epsilon,
        StringView message = "",
        SourceLocation loc = SourceLocation::current()
    ) {
        const F difference = actual > expected ? actual - expected : expected - actual;
        if (difference <= epsilon) {
            pass();
            return;
        }
        fail(
            loc,
            annotate(Ops::format("expected {} within {} of {}", actual, epsilon, expected), message)
        );
    }

    /**
     * @brief Expects the callable to throw an exception of type E.
     * @tparam E The expected exception type.
     * @tparam Fn The callable type.
     * @param fn The callable expected to throw.
     * @param message Optional context shown on failure.
     * @param loc The source location of the call (defaulted to the call site).
     */
    template <typename E, typename Fn>
    inline void expect_throws(
        Fn&& fn,
        StringView message = "",
        SourceLocation loc = SourceLocation::current()
    ) {
        try {
            fn();
        } catch (const E& _) {
            pass();
            return;
        } catch (const Exception& e) {
            fail(loc, annotate(Ops::format("threw a different exception: {}", e.what()), message));
            return;
        } catch (...) {
            fail(loc, annotate("threw an unrecognized exception", message));
            return;
        }
        fail(loc, annotate("no exception was thrown", message));
    }

    /**
     * @brief Expects the callable to complete without throwing.
     * @tparam Fn The callable type.
     * @param fn The callable expected not to throw.
     * @param message Optional context shown on failure.
     * @param loc The source location of the call (defaulted to the call site).
     */
    template <typename Fn>
    inline void expect_no_throw(
        Fn&& fn,
        StringView message = "",
        SourceLocation loc = SourceLocation::current()
    ) {
        try {
            fn();
            pass();
        } catch (const Exception& e) {
            fail(loc, annotate(Ops::format("threw: {}", e.what()), message));
        } catch (...) {
            fail(loc, annotate("threw an unrecognized exception", message));
        }
    }

    /**
     * @brief Records a fatal condition; on failure the current test is aborted.
     * @param condition The condition required to hold.
     * @param message Optional context shown on failure.
     * @param loc The source location of the call (defaulted to the call site).
     */
    inline void require(
        bool condition,
        StringView message = "",
        SourceLocation loc = SourceLocation::current()
    ) {
        if (condition) {
            pass();
            return;
        }
        fail(loc, annotate("requirement failed", message));
        abort_test();
    }

    /**
     * @brief Requires two values to compare equal, aborting the test otherwise.
     * @tparam A The actual value type.
     * @tparam B The expected value type.
     * @param actual The value produced by the code under test.
     * @param expected The value it must equal.
     * @param message Optional context shown on failure.
     * @param loc The source location of the call (defaulted to the call site).
     */
    template <typename A, typename B>
    inline void require_eq(
        const A& actual,
        const B& expected,
        StringView message = "",
        SourceLocation loc = SourceLocation::current()
    ) {
        if (actual == expected) {
            pass();
            return;
        }
        fail(loc, annotate(Ops::format("expected {} == {}", actual, expected), message));
        abort_test();
    }

    /**
     * @brief Skips the current test, recording the given reason.
     * @param reason The human-readable reason for skipping.
     */
    [[noreturn]]
    inline void skip(StringView reason = "") {
        skip_test(reason);
    }
}
