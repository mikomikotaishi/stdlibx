#pragma once

using stdx::collections::Vector;
using stdx::future::Future;
using stdx::future::FutureStatus;
using stdx::future::LaunchPolicy;
using stdx::io::TextStyle;
using stdx::mem::Pointers;
using stdx::sync::ConditionVariable;
using stdx::sync::LockGuard;
using stdx::sync::Mutex;
using stdx::sync::UniqueLock;
using stdx::thread::StopToken;
using stdx::thread::Thread;
using stdx::time::Duration;
using stdx::time::Nanoseconds;

#ifdef __cpp_impl_reflection
using stdx::meta::reflect::Info;
#endif

/**
 * @namespace stdx::test
 * @brief Minimal assertion-based unit-testing framework.
 */
export namespace stdx::test {
    #ifdef __cpp_impl_reflection
    /**
     * @concept ScannableScope
     * @brief An Info that test discovery can scan: a namespace, or a class whose
     * static member functions are the tests.
     * @tparam Nsp The reflection to check, e.g. ^^tests or ^^Fixture.
     *
     * Spelled as a concept rather than repeated as a requires-clause at each use,
     * for two reasons beyond brevity. A requires-clause takes a primary expression,
     * so `requires reflect::is_namespace(Nsp) || ...` is ill-formed without an extra
     * pair of parentheses, which is easy to omit; naming a concept is a primary
     * expression and needs none. And the class case is not `is_class_type` alone -
     * that is true of unions too, which have no meaningful tests to find.
     */
    template <Info Nsp>
    concept ScannableScope = reflect::is_namespace(Nsp)
        || (reflect::is_class_type(Nsp) && !reflect::is_union_type(Nsp));
    #endif

    /**
     * @class Test
     * @brief A single named test: a function plus optional tags.
     */
    struct Test {
        using Callback = void(*)(); ///< The type of the test body function.
        using CancellableCallback = void(*)(StopToken); ///< A test body that polls for a stop request.

        StringView name; ///< Test display name.
        Callback fn; ///< The test body.
        Vector<StringView> tags; ///< Optional tags for --tag filtering.
        Optional<StringView> disabled = nullopt; ///< When set, the test is reported skipped, with this reason, without running.
        Optional<Nanoseconds> timeout = nullopt; ///< When set, the test fails if its body runs longer than this.

        /**
         * When set, a timed-out test is asked to stop rather than simply
         * abandoned: run_body launches it on a std::jthread and calls
         * request_stop() on timeout instead of orphaning a std::future. The
         * test body must poll the token itself (e.g. `if (token.stop_requested())
         * return;` inside its loop) - nothing forces it to notice, this only
         * gives it the chance to. fn is still required and still runs when no
         * timeout is set, so a cancellable test works the same as any other
         * when it isn't given a budget.
         */
        Optional<CancellableCallback> cancellable_fn = nullopt;
    };

    /**
     * @class Suite
     * @brief A group of tests with optional per-test and per-suite hooks.
     */
    struct Suite {
        using Callback = void(*)(); ///< The type of a hook function.

        StringView name = ""; ///< Optional suite name.
        Callback before_each = nullptr; ///< Run before every selected test.
        Callback after_each = nullptr; ///< Run after every selected test.
        Callback before_all = nullptr; ///< Run once before the first selected test.
        Callback after_all = nullptr; ///< Run once after the last selected test.
        Vector<Test> tests = {}; ///< The tests in this suite.

        #ifdef __cpp_impl_reflection
        /**
         * @brief The suite a namespace declares: the tests reflection finds in it,
         * under the hooks it declares.
         * @tparam Nsp The reflection of the namespace or class to scan, e.g. ^^tests.
         * @return A suite named after the namespace.
         *
         * A test is a nullary void member named test_* or annotated with TestCase; a
         * hook is one annotated with BeforeEach, AfterEach, BeforeAll or AfterAll, or
         * simply named before_each, after_each, before_all or after_all. Defined in
         * discovery.inl, alongside the machinery it drives.
         */
        template <Info Nsp>
            requires ScannableScope<Nsp>
        [[nodiscard]]
        static Suite of();
        #endif
    };
}

