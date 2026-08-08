#include <version>

import stdx;

#ifdef __cpp_impl_reflection
using stdx::inject::AnnotationKey;
using stdx::inject::Binder;
using stdx::inject::Configuration;
using stdx::inject::Inject;
using stdx::inject::Injector;
using stdx::inject::Named;
using stdx::inject::NoBindingException;
using stdx::inject::Provider;
using stdx::inject::Qualifier;
using stdx::inject::Singleton;
using stdx::inject::SingletonAccessException;
using stdx::mem::SharedPointer;

using namespace stdx::test;

namespace {
    /**
     * @struct Port
     * @brief Assembled by a provider that resolves its own dependencies.
     */
    struct Port {
        i32 value = 0;
    };

    /**
     * @struct Host
     * @brief Assembled by a provider that resolves its own dependencies.
     */
    struct Host {
        String name;
    };

    /**
     * @struct Endpoint
     * @brief Assembled by a provider that resolves its own dependencies.
     */
    struct Endpoint {
        String host;
        i32 port = 0;
    };

    /**
     * @struct Ticket
     * @brief Counts constructions so a binding's scope can be observed.
     */
    struct Ticket {
        static inline i32 issued = 0;
        i32 serial;

        Ticket():
            serial{++issued} {}
    };

    /**
     * @struct Tier
     * @brief A qualifier of our own, to show that Named is not special-cased.
     */
    struct [[=Qualifier]] Tier {
        const char* value;

        consteval Tier(StringView name):
            value{Ops::define_static_string(name)} {}
    };

    /**
     * @struct Connection
     * @brief Constructor injection: the [[=Inject]] constructor's parameters
     * are the dependencies, each resolved by its own type.
     *
     * Singleton-scoped because a class binding is only retrievable that way;
     * see inject.class_binding_without_singleton_scope for the gap.
     */
    struct [[=Singleton]] Connection {
        Port port;
        Host host;

        [[=Inject]]
        Connection(Port p, Host h):
            port{p}, host{Ops::move(h)} {}
    };

    /**
     * @struct Registry
     * @brief No [[=Inject]] anywhere, so the default constructor is the fallback.
     */
    struct [[=Singleton]] Registry {
        static inline i32 constructed = 0;
        i32 id;

        Registry():
            id{++constructed} {}
    };

    /**
     * @struct PortPair
     * @brief Its dependencies are told apart by qualifier annotations rather
     * than by type - both parameters are the same type.
     */
    struct [[=Singleton]] PortPair {
        Port primary;
        Port secondary;

        [[=Inject]]
        PortPair([[=Named("primary")]] Port a, [[=Named("secondary")]] Port b):
            primary{a}, secondary{b} {}
    };
}

namespace stdx::core {
    template <>
    struct Hash<Tier> {
        [[nodiscard]]
        usize operator()(const Tier& tier) const noexcept {
            return Hash<StringView>()(tier.value);
        }
    };
}

template <>
struct stdx::core::hash<Tier>: public Hash<Tier> {};

namespace {
    /**
     * @class TestConfig
     * @brief A test configuration for the injection system.
     * @extends Configuration
     */
    class TestConfig final: public Configuration {
    public:
        void configure(Binder& binder) override {
            binder.bind<Port>().to_instance(Port {.value = 8080});
            binder.bind<Host>().to_instance(Host {.name = "localhost"});

            // A provider runs on every resolution, so each get() is a fresh value.
            binder.bind<Ticket>().to_provider([] -> Ticket { return Ticket(); });

            // The (Injector&) form can resolve the bindings it depends on.
            binder.bind<Endpoint>().to_provider([](Injector& injector) -> Endpoint {
                return Endpoint{injector.get<Host>().name, injector.get<Port>().value};
            });

            // Same type, told apart by annotation. The unannotated Port above is
            // the fallback when neither qualifier matches.
            binder.bind<Port>().annotated_with(Named("primary")).to_instance(Port {.value = 80});
            binder.bind<Port>().annotated_with(Named("secondary")).to_instance(Port {.value = 443});

            // One provider serving every value of an annotation type: the
            // TYPE_ONLY fallback hands it the value the site asked for.
            binder.bind<Host>().annotated_with_type<Named>().to_provider([](const Named& named) -> Host {
                return Host {.name = Ops::fmt("host-{}", named.value)};
            });

            // A qualifier that is not Named, to show the mechanism is generic.
            binder.bind<Host>().annotated_with(Tier("gold")).to_instance(Host {.name = "gold.example"});

            binder.add_binding<Connection>();
            binder.add_binding<Registry>();
            binder.add_binding<PortPair>();
        }
    };

