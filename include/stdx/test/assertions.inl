#pragma once

using stdx::debug::SourceLocation;
using stdx::meta::TypeIdentityType;
using stdx::sync::Atomic;
using stdx::sync::MemoryOrder;

namespace stdx::test {
    #ifdef __cpp_lib_contracts
    /**
     * @internal
     * @struct ViolationRecord
     * @brief The parts of a contract violation the matchers compare against.
     *
     * ContractViolation itself can be neither copied nor kept beyond the
     * handler, so the handler copies these out while it runs.
     */
    struct ViolationRecord {
        AssertionKind::Self kind; ///< Which flavor of contract was violated.
        String comment; ///< The predicate text of the violated contract.
        SourceLocation location; ///< Where the violated contract was written.
    };
    #endif

    /**
     * @internal
     * @class Context
     * @brief Per-run state shared by the assertions and the runner.
     */
    class [[nodiscard]] Context {
    private:
        Atomic<usize> _total_assertions_count{0}; ///< Assertions evaluated across the whole run.
        Atomic<usize> _test_assertions_count{0}; ///< Assertions evaluated in the current test.
        Atomic<usize> _test_failures_count{0}; ///< Failed assertions in the current test.
        bool _use_color = true; ///< Set once before the run, read-only while it runs.
        bool _test_active = false; ///< True while the runner is executing a test.
        #ifdef __cpp_lib_contracts
        bool _expecting_violation = false; ///< True while expect_contract_violation runs its callable.
        Optional<ViolationRecord> _captured_violation = nullopt; ///< The violation trapped for the waiting expectation.
        #endif

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
            _total_assertions_count.store(0, MemoryOrder::RELAXED);
            _test_assertions_count.store(0, MemoryOrder::RELAXED);
            _test_failures_count.store(0, MemoryOrder::RELAXED);
            _use_color = true;
            _test_active = false;
            #ifdef __cpp_lib_contracts
            _expecting_violation = false;
            _captured_violation = nullopt;
            #endif
        }

        /**
         * @brief Resets the per-test counters before a test runs.
         */
        void begin_test() noexcept {
            _test_assertions_count.store(0, MemoryOrder::RELAXED);
            _test_failures_count.store(0, MemoryOrder::RELAXED);
            _test_active = true;
        }

        /**
         * @brief Marks the end of the running test.
         */
        void end_test() noexcept {
            _test_active = false;
        }

        /**
         * @brief Atomically records a passing assertion.
         */
        void record_pass() noexcept {
            _total_assertions_count.fetch_add(1, MemoryOrder::RELAXED);
            _test_assertions_count.fetch_add(1, MemoryOrder::RELAXED);
        }

        /**
         * @brief Atomically records a failing assertion.
         */
        void record_failure() noexcept {
            _total_assertions_count.fetch_add(1, MemoryOrder::RELAXED);
            _test_assertions_count.fetch_add(1, MemoryOrder::RELAXED);
            _test_failures_count.fetch_add(1, MemoryOrder::RELAXED);
        }

        /**
         * @brief Atomically records a non-assertion test failure (e.g. an uncaught exception).
         */
        void record_error() noexcept {
            _test_failures_count.fetch_add(1, MemoryOrder::RELAXED);
        }

        /**
         * @brief Returns the total number of assertions evaluated across the whole run.
         * @return The total assertion count.
         */
        [[nodiscard]]
        usize total_assertions() const noexcept {
            return _total_assertions_count.load(MemoryOrder::RELAXED);
        }

        /**
         * @brief Returns the number of assertions evaluated in the current test.
         * @return The current test's assertion count.
         */         
        [[nodiscard]]
        usize test_assertions() const noexcept {
            return _test_assertions_count.load(MemoryOrder::RELAXED);
        }

        /**
         * @brief Returns the number of failed assertions in the current test.
         * @return The current test's failure count.
         */
        [[nodiscard]]
        usize test_failures() const noexcept {
            return _test_failures_count.load(MemoryOrder::RELAXED);
        }