namespace stdx::test {
    /**
     * @internal
     * @struct Options
     * @brief Command-line options parsed from argv.
     */
    struct Options {
        StringView filter = ""; ///< Name glob; only matching tests run.
        StringView tag = ""; ///< Required tag; only tagged tests run.
        bool list = false; ///< Print test names and exit.
        bool verbose = false; ///< Print a line for passing tests too.
        bool color = true; ///< Colorize the output.
    };

    /**
     * @internal
     * @struct Tally
     * @brief Running pass/fail/skip totals.
     */
    struct Tally {
        usize passed = 0; ///< Tests with no failed assertions.
        usize failed = 0; ///< Tests with at least one failed assertion.
        usize skipped = 0; ///< Tests that called skip().
    };

    /**
     * @internal
     * @brief Matches text against a pattern supporting the '*' wildcard.
     * @param pattern The glob pattern.
     * @param text The text to match.
     * @return True if the whole text matches the pattern.
     */
    [[nodiscard]]
    inline bool wildcard_match(StringView pattern, StringView text) noexcept {
        usize p = 0;
        usize t = 0;
        usize star = 0;
        usize mark = 0;
        bool has_star = false;
        while (t < text.size()) {
            if (p < pattern.size() && pattern[p] == text[t]) {
                ++p;
                ++t;
            } else if (p < pattern.size() && pattern[p] == '*') {
                has_star = true;
                star = p;
                mark = t;
                ++p;
            } else if (has_star) {
                p = star + 1;
                ++mark;
                t = mark;
            } else {
                return false;
            }
        }
        while (p < pattern.size() && pattern[p] == '*') {
            ++p;
        }
        return p == pattern.size();
    }

    /**
     * @internal
     * @brief Parses the runner options from a command line.
     * @param args The command line, argv[0] first - as Environment::args reports it.
     * @return The parsed options.
     */
    [[nodiscard]]
    inline Options parse_options(Span<const StringView> args) {
        Options options;
        for (usize i = 1; i < args.size(); ++i) {
            const StringView arg = args[i];
            if (arg == "--filter" && i + 1 < args.size()) {
                options.filter = args[++i];
            } else if (arg == "--tag" && i + 1 < args.size()) {
                options.tag = args[++i];
            } else if (arg == "--list") {
                options.list = true;
            } else if (arg == "--verbose" || arg == "-v") {
                options.verbose = true;
            } else if (arg == "--no-color") {
                options.color = false;
            }
        }
        return options;
    }

    /**
     * @internal
     * @brief Adapts a main() argument vector to the Span form the runner uses.
     * @param argc The argument count.
     * @param argv The argument vector.
     * @return The arguments as views, argv[0] first.
     *
     * The result owns the view array, so callers keep it alive for as long as they
     * hold the span - the views themselves point into argv, which outlives them.
     */
    [[nodiscard]]
    inline Vector<StringView> args_of(int argc, char* argv[]) {
        Vector<StringView> args;
        if (argv == nullptr || argc <= 0) {
            return args;
        }
        args.reserve(static_cast<usize>(argc));
        for (i32 i = 0; i < argc; ++i) {
            if (argv[i] != nullptr) {
                args.emplace_back(argv[i]);
            }
        }
        return args;
    }

    /**
     * @internal
     * @brief Decides whether a test is selected by the current options.
     * @param test The test to consider.
     * @param options The active options.
     * @return True if the test should run.
     */
    [[nodiscard]]
    inline bool selected(const Test& test, const Options& options) noexcept {
        if (!options.filter.empty() && !wildcard_match(options.filter, test.name)) {
            return false;
        }
        if (!options.tag.empty()) {
            for (StringView tag: test.tags) {
                if (tag == options.tag) {
                    return true;
                }
            }
            return false;
        }
        return true;
    }

