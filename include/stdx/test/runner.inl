#pragma once

using stdx::collections::Vector;
using stdx::io::TextStyle;

/**
 * @namespace stdx::test
 * @brief Minimal assertion-based unit-testing framework.
 */
export namespace stdx::test {
    /**
     * @class Test
     * @brief A single named test: a function plus optional tags.
     */
    struct Test {
        using Callback = void(*)(); ///< The type of the test body function.

        StringView name; ///< Test display name.
        Callback fn; ///< The test body.
        Vector<StringView> tags; ///< Optional tags for --tag filtering.
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
     * @brief Parses the runner options from argv.
     * @param argc The argument count.
     * @param argv The argument vector.
     * @return The parsed options.
     */
    [[nodiscard]]
    inline Options parse_options(int argc, char* argv[]) {
        Options options;
        for (usize i = 1; i < argc; ++i) {
            const StringView arg = argv[i];
            if (arg == "--filter" && i + 1 < argc) {
                options.filter = argv[++i];
            } else if (arg == "--tag" && i + 1 < argc) {
                options.tag = argv[++i];
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
     * @brief Prints one status line, optionally colorised.
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
            stdx::io::println(TextStyle().fg(color), "{}  {}", status, line);
        } else {
            System::out.println("{}  {}", status, line);
        }
    }

    /**
     * @internal
     * @brief Runs a single test, catching aborts, skips and stray exceptions.
     * @param test The test to run.
     * @param suite The owning suite (for its hooks).
     * @param options The active options.
     * @param tally The running totals to update.
     */
    inline void run_one(
        const Test& test,
        const Suite& suite,
        const Options& options,
        Tally& tally
    ) {
        Context& ctx = Context::context();
        ctx.begin_test();
        bool skipped = false;
        String skip_reason;
        const u64 start = System::nano_time();
        try {
            if (suite.before_each != nullptr) {
                suite.before_each();
            }
            test.fn();
        } catch (const TestAbort& _) {
            // A require_* assertion already reported the failure.
        } catch (const TestSkipped& s) {
            skipped = true;
            skip_reason = s.what();
        } catch (const Exception& e) {
            ctx.record_error();
            System::err.println("    uncaught exception: {}", e.what());
        } catch (...) {
            ctx.record_error();
            System::err.println("    uncaught unrecognized exception");
        }
        try {
            if (suite.after_each != nullptr) {
                suite.after_each();
            }
        } catch (...) {
            // Teardown failures are ignored.
        }
        const f64 elapsed_ms = static_cast<f64>(System::nano_time() - start) / 1.0e6;
        const String line = stdx::fmt::format(
            "{} ({:.3f} ms, {} assertions)",
            test.name,
            elapsed_ms,
            ctx.test_assertions()
        );
        if (skipped) {
            ++tally.skipped;
            report(
                "SKIP",
                TextStyle::Color::YELLOW,
                skip_reason.empty() ? line : stdx::fmt::format("{} - {}", line, skip_reason),
                options.color
            );
        } else if (ctx.test_failures() == 0) {
            ++tally.passed;
            if (options.verbose) {
                report("PASS", TextStyle::Color::GREEN, line, options.color);
            }
        } else {
            ++tally.failed;
            report("FAIL", TextStyle::Color::RED, line, options.color);
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
    inline int run_impl(int argc, char* argv[], InitializerList<Suite> suites) {
        const Options options = parse_options(argc, argv);
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
        const String summary = stdx::fmt::format(
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
        return run_impl(argc, argv, {Suite{.tests = Vector<Test>(tests)}});
    }

    /**
     * @brief Runs tests passed as separate arguments as an anonymous suite.
     * @param argc The argument count from main.
     * @param argv The argument vector from main.
     * @param tests The tests to run, each given as its own argument.
     * @return 0 if no test failed, 1 otherwise.
     *
     * The variadic form accepts explicitly-typed Test arguments, e.g.
     * run(argc, argv, Test{...}, Test{...}); a brace-enclosed list selects the
     * InitializerList<Test> overload instead.
     */
    int run(int argc, char* argv[], SameAs<Test> auto&&... tests) {
        Vector<Test> list;
        list.reserve(sizeof...(tests));
        (list.push_back(Ops::forward<decltype(tests)>(tests)), ...);
        return run_impl(argc, argv, {Suite{.tests = Ops::move(list)}});
    }

    /**
     * @brief Runs a single suite, honouring its setup and teardown hooks.
     * @param argc The argument count from main.
     * @param argv The argument vector from main.
     * @param suite The suite to run.
     * @return 0 if no test failed, 1 otherwise.
     *
     * Pass an explicit Suite{...} to reach this overload; a brace-enclosed list
     * of {name, fn} entries selects the InitializerList<Test> overload instead.
     */
    int run(int argc, char* argv[], const Suite& suite) {
        return run_impl(argc, argv, {suite});
    }
}