        /**
         * @brief Returns whether the test run should colorize its output.
         * @return True if color is enabled, false otherwise.
         */
        [[nodiscard]]
        bool color_enabled() const noexcept {
            return _use_color;
        }

        /**
         * @brief Enables or disables colorized output for the test run.
         * @param enable True to enable color, false to disable.
         */
        void color(bool enable) noexcept {
            _use_color = enable;
        }

        /**
         * @brief Returns whether a test body is currently running.
         * @return True between begin_test() and end_test().
         */
        [[nodiscard]]
        bool test_active() const noexcept {
            return _test_active;
        }

        #ifdef __cpp_lib_contracts
        /**
         * @brief Arms or disarms the trap for an expected contract violation.
         * @param armed True to trap the next violation, false to stand down.
         *
         * Arming clears any previously captured violation. The trap is read and
         * written only on the thread running the test, so a violation raised on
         * another thread is never captured.
         */
        void expecting_violation(bool armed) {
            _expecting_violation = armed;
            if (armed) {
                _captured_violation = nullopt;
            }
        }

        /**
         * @brief Returns whether expect_contract_violation awaits a violation.
         * @return True while the trap is armed.
         */
        [[nodiscard]]
        bool violation_expected() const noexcept {
            return _expecting_violation;
        }

        /**
         * @brief Stores the violation the armed trap caught.
         * @param record The violation, copied out of the handler.
         */
        void capture_violation(ViolationRecord record) {
            _captured_violation = Ops::move(record);
        }