    /**
     * @internal
     * @brief Prints one status line, optionally colorized.
     * @param status The status label, e.g. "PASS".
     * @param color The color to use when color is enabled.
     * @param line The remainder of the line.
     * @param use_color Whether to colorise the line.
     */
    inline void report(
        StringView status,
        TextStyle::Color color,
        StringView line,
        bool use_color
    ) {
        if (use_color) {
            stdx::io::print(TextStyle().fg(color), "{}", status);
            System::out.println("  {}", line);
        } else {
            System::out.println("{}  {}", status, line);
        }
    }

    /**
     * @internal
     * @enum BodyOutcome
     * @brief How a test body's run ended, decided on whichever thread ran it
     * and read back on the caller's thread once that is safe to do.
     */
    enum class BodyOutcome {
        COMPLETED, ///< Returned normally.
        ABORTED, ///< Threw TestAbort - a require_* or contract handler already reported it.
        SKIPPED, ///< Threw TestSkipped.
        THREW, ///< Threw a recognized Exception.
        THREW_UNKNOWN, ///< Threw something not derived from Exception.
    };

    /**
     * @internal
     * @struct BodyResult
     * @brief The outcome of a test body plus whatever it needs reported.
     */
    struct BodyResult {
        BodyOutcome outcome = BodyOutcome::COMPLETED;
        String detail; ///< The skip reason (SKIPPED) or exception message (THREW).
    };

    /**
     * @internal
     * @brief Runs fn, turning the exceptions run_one already knows how to
     * handle into a BodyResult instead of letting them propagate.
     * @param fn The test body.
     * @return What happened.
     *
     * Shared by both run_body overloads so a cancellable test's cross-thread
     * run and a plain test's same-thread run report identically. This must
     * never let an exception escape when called as a std::jthread's
     * function - jthread, like thread, calls std::terminate() if one does -
     * so every exception type run_one previously caught directly is caught
     * here too, without exception.
     */
    template <typename Fn>
    [[nodiscard]]
    inline BodyResult run_body_capturing(Fn&& fn) {
        try {
            fn();
            return BodyResult {.outcome = BodyOutcome::COMPLETED};
        } catch (const TestAbort& _) {
            return BodyResult {.outcome = BodyOutcome::ABORTED};
        } catch (const TestSkipped& s) {
            return BodyResult {.outcome = BodyOutcome::SKIPPED, .detail = String(s.what())};
        } catch (const Exception& e) {
            return BodyResult {.outcome = BodyOutcome::THREW, .detail = String(e.what())};
        } catch (...) {
            return BodyResult {.outcome = BodyOutcome::THREW_UNKNOWN};
        }
    }

    /**
     * @internal
     * @struct Completion
     * @brief A one-shot, wait_for-capable "is it done yet" signal.
     *
     * std::jthread only offers a blocking join() - no wait_for - so a bounded
     * wait for one needs its own condition variable rather than anything the
     * thread type provides directly.
     */
    struct Completion {
        Mutex mutex;
        ConditionVariable ready;
        bool done = false;

        void signal() {
            const LockGuard<Mutex> guard(mutex);
            done = true;
            ready.notify_all();
        }

        /**
         * @return True if signal() had already happened within the wait.
         */
        template <typename Rep, typename Period>
        [[nodiscard]]
        bool wait_for(const Duration<Rep, Period>& timeout) {
            UniqueLock<Mutex> lock(mutex);
            return ready.wait_for(lock, timeout, [this] -> bool { return done; });
        }
    };

