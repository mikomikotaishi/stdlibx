#pragma once

#ifdef __cpp_impl_reflection
using stdx::collections::Vector;
using stdx::meta::reflect::AccessContext;
using stdx::meta::reflect::Info;

namespace reflect = stdx::meta::reflect;

namespace stdx::test {
    /**
     * @internal
     * @struct Discovered
     * @brief One reflection-discovered test, in a form that can live in static storage.
     *
     * Test holds a Vector and StringViews, so it is not a structural type and cannot
     * be promoted to static storage; the compile-time list is built out of pointers
     * into static storage instead, and turned back into Tests by discover().
     */
    struct Discovered {
        const char* name; ///< The reported test name.
        Test::Callback fn; ///< The test body.
        const char* const* tags; ///< The tags, one string each.
        usize tag_count; ///< The number of tags.
        const char* disabled; ///< The skip reason, or nullptr when the test is enabled.
    };

    /**
     * @internal
     * @struct DiscoveredHooks
     * @brief The setup and teardown hooks found alongside a namespace's tests.
     */
    struct DiscoveredHooks {
        Suite::Callback before_each = nullptr; ///< Run before every selected test.
        Suite::Callback after_each = nullptr; ///< Run after every selected test.
        Suite::Callback before_all = nullptr; ///< Run once before the first selected test.
        Suite::Callback after_all = nullptr; ///< Run once after the last selected test.
    };

    constexpr StringView TEST_PREFIX = "test_"; ///< Name prefix that makes a function a test.

    /**
     * @internal
     * @brief Checks whether an entity carries at least one annotation of a given type.
     * @param entity The reflection of the annotated entity.
     * @param annotation The reflection of the annotation type to look for.
     * @return True if at least one annotation of that type is present.
     */
    [[nodiscard]]
    consteval bool annotated_with(Info entity, Info annotation) {
        return !reflect::annotations_of_with_type(entity, annotation).empty();
    }

    /**
     * @internal
     * @brief Checks whether a function has the shape of a test body or a hook, i.e.
     * whether it is a named function taking no arguments and returning nothing.
     * @param fn The reflection to inspect.
     * @return True if the function can be called through a Test::Callback.
     */
    [[nodiscard]]
    consteval bool is_callback(Info fn) {
        if (!reflect::is_function(fn) || !reflect::has_identifier(fn)) {
            return false;
        }
        // A non-static member function needs an object to run against, so it cannot
        // be reached through the plain function pointer a Test holds.
        if (reflect::is_class_member(fn) && !reflect::is_static_member(fn)) {
            return false;
        }
        return reflect::return_type_of(fn) == (^^void)
            && reflect::parameters_of(fn).empty();
    }

    /**
     * @internal
     * @brief Checks whether a function is one of the suite's hooks, either by
     * annotation or by bearing a hook's name.
     * @param fn The reflection of a candidate function.
     * @return True if the function is a hook rather than a test.
     */
    [[nodiscard]]
    consteval bool is_hook(Info fn) {
        const StringView id = reflect::identifier_of(fn);
        return annotated_with(fn, ^^BeforeEach)
            || annotated_with(fn, ^^AfterEach)
            || annotated_with(fn, ^^BeforeAll)
            || annotated_with(fn, ^^AfterAll)
            || id == "before_each"
            || id == "after_each"
            || id == "before_all"
            || id == "after_all";
    }

    /**
     * @internal
     * @brief Checks whether a namespace member should be registered as a test.
     * @param fn The reflection of the member to consider.
     * @return True for a nullary void function that is either named test_* or
     * annotated with TestCase, and is not a hook.
     */
    [[nodiscard]]
    consteval bool is_test(Info fn) {
        if (!is_callback(fn) || is_hook(fn)) {
            return false;
        }
        return reflect::identifier_of(fn).starts_with(TEST_PREFIX)
            || annotated_with(fn, ^^TestCase);
    }

    /**
     * @internal
     * @brief The address of a nullary void function, as a plain callback pointer.
     * @param fn The reflection of the function.
     * @return A pointer to the function.
     */
    [[nodiscard]]
    consteval Test::Callback callback_of(Info fn) {
        if (reflect::is_noexcept(fn)) {
            return reflect::extract<void(*)() noexcept>(fn);
        }
        return reflect::extract<Test::Callback>(fn);
    }