        /**
         * @brief Returns the violation the trap caught, if any.
         * @return The captured violation, or nullopt.
         */
        [[nodiscard]]
        const Optional<ViolationRecord>& captured_violation() const noexcept {
            return _captured_violation;
        }
        #endif
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
            detail += Ops::fmt(" ({})", message);
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
        fail(loc, annotate(Ops::fmt("expected {} {} {}", a, op, b), message));
    }

    #ifdef __cpp_lib_contracts
    /**
     * @internal
     * @brief Names an assertion kind the way it is spelled in source.
     * @param kind The kind to name.
     * @return "pre", "post" or "contract_assert".
     */
    [[nodiscard]]
    inline StringView kind_name(AssertionKind kind) noexcept {
        switch (kind) {
            case AssertionKind::PRE:
                return "pre";
            case AssertionKind::POST:
                return "post";
            case AssertionKind::ASSERT:
                return "contract_assert";
        }
        Ops::unreachable();
    }
    #endif
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

    #ifdef __cpp_lib_contracts
    /**
     * @class ContractViolationAbort
     * @brief Thrown by the violation handler to unwind out of a contract violation.
     * @extends TestAbort
     *
     * Extends TestAbort so the runner's existing catch also unwinds a violating
     * test; expect_contract_violation catches it first and passes instead. It is
     * thrown and caught only inside the module, so its RTTI never crosses the
     * module boundary.
     */
    class ContractViolationAbort: public TestAbort {
    public:
        using TestAbort::TestAbort;
    };
    #endif

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

    #ifdef __cpp_lib_contracts
    /**
     * @brief Routes a contract violation into the test run.
     * @param violation The violation the contract runtime reported.
     *
     * The stdlibx_test_contracts object library defines the replaceable
     * ::handle_contract_violation to forward here, so a test binary that links
     * it turns contract violations into test outcomes instead of process
     * termination. While expect_contract_violation is waiting, the violation is
     * captured for matching and thrown to it; otherwise a violation inside a
     * running test is recorded as a failed assertion, aborting the test when
     * the semantic is enforce and continuing when it is observe. Outside any
     * test the default handler decides, as if it had never been replaced.
     * Violations under ignore or quick_enforce never reach a handler at all.
     *
     * The linked handler definition is weak, so a test binary that defines its
     * own ::handle_contract_violation takes precedence; such a handler can call
     * this function directly to reuse the framework's routing.
     */
    void on_contract_violation(const ContractViolation& violation) {
        Context& ctx = Context::context();
        if (ctx.violation_expected()) {
            ctx.capture_violation(ViolationRecord {
                .kind = violation.kind(),
                .comment = String(violation.comment()),
                .location = violation.location()
            });
            throw ContractViolationAbort();
        }
        if (!ctx.test_active()) {
            invoke_default_contract_violation_handler(violation);
            return;
        }
        const StringView note = violation.mode() == DetectionMode::EVALUATION_EXCEPTION
            ? ", its predicate exited via an exception"
            : "";
        fail(
            violation.location(),
            Ops::fmt("{} '{}' violated{}", kind_name(violation.kind()), violation.comment(), note)
        );
        if (violation.semantic() == EvaluationSemantic::ENFORCE) {
            throw ContractViolationAbort();
        }
    }
    #endif

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
     *
     * Only the actual value takes part in deduction, so the tolerance and the
     * expected value may be written as plain literals even when the value under
     * test is of an extended floating-point type such as f64.
     */
    template <FloatingPoint F>
    inline void expect_near(
        F actual,
        TypeIdentityType<F> expected,
        TypeIdentityType<F> epsilon,
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
            annotate(Ops::fmt("expected {} within {} of {}", actual, epsilon, expected), message)
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
            fail(loc, annotate(Ops::fmt("threw a different exception: {}", e.what()), message));
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
            fail(loc, annotate(Ops::fmt("threw: {}", e.what()), message));
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
        fail(loc, annotate(Ops::fmt("expected {} == {}", actual, expected), message));
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

#ifdef __cpp_lib_contracts
namespace stdx::test {
    /**
     * @internal
     * @enum ViolationOutcome
     * @brief How the callable under expect_contract_violation came back.
     */
    enum class ViolationOutcome {
        VIOLATED, ///< A contract violation was captured.
        COMPLETED, ///< The callable returned with no contract violated.
        THREW ///< The callable exited via an ordinary exception instead.
    };

    /**
     * @internal
     * @brief Arms the context and invokes the callable, trapping the violation.
     * @param invoke Type-erased trampoline that calls the callable.
     * @param fn The callable, behind a void pointer.
     * @param thrown Receives the exception description on the THREW outcome.
     * @return What the callable did.
     *
     * Non-inline so that ContractViolationAbort is caught in the translation
     * unit that owns its RTTI, mirroring abort_test on the throwing side. The
     * test-control signals TestAbort and TestSkipped pass through untouched.
     */
    [[nodiscard]]
    ViolationOutcome run_expecting_violation(
        void (*invoke)(void*),
        void* fn,
        String& thrown
    ) {
        Context& ctx = Context::context();
        ctx.expecting_violation(true);
        try {
            invoke(fn);
        } catch (const ContractViolationAbort& _) {
            ctx.expecting_violation(false);
            return ViolationOutcome::VIOLATED;
        } catch (const TestAbort& _) {
            ctx.expecting_violation(false);
            throw;
        } catch (const TestSkipped& _) {
            ctx.expecting_violation(false);
            throw;
        } catch (const Exception& e) {
            ctx.expecting_violation(false);
            thrown = e.what();
            return ViolationOutcome::THREW;
        } catch (...) {
            ctx.expecting_violation(false);
            thrown = "an unrecognized exception";
            return ViolationOutcome::THREW;
        }
        ctx.expecting_violation(false);
        return ViolationOutcome::COMPLETED;
    }

    /**
     * @internal
     * @brief Shared implementation of the expect_contract_violation overloads.
     * @tparam Fn The callable type.
     * @param fn The callable expected to violate a contract.
     * @param kind The required assertion kind, or nullopt for any.
     * @param predicate The required predicate text, empty for any.
     * @param message The optional user message.
     * @param loc The source location of the call.
     *
     * The expectation is not nestable: the callable must not itself call
     * expect_contract_violation.
     */
    template <typename Fn>
    inline void expect_violation_impl(
        Fn&& fn,
        const Optional<AssertionKind::Self>& kind,
        StringView predicate,
        StringView message,
        const SourceLocation& loc
    ) {
        auto call = [&fn] -> void { Ops::forward<Fn>(fn)(); };
        String thrown;
        const ViolationOutcome outcome = run_expecting_violation(
            [](void* erased) -> void { (*static_cast<decltype(call)*>(erased))(); },
            &call,
            thrown
        );
        if (outcome == ViolationOutcome::COMPLETED) {
            fail(loc, annotate("no contract violation", message));
            return;
        }
        if (outcome == ViolationOutcome::THREW) {
            fail(loc, annotate(Ops::fmt("threw instead of violating a contract: {}", thrown), message));
            return;
        }
        const ViolationRecord& record = *Context::context().captured_violation();
        if (kind.has_value() && *kind != record.kind) {
            fail(
                loc,
                annotate(
                    Ops::fmt(
                        "expected a {} violation, got {} '{}'",
                        kind_name(*kind),
                        kind_name(record.kind),
                        record.comment
                    ),
                    message
                )
            );
            return;
        }
        if (!predicate.empty() && StringView(record.comment) != predicate) {
            fail(
                loc,
                annotate(
                    Ops::fmt(
                        "expected '{}' to be violated, got {} '{}'",
                        predicate,
                        kind_name(record.kind),
                        record.comment
                    ),
                    message
                )
            );
            return;
        }
        pass();
    }
}

export namespace stdx::test {
    /**
     * @brief Expects the callable to violate some contract.
     * @tparam Fn The callable type.
     * @param fn The callable expected to violate a pre, post or contract_assert.
     * @param message Optional context shown on failure.
     * @param loc The source location of the call (defaulted to the call site).
     *
     * Needs the enforce (default) or observe evaluation semantic and the
     * stdlibx_test_contracts handler linked in; under ignore no violation can
     * reach the test, and under quick_enforce the process traps instead.
     */
    template <typename Fn>
    inline void expect_contract_violation(
        Fn&& fn,
        StringView message = "",
        SourceLocation loc = SourceLocation::current()
    ) {
        expect_violation_impl(Ops::forward<Fn>(fn), nullopt, "", message, loc);
    }

    /**
     * @brief Expects the callable to violate a contract of the given kind.
     * @tparam Fn The callable type.
     * @param kind The kind that must be violated, e.g. AssertionKind::PRE.
     * @param fn The callable expected to violate such a contract.
     * @param message Optional context shown on failure.
     * @param loc The source location of the call (defaulted to the call site).
     */
    template <typename Fn>
    inline void expect_contract_violation(
        AssertionKind kind,
        Fn&& fn,
        StringView message = "",
        SourceLocation loc = SourceLocation::current()
    ) {
        expect_violation_impl(
            Ops::forward<Fn>(fn),
            static_cast<AssertionKind::Self>(kind),
            "",
            message,
            loc
        );
    }

    /**
     * @brief Expects the callable to violate the contract with the given predicate.
     * @tparam Fn The callable type.
     * @param predicate The predicate text of the contract that must be violated,
     * exactly as spelled in source, e.g. "b != 0".
     * @param fn The callable expected to violate that contract.
     * @param message Optional context shown on failure.
     * @param loc The source location of the call (defaulted to the call site).
     */
    template <typename Fn>
    inline void expect_contract_violation(
        StringView predicate,
        Fn&& fn,
        StringView message = "",
        SourceLocation loc = SourceLocation::current()
    ) {
        expect_violation_impl(Ops::forward<Fn>(fn), nullopt, predicate, message, loc);
    }

    /**
     * @brief Expects the callable to violate the given kind of contract with the
     * given predicate.
     * @tparam Fn The callable type.
     * @param kind The kind that must be violated, e.g. AssertionKind::POST.
     * @param predicate The predicate text of the contract that must be violated,
     * exactly as spelled in source.
     * @param fn The callable expected to violate that contract.
     * @param message Optional context shown on failure.
     * @param loc The source location of the call (defaulted to the call site).
     */
    template <typename Fn>
    inline void expect_contract_violation(
        AssertionKind kind,
        StringView predicate,
        Fn&& fn,
        StringView message = "",
        SourceLocation loc = SourceLocation::current()
    ) {
        expect_violation_impl(
            Ops::forward<Fn>(fn),
            static_cast<AssertionKind::Self>(kind),
            predicate,
            message,
            loc
        );
    }
}
#endif