    /**
     * @internal
     * @brief Runs a test body, enforcing an optional timeout.
     * @param fn The test body.
     * @param cancellable_fn When set, used instead of fn once a timeout is also
     * set, so the body can be asked to stop rather than merely abandoned.
     * @param timeout When set, the longest the body is allowed to run.
     * @return The outcome, or nullopt if the timeout elapsed first.
     */
    [[nodiscard]]
    inline Optional<BodyResult> run_body(
        Test::Callback fn,
        const Optional<Test::CancellableCallback>& cancellable_fn,
        const Optional<Nanoseconds>& timeout
    ) {
        if (!timeout.has_value()) {
            return run_body_capturing([fn] -> void { fn(); });
        }
        if (cancellable_fn.has_value()) {
            auto result = Pointers::shared<BodyResult>();
            auto completion = Pointers::shared<Completion>();
            Thread worker([cancellable_fn, result, completion](StopToken token) -> void {
                *result = run_body_capturing([cancellable_fn, token] -> void { (*cancellable_fn)(token); });
                completion->signal();
            });
            if (completion->wait_for(*timeout)) {
                worker.join();
                return *result;
            }
            worker.request_stop();
            static constexpr Nanoseconds GRACE = 50ms;
            if (completion->wait_for(GRACE)) {
                worker.join();
                return *result;
            }
            static Vector<Thread> orphaned;
            worker.detach();
            orphaned.push_back(Ops::move(worker));
            return nullopt;
        }
        Future<BodyResult> future = stdx::future::async(
            LaunchPolicy::ASYNC,
            [fn] -> BodyResult { return run_body_capturing([fn] -> void { fn(); }); }
        );
        if (future.wait_for(*timeout) == FutureStatus::TIMEOUT) {
            static Vector<Future<BodyResult>> orphaned;
            orphaned.push_back(Ops::move(future));
            return nullopt;
        }
        return future.get();
    }

    /**
     * @internal
     * @brief Runs a single test, catching aborts, skips and stray exceptions.
     * @param test The test to run.
     * @param suite The owning suite (for its hooks).
     * @param options The active options.
     * @param tally The running totals to update.
     *
     * A test carrying a disabled reason is reported skipped without being run.
     */
    inline void run_one(
        const Test& test,
        const Suite& suite,
        const Options& options,
        Tally& tally
    ) {
        Context& ctx = Context::context();
        if (test.disabled.has_value()) {
            ++tally.skipped;
            const StringView why = *test.disabled;
            report(
                "SKIP",
                TextStyle::Color::YELLOW,
                why.empty() ? String(test.name) : Ops::fmt("{} - {}", test.name, why),
                options.color
            );
            return;
        }
        ctx.begin_test();
        bool skipped = false;
        bool timed_out = false;
        String skip_reason;
        const u64 start = System::nano_time();
        try {
            if (suite.before_each != nullptr) {
                suite.before_each();
            }
            const Optional<BodyResult> outcome = run_body(test.fn, test.cancellable_fn, test.timeout);
            if (!outcome.has_value()) {
                timed_out = true;
            } else {
                switch (outcome->outcome) {
                    case BodyOutcome::COMPLETED:
                        break;
                    case BodyOutcome::ABORTED:
                        // A require_* assertion or the contract-violation
                        // handler already reported the failure.
                        break;
                    case BodyOutcome::SKIPPED:
                        skipped = true;
                        skip_reason = outcome->detail;
                        break;
                    case BodyOutcome::THREW:
                        ctx.record_error();
                        System::err.println("    uncaught exception: {}", outcome->detail);
                        break;
                    case BodyOutcome::THREW_UNKNOWN:
                        ctx.record_error();
                        System::err.println("    uncaught unrecognized exception");
                        break;
                }
            }
        } catch (const Exception& e) {
            ctx.record_error();
            System::err.println("    uncaught exception in before_each: {}", e.what());
        } catch (...) {
            ctx.record_error();
            System::err.println("    uncaught unrecognized exception in before_each");
        }
        try {
            if (suite.after_each != nullptr) {
                suite.after_each();
            }
        } catch (...) {
            // Teardown failures are ignored.
        }
        ctx.end_test();
        const f64 elapsed_ms = static_cast<f64>(System::nano_time() - start) / 1.0e6;
        String line = Ops::fmt(
            "{} ({:.3f} ms, {} assertions)",
            test.name,
            elapsed_ms,
            ctx.test_assertions()
        );
        if (timed_out) {
            const f64 timeout_ms = static_cast<f64>(test.timeout->count()) / 1.0e6;
            line = Ops::fmt(
                "{} - exceeded its {:.3f} ms timeout; its thread is still running in the "
                "background and may still record assertions against later tests",
                line,
                timeout_ms
            );
        }
        if (skipped) {
            ++tally.skipped;
            report(
                "SKIP",
                TextStyle::Color::YELLOW,
                skip_reason.empty() ? line : Ops::fmt("{} - {}", line, skip_reason),
                options.color
            );
        } else if (timed_out || ctx.test_failures() != 0) {
            ++tally.failed;
            report("FAIL", TextStyle::Color::RED, line, options.color);
        } else {
            ++tally.passed;
            if (options.verbose) {
                report("PASS", TextStyle::Color::GREEN, line, options.color);
            }
        }
    }