    /**
     * @internal
     * @brief The name a discovered test is reported under: its DisplayName if it
     * carries one, otherwise the enclosing namespace, a dot, and the function name
     * with its test_ prefix removed.
     * @param fn The reflection of the test function.
     * @param ns The reflection of the enclosing namespace.
     * @return The name, in static storage.
     */
    [[nodiscard]]
    consteval const char* name_of(Info fn, Info ns) {
        const Vector<Info> given = reflect::annotations_of_with_type(fn, ^^DisplayName);
        if (!given.empty()) {
            return reflect::extract<DisplayName>(given.front()).value;
        }
        StringView id = reflect::identifier_of(fn);
        if (id.starts_with(TEST_PREFIX)) {
            id.remove_prefix(TEST_PREFIX.size());
        }
        String name;
        if (reflect::has_identifier(ns)) {
            name += reflect::identifier_of(ns);
            name += '.';
        }
        name += id;
        return reflect::define_static_string(name);
    }

    /**
     * @internal
     * @brief Turns one test function into its compile-time description.
     * @param fn The reflection of the test function.
     * @param ns The reflection of the enclosing namespace.
     * @return The description, referring only to static storage.
     */
    [[nodiscard]]
    consteval Discovered describe(Info fn, Info ns) {
        Vector<const char*> tags;
        for (const Info tag: reflect::annotations_of_with_type(fn, ^^Tag)) {
            tags.emplace_back(reflect::extract<Tag>(tag).value);
        }
        const Span<const char* const> stored = reflect::define_static_array(tags);
        const Vector<Info> disabled = reflect::annotations_of_with_type(fn, ^^Disabled);
        return Discovered {
            .name = name_of(fn, ns),
            .fn = callback_of(fn),
            .tags = stored.data(),
            .tag_count = stored.size(),
            .disabled = disabled.empty()
                ? nullptr
                : reflect::extract<Disabled>(disabled.front()).reason,
        };
    }

    /**
     * @internal
     * @brief Describes every test declared in a namespace, in declaration order.
     * @param ns The reflection of the namespace to scan.
     * @return One description per test.
     *
     * Only members declared before the point this is evaluated are visible, which is
     * why the entry points are templates instantiated from the caller's translation
     * unit rather than functions taking a namespace as an argument.
     */
    [[nodiscard]]
    consteval Vector<Discovered> discovered_in(Info ns) {
        Vector<Discovered> result;
        for (const Info member: reflect::members_of(ns, AccessContext::unchecked())) {
            if (is_test(member)) {
                result.emplace_back(describe(member, ns));
            }
        }
        return result;
    }

    /**
     * @internal
     * @brief Collects a namespace's setup and teardown hooks.
     * @param ns The reflection of the namespace to scan.
     * @return The hooks, each null when the namespace declares none.
     *
     * When several functions claim the same hook the last one declared wins.
     */
    [[nodiscard]]
    consteval DiscoveredHooks hooks_in(Info ns) {
        DiscoveredHooks hooks;
        for (const Info member: reflect::members_of(ns, AccessContext::unchecked())) {
            if (!is_callback(member) || !is_hook(member)) {
                continue;
            }
            const StringView id = reflect::identifier_of(member);
            const Test::Callback fn = callback_of(member);
            if (annotated_with(member, ^^BeforeEach) || id == "before_each") {
                hooks.before_each = fn;
            }
            if (annotated_with(member, ^^AfterEach) || id == "after_each") {
                hooks.after_each = fn;
            }
            if (annotated_with(member, ^^BeforeAll) || id == "before_all") {
                hooks.before_all = fn;
            }
            if (annotated_with(member, ^^AfterAll) || id == "after_all") {
                hooks.after_all = fn;
            }
        }
        return hooks;
    }

    /**
     * @internal
     * @brief A namespace's (or class's) own name, or the empty string for the global
     * namespace.
     * @param ns The reflection of the namespace or class.
     * @return The name, in static storage.
     */
    [[nodiscard]]
    consteval const char* namespace_name(Info ns) {
        if (!reflect::has_identifier(ns)) {
            return "";
        }
        return reflect::define_static_string(reflect::identifier_of(ns));
    }

    /**
     * @internal
     * @brief The tests declared in a namespace, promoted to static storage.
     * @tparam Nsp The reflection of the namespace to scan.
     */
    template <Info Nsp>
        requires ScannableScope<Nsp>
    constexpr Span<const Discovered> DISCOVERED = reflect::define_static_array(discovered_in(Nsp));

    /**
     * @internal
     * @brief The hooks declared in a namespace.
     * @tparam Nsp The reflection of the namespace to scan.
     */
    template <Info Nsp>
        requires ScannableScope<Nsp>
    constexpr DiscoveredHooks HOOKS = hooks_in(Nsp);