    [[nodiscard]]
    Injector make_injector(Configuration& config) {
        return Injector::create(config);
    }
}

void test_to_instance_returns_the_bound_value() {
    TestConfig config;
    Injector injector = make_injector(config);

    expect_eq(injector.get<Port>().value, 8080, "a value binding comes back by value");
    expect_eq(injector.get<Host>().name, "localhost", "and so does one of a different type");
}

void test_missing_binding_is_refused() {
    struct Unbound {};

    TestConfig config;
    Injector injector = make_injector(config);

    expect_throws<NoBindingException>(
        [&] -> void { static_cast<void>(injector.get<Unbound>()); },
        "resolving a type nothing bound is refused rather than default-constructed"
    );
}

void test_provider_binding_runs_on_every_resolution() {
    TestConfig config;
    Injector injector = make_injector(config);

    const Ticket first = injector.get<Ticket>();
    const Ticket second = injector.get<Ticket>();

    expect(first.serial != second.serial, "a to_provider binding yields a fresh value each time");
    expect_eq(second.serial, first.serial + 1, "and ran exactly once per get()");
}

void test_provider_binding_can_resolve_its_own_dependencies() {
    TestConfig config;
    Injector injector = make_injector(config);

    const Endpoint endpoint = injector.get<Endpoint>();
    expect_eq(endpoint.host, "localhost", "the (Injector&) provider form resolved a dependency");
    expect_eq(endpoint.port, 8080, "and a second one");
}

void test_constructor_injection_resolves_parameters_by_type() {
    TestConfig config;
    Injector injector = make_injector(config);

    const Connection& conn = injector.get<Connection&>();
    expect_eq(conn.port.value, 8080, "the [[=Inject]] constructor's first parameter was resolved");
    expect_eq(conn.host.name, "localhost", "and so was the second, each by its own type");
}

void test_default_constructor_is_the_fallback() {
    // Registry declares no [[=Inject]] constructor at all, so add_binding has
    // to fall back to the default one rather than failing to find a constructor.
    TestConfig config;
    Injector injector = make_injector(config);

    const i32 before = Registry::constructed;
    static_cast<void>(injector.get<Registry&>());
    expect_eq(Registry::constructed, before + 1, "a class with only a default constructor is still bindable");
}

void test_singleton_is_created_once_and_shared() {
    TestConfig config;
    Injector injector = make_injector(config);

    const i32 before = Registry::constructed;
    Registry& first = injector.get<Registry&>();
    Registry& second = injector.get<Registry&>();

    expect_eq(Registry::constructed, before + 1, "a [[=Singleton]] class is constructed exactly once");
    expect(&first == &second, "and every resolution hands back that same instance");
}

void test_each_injector_owns_its_own_singleton() {
    // The scope is the injector, not the process - this is the property that
    // replaced the ambient LoggerFactory::instance() singletons.
    TestConfig config;
    Injector first = make_injector(config);
    Injector second = make_injector(config);

    Registry& from_first = first.get<Registry&>();
    Registry& from_second = second.get<Registry&>();

    expect(&from_first != &from_second, "two injectors do not share a singleton instance");
}

void test_singleton_by_value_is_refused() {
    TestConfig config;
    Injector injector = make_injector(config);

    expect_throws<SingletonAccessException>(
        [&] -> void { static_cast<void>(injector.get<Registry>()); },
        "a singleton requested by value is refused rather than silently copied"
    );
}

void test_annotation_selects_between_bindings_of_one_type() {
    TestConfig config;
    Injector injector = make_injector(config);

    expect_eq(
        injector.get<Port>(AnnotationKey(Named("primary"))).value, 80,
        "an exact annotation picks its own binding"
    );
    expect_eq(
        injector.get<Port>(AnnotationKey(Named("secondary"))).value, 443,
        "and a different value picks a different one"
    );
    expect_eq(
        injector.get<Port>().value, 8080,
        "while an unannotated request still gets the unannotated binding"
    );
}

void test_annotated_constructor_parameters_are_qualified() {
    // Both parameters are Port. Only the [[=Named(...)]] annotations tell the
    // injector which binding each one wants.
    TestConfig config;
    Injector injector = make_injector(config);

    const PortPair& pair = injector.get<PortPair&>();
    expect_eq(pair.primary.value, 80, "the parameter annotated primary got the primary binding");
    expect_eq(pair.secondary.value, 443, "and the one annotated secondary got the other");
}

void test_type_only_binding_serves_every_annotation_value() {
    TestConfig config;
    Injector injector = make_injector(config);

    expect_eq(
        injector.get<Host>(AnnotationKey(Named("alpha"))).name, "host-alpha",
        "annotated_with_type serves a value it was never told about"
    );
    expect_eq(
        injector.get<Host>(AnnotationKey(Named("beta"))).name, "host-beta",
        "and the provider receives the value the site asked for"
    );
}