    /**
     * @internal
     * @brief Runs all selected tests across the given suites and prints a summary.
     * @param argc The argument count.
     * @param argv The argument vector.
     * @param suites The suites to run.
     * @return 0 if no test failed, 1 otherwise.
     */
    [[nodiscard]]
    inline int run_impl(Span<const StringView> args, InitializerList<Suite> suites) {
        const Options options = parse_options(args);
        Context::context().color(options.color);
        if (options.list) {
            for (const Suite& suite: suites) {
                for (const Test& test: suite.tests) {
                    System::out.println("{}", test.name);
                }
            }
            return System::EXIT_SUCCESS;
        }
        Tally tally;
        const u64 start = System::nano_time();
        for (const Suite& suite: suites) {
            bool any = false;
            for (const Test& test: suite.tests) {
                if (selected(test, options)) {
                    any = true;
                    break;
                }
            }
            if (any && suite.before_all != nullptr) {
                suite.before_all();
            }
            for (const Test& test: suite.tests) {
                if (selected(test, options)) {
                    run_one(test, suite, options, tally);
                }
            }
            if (any && suite.after_all != nullptr) {
                suite.after_all();
            }
        }
        const f64 elapsed_ms = static_cast<f64>(System::nano_time() - start) / 1.0e6;
        const String summary = Ops::fmt(
            "{} passed, {} failed, {} skipped - {} assertions, {:.3f} ms",
            tally.passed,
            tally.failed,
            tally.skipped,
            Context::context().total_assertions(),
            elapsed_ms
        );
        if (tally.failed == 0) {
            if (options.color) {
                stdx::io::println(TextStyle().fg(TextStyle::Color::GREEN), "{}", summary);
            } else {
                System::out.println("{}", summary);
            }
        } else if (options.color) {
            stdx::io::println(TextStyle().fg(TextStyle::Color::RED), "{}", summary);
        } else {
            System::err.println("{}", summary);
        }
        return tally.failed == 0 ? System::EXIT_SUCCESS : System::EXIT_FAILURE;
    }
}

export namespace stdx::test {
    /**
     * @brief Runs a flat list of tests as an anonymous suite.
     * @param argc The argument count from main.
     * @param argv The argument vector from main.
     * @param tests The tests to run.
     * @return 0 if no test failed, 1 otherwise.
     */
    int run(int argc, char* argv[], InitializerList<Test> tests) {
        const Vector<StringView> args = args_of(argc, argv);
        return run_impl(args, {Suite {.tests = Vector<Test>(tests)}});
    }

    /**
     * @brief Runs tests as an anonymous suite, taking the command line from the process.
     * @param tests The tests to run.
     * @return 0 if no test failed, 1 otherwise.
     *
     * Equivalent to the argc/argv overload with Environment::args(), which needs no
     * cooperation from main - so `int main() { return run({...}); }` works.
     */
    int run(InitializerList<Test> tests) {
        return run_impl(Environment::args(), {Suite {.tests = Vector<Test>(tests)}});
    }