    /**
     * @internal
     * @brief Turns compile-time descriptions into the runner's Tests.
     * @param found The descriptions, as produced by discovered_in().
     * @return One Test per description, in the same order.
     */
    [[nodiscard]]
    inline Vector<Test> tests_from(Span<const Discovered> found) {
        Vector<Test> tests;
        tests.reserve(found.size());
        for (const Discovered& test: found) {
            Vector<StringView> tags;
            tags.reserve(test.tag_count);
            for (usize i = 0; i < test.tag_count; ++i) {
                tags.emplace_back(test.tags[i]);
            }
            tests.emplace_back(Test{
                .name = test.name,
                .fn = test.fn,
                .tags = Ops::move(tags),
                .disabled = test.disabled == nullptr
                    ? nullopt
                    : Optional<StringView>(test.disabled),
            });
        }
        return tests;
    }

    /**
     * @internal
     * @brief Assembles a suite from what was discovered in one namespace.
     * @param name The suite name.
     * @param hooks The discovered hooks.
     * @param found The discovered tests.
     * @return The assembled suite.
     */
    [[nodiscard]]
    inline Suite suite_from(
        StringView name,
        const DiscoveredHooks& hooks,
        Span<const Discovered> found
    ) {
        return Suite{
            .name = name,
            .before_each = hooks.before_each,
            .after_each = hooks.after_each,
            .before_all = hooks.before_all,
            .after_all = hooks.after_all,
            .tests = tests_from(found),
        };
    }
}

/**
 * @namespace stdx::test
 * @brief Minimal assertion-based unit-testing framework.
 */
export namespace stdx::test {
    /**
     * @brief Discovers the tests a namespace declares.
     * @tparam Nsp The reflection of the namespace to scan, e.g. ^^tests.
     * @return One Test per discovered function, in declaration order.
     *
     * A namespace member is a test when it takes no arguments, returns nothing, is
     * not a hook, and is either named test_* or annotated with TestCase. Tag,
     * DisplayName and Disabled annotations are carried over.
     *
     * Only the namespace's own members are scanned, not those of the namespaces
     * nested inside it; discover each of those in turn and run them as their own
     * suites.
     *
     * A class reflection works too, e.g. discover<^^Fixture>(): its static member
     * functions are scanned the same way, private ones included, since members are
     * collected in an unchecked access context. Non-static members are skipped -
     * they need an object, which a Test::Callback cannot supply.
     *
     * Only declarations preceding the call are visible, so call this after the tests
     * are defined - from main(), as run() does.
     */
    template <Info Nsp>
        requires ScannableScope<Nsp>
    [[nodiscard]]
    Vector<Test> discover() {
        return tests_from(DISCOVERED<Nsp>);
    }

    /**
     * @brief Discovers a namespace as a suite: its tests plus its hooks.
     * @tparam Nsp The reflection of the namespace or class to scan, e.g. ^^tests.
     * @return A suite named after the namespace.
     *
     * Declared in runner.inl, on the type it produces; defined here, where the
     * machinery it drives lives.
     */
    template <Info Nsp>
        requires ScannableScope<Nsp>
    Suite Suite::of() {
        // Bound to a constant first: an Info is a consteval-only value, so it cannot
        // appear in a run-time expression, not even as an immediate call's argument.
        constexpr const char* name = namespace_name(Nsp);
        return suite_from(name, HOOKS<Nsp>, DISCOVERED<Nsp>);
    }

    /**
     * @brief Discovers a namespace and runs it as a single suite.
     * @tparam Nsp The reflection of the namespace to scan, e.g. ^^tests.
     * @param argc The argument count from main.
     * @param argv The argument vector from main.
     * @return 0 if no test failed, 1 otherwise.
     *
     * The whole main() of a test program:
     * @code
     * int main(int argc, char* argv[]) {
     *     return stdx::test::run<^^tests>(argc, argv);
     * }
     * @endcode
     */
    template <Info Nsp>
        requires ScannableScope<Nsp>
    int run(int argc, char* argv[]) {
        constexpr const char* name = namespace_name(Nsp);
        return run(argc, argv, suite_from(name, HOOKS<Nsp>, DISCOVERED<Nsp>));
    }

    /**
     * @brief Discovers and runs the tests in a namespace, taking the command line
     * from the process.
     * @tparam Nsp The reflection of the namespace to scan, e.g. ^^tests.
     * @return 0 if no test failed, 1 otherwise.
     *
     * @code
     * int main() {
     *     return stdx::test::run<^^tests>();
     * }
     * @endcode
     */
    template <Info Nsp>
        requires ScannableScope<Nsp>
    int run() {
        constexpr const char* name = namespace_name(Nsp);
        return run(suite_from(name, HOOKS<Nsp>, DISCOVERED<Nsp>));
    }
}
#endif