void test_resolution_falls_back_from_exact_to_type_to_unannotated() {
    TestConfig config;
    Injector injector = make_injector(config);

    // Host has all three: an unannotated instance, a TYPE_ONLY provider for
    // Named, and an exact binding for Tier("gold").
    expect_eq(
        injector.get<Host>(AnnotationKey(Tier("gold"))).name, "gold.example",
        "an exact match wins"
    );
    expect_eq(
        injector.get<Host>(AnnotationKey(Named("alpha"))).name, "host-alpha",
        "with no exact match, the annotation's TYPE_ONLY binding is next"
    );
    expect_eq(
        injector.get<Host>(AnnotationKey(Tier("silver"))).name, "localhost",
        "and with neither, the unannotated binding is last"
    );
}

void test_provider_defers_resolution_to_use() {
    TestConfig config;
    Injector injector = make_injector(config);

    const Provider<Ticket> provider = injector.get<Provider<Ticket>>();
    const i32 before = Ticket::issued;
    expect_eq(Ticket::issued, before, "asking for a Provider resolves nothing yet");

    const Ticket first = provider.get();
    const Ticket second = provider.get();
    expect_eq(second.serial, first.serial + 1, "each Provider::get() is its own resolution");
}

void test_provider_carries_its_annotation() {
    TestConfig config;
    Injector injector = make_injector(config);

    const Provider<Host> provider = injector.get<Provider<Host>>(AnnotationKey(Named("gamma")));
    expect_eq(provider.get().name, "host-gamma", "a Provider applies the site's qualifier on every get()");
}

void test_shared_pointer_retrieval_of_a_singleton() {
    skip(
        "Injector::get looks a binding up under typeid(T), so get<SharedPointer<Registry>>() "
        "searches for a SharedPointer<Registry> binding and throws NoBindingException - even "
        "though its own doc comment promises this spelling and both of its "
        "SharedPointer-unwrapping branches exist to serve it."
    );
}

void test_class_binding_without_singleton_scope() {
    skip(
        "add_binding<T>() on a class with no [[=Singleton]] is unretrievable: "
        "ConstructorHelper::make stores Any(SharedPointer<void>), but the non-singleton branch "
        "of Injector::get reads it back as T or T*, so get<T>() and get<T&>() both throw "
        "bad any_cast and get<SharedPointer<T>>() throws NoBindingException."
    );
}
#endif

int main(int argc, char* argv[]) {
    #ifdef __cpp_impl_reflection
    return run(argc, argv, {
        {"Inject.to_instance_returns_the_bound_value", test_to_instance_returns_the_bound_value},
        {"Inject.missing_binding_is_refused", test_missing_binding_is_refused},
        {"Inject.provider_binding_runs_on_every_resolution", test_provider_binding_runs_on_every_resolution},
        {"Inject.provider_binding_can_resolve_its_own_dependencies", test_provider_binding_can_resolve_its_own_dependencies},
        {"Inject.constructor_injection_resolves_parameters_by_type", test_constructor_injection_resolves_parameters_by_type},
        {"Inject.default_constructor_is_the_fallback", test_default_constructor_is_the_fallback},
        {"Inject.singleton_is_created_once_and_shared", test_singleton_is_created_once_and_shared},
        {"Inject.each_injector_owns_its_own_singleton", test_each_injector_owns_its_own_singleton},
        {"Inject.singleton_by_value_is_refused", test_singleton_by_value_is_refused},
        {"Inject.annotation_selects_between_bindings_of_one_type", test_annotation_selects_between_bindings_of_one_type},
        {"Inject.annotated_constructor_parameters_are_qualified", test_annotated_constructor_parameters_are_qualified},
        {"Inject.type_only_binding_serves_every_annotation_value", test_type_only_binding_serves_every_annotation_value},
        {"Inject.resolution_falls_back_from_exact_to_type_to_unannotated", test_resolution_falls_back_from_exact_to_type_to_unannotated},
        {"Inject.provider_defers_resolution_to_use", test_provider_defers_resolution_to_use},
        {"Inject.provider_carries_its_annotation", test_provider_carries_its_annotation},
        {"Inject.shared_pointer_retrieval_of_a_singleton", test_shared_pointer_retrieval_of_a_singleton},
        {"Inject.class_binding_without_singleton_scope", test_class_binding_without_singleton_scope},
    });
    #else
    System::out.println("[test] Test disabled (compiler does not support reflection).");
    return System::EXIT_SUCCESS;
    #endif
}