    /**
     * @brief Runs tests passed as separate arguments as an anonymous suite.
     * @param argc The argument count from main.
     * @param argv The argument vector from main.
     * @param tests The tests to run, each given as its own argument.
     * @return 0 if no test failed, 1 otherwise.
     *
     * The variadic form accepts explicitly-typed Test arguments, e.g.
     * run(argc, argv, Test {...}, Test {...}).
     */
    int run(int argc, char* argv[], DecaysTo<Test> auto&&... tests) {
        Vector<Test> list;
        list.reserve(sizeof...(tests));
        (list.push_back(Ops::forward<decltype(tests)>(tests)), ...);
        const Vector<StringView> args = args_of(argc, argv);
        return run_impl(args, {Suite {.tests = Ops::move(list)}});
    }

    /**
     * @brief Runs tests given as separate arguments, taking the command line from
     * the process. See the argc/argv overload.
     * @param tests The tests to run, each given as its own argument.
     * @return 0 if no test failed, 1 otherwise.
     */
    int run(DecaysTo<Test> auto&&... tests) {
        Vector<Test> list;
        list.reserve(sizeof...(tests));
        (list.push_back(Ops::forward<decltype(tests)>(tests)), ...);
        return run_impl(Environment::args(), {Suite {.tests = Ops::move(list)}});
    }

    /**
     * @brief Runs a single suite, honouring its setup and teardown hooks.
     * @param argc The argument count from main.
     * @param argv The argument vector from main.
     * @param suite The suite to run.
     * @return 0 if no test failed, 1 otherwise.
     *
     * Pass an explicit Suite {...} to reach this overload; a brace-enclosed list
     * of {name, fn} entries selects the InitializerList<Test> overload instead.
     */
    int run(int argc, char* argv[], const Suite& suite) {
        const Vector<StringView> args = args_of(argc, argv);
        return run_impl(args, {suite});
    }

    /**
     * @brief Runs a single suite, taking the command line from the process.
     * @param suite The suite to run.
     * @return 0 if no test failed, 1 otherwise.
     */
    int run(const Suite& suite) {
        return run_impl(Environment::args(), {suite});
    }

    /**
     * @brief Runs several suites in order under one summary.
     * @param argc The argument count from main.
     * @param argv The argument vector from main.
     * @param suites The suites to run.
     * @return 0 if no test failed, 1 otherwise.
     *
     * Each suite's hooks run around its own tests only. This is spelled differently
     * from run() because a brace-enclosed {name, fn} entry initializes a Suite just
     * as readily as a Test, which would make every run(argc, argv, {...}) call
     * ambiguous.
     */
    int run_suites(int argc, char* argv[], InitializerList<Suite> suites) {
        const Vector<StringView> args = args_of(argc, argv);
        return run_impl(args, suites);
    }

    /**
     * @brief Runs several suites, taking the command line from the process.
     * @param suites The suites to run.
     * @return 0 if no test failed, 1 otherwise.
     */
    int run_suites(InitializerList<Suite> suites) {
        return run_impl(Environment::args(), suites);
    }

    /**
     * @brief Runs suites passed as separate arguments as an anonymous suite.
     * @param argc The argument count from main.
     * @param argv The argument vector from main.
     * @param suites The suites to run, each given as its own argument.
     * @return 0 if no test failed, 1 otherwise.
     *
     * The variadic form accepts explicitly-typed Suite arguments, e.g.
     * run(argc, argv, Suite {...}, Suite {...}).
     */
    int run(int argc, char* argv[], DecaysTo<Suite> auto&&... suites) {
        const Vector<StringView> args = args_of(argc, argv);
        return run_impl(args, {Ops::forward<decltype(suites)>(suites)...});
    }

    /**
     * @brief Runs suites given as separate arguments, taking the command line from
     * the process. See the argc/argv overload.
     * @param suites The suites to run, each given as its own argument.
     * @return 0 if no test failed, 1 otherwise.
     */
    int run(DecaysTo<Suite> auto&&... suites) {
        return run_impl(Environment::args(), {Ops::forward<decltype(suites)>(suites)...});
    }
}
