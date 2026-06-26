#pragma once

#ifdef __cpp_impl_reflection
using stdx::collections::HashMap;
using stdx::collections::Vector;
using stdx::mem::Pointers;
using stdx::mem::SharedPointer;
using stdx::meta::IsLvalueReferenceValue;
using stdx::meta::RemoveReferenceType;
using stdx::meta::TypeIndex;
using stdx::meta::reflect::Annotation;
using stdx::meta::reflect::Class;
using stdx::meta::reflect::Constructor;
using stdx::meta::reflect::Info;
using stdx::meta::reflect::Parameter;
using stdx::meta::reflect::ReflectiveOperationException;

namespace reflect = stdx::meta::reflect;

namespace stdx::inject {
    export class Configuration;
    export template <typename T> class Provider;

    template <typename T>
    constexpr bool IsProviderValue = false;

    template <typename U>
    constexpr bool IsProviderValue<Provider<U>> = true;

    template <typename T>
    struct ProviderElement {};

    template <typename U>
    struct ProviderElement<Provider<U>> {
        using Element = U;
    };

    /**
     * @class Injector
     * @brief Resolves instances from a set of bindings.
     *
     * Created from a {@code Configuration} via {@code Injector::create()}. Request
     * instances with {@code get<T>()}, where {@code T} may be a value type,
     * an lvalue reference (required for singleton class bindings), or a
     * {@code SharedPointer<U>}.
     */
    export class Injector {
    public:
        /**
         * @brief A binding's factory: produces a type-erased instance on demand.
         */
        using Factory = Function<Any(Injector&, const Optional<AnnotationKey>&)>;

        /**
         * @struct Binding
         * @brief A registered binding: its factory, scope, and cached instance.
         */
        struct Binding {
            Factory factory; ///< Creates the instance.
            bool singleton; ///< Whether the instance is cached after first creation.
            SharedPointer<void> instance; ///< The cached singleton instance, if created.
        };
    private:
        HashMap<BindingKey, Binding, BindingKeyHash> bindings;

        /**
        * @brief Finds the best binding for a type: exact annotation match first,
        * then a TYPE_ONLY fallback for the annotation's type, then unannotated.
        */
        [[nodiscard]]
        Binding* find_binding(TypeIndex type, const Optional<AnnotationKey>& annotation) {
            auto lookup = [&](const Optional<AnnotationKey>& key) -> Binding* {
                auto it = bindings.find(BindingKey {
                    .type = type,
                    .annotation = key
                });
                return it != bindings.end() ? &it->second : nullptr;
            };

            Binding* binding = lookup(annotation);
            if (!binding && annotation.has_value()) {
                binding = lookup(Optional<AnnotationKey>(AnnotationKey(annotation->type())));
            }
            if (!binding) {
                binding = lookup(nullopt);
            }
            return binding;
        }
    public:
        /**
         * @brief Resolves an instance of {@code T}.
         *
         * Singleton class bindings must be requested as {@code T&} or
         * {@code SharedPointer<T>}; non-singleton value bindings are returned
         * by value.
         *
         * @tparam T The requested type: a value, lvalue reference, or SharedPointer.
         * @param annotation Optional annotation to select between bindings.
         * @return The resolved instance.
         * @throws NoBindingException if no binding matches.
         * @throws SingletonAccessException if a singleton is requested by value.
         */
        template <typename T>
        [[nodiscard]]
        [[=Throws<NoBindingException, SingletonAccessException>()]]
        T get(Optional<AnnotationKey> annotation = nullopt) {
            if constexpr (IsProviderValue<T>) {
                // A Provider<U> is satisfied without a binding lookup: it
                // captures the injection site's annotation and resolves U
                // lazily on each Provider::get().
                return T(*this, Ops::move(annotation));
            } else {
                using Plain = RemoveReferenceType<T>;

                Binding* binding = find_binding(TypeIndex(typeid(T)), annotation);
                if (!binding) {
                    throw NoBindingException(stdx::fmt::format("No binding for type {}", typeid(T).name()));
                }

                if (binding->singleton) {
                    if (!binding->instance) {
                        binding->instance = Ops::any_cast<SharedPointer<void>>(binding->factory(*this, annotation));
                    }
                    if constexpr (IsSharedPointerValue<T>) {
                        return Pointers::static_pointer_cast<typename SharedPointerElement<T>::Element>(binding->instance);
                    } else if constexpr (IsLvalueReferenceValue<T>) {
                        return *static_cast<Plain*>(binding->instance.get());
                    } else {
                        throw SingletonAccessException(
                            stdx::fmt::format("Singleton {} must be requested by reference or SharedPointer", typeid(T).name())
                        );
                    }
                }

                Any result = binding->factory(*this, annotation);
                if constexpr (IsSharedPointerValue<T>) {
                    // Class bindings store instances type-erased as SharedPointer<void>;
                    // provider/instance bindings store the SharedPointer<U> directly.
                    if (SharedPointer<void>* erased = Ops::any_cast<SharedPointer<void>>(&result)) {
                        return Pointers::static_pointer_cast<typename SharedPointerElement<T>::Element>(*erased);
                    }
                    return Ops::any_cast<T>(Ops::move(result));
                } else if constexpr (IsLvalueReferenceValue<T>) {
                    return *Ops::any_cast<Plain*>(result);
                } else {
                    return Ops::any_cast<T>(Ops::move(result));
                }
            }
        }

        /**
         * @brief Registers a binding for {@code T}.
         *
         * Prefer configuring bindings through a {@code Configuration}; this is the
         * low-level registration used by the binding builders.
         *
         * @param factory Produces the instance.
         * @param annotation Optional annotation distinguishing this binding.
         * @param singleton Whether the produced instance is cached and shared.
         */
        template <typename T>
        void bind(Factory factory, Optional<AnnotationKey> annotation = nullopt, bool singleton = true) {
            BindingKey key {
                .type = TypeIndex(typeid(T)),
                .annotation = Ops::move(annotation)
            };
            bindings[Ops::move(key)] = Binding {
                .factory = Ops::move(factory),
                .singleton = singleton,
                .instance = nullptr
            };
        }

        /**
         * @brief Creates an injector configured by {@code config}.
         * @param config The configuration whose {@code configure()} registers the bindings.
         * @return The configured injector.
         */
        [[nodiscard]]
        static Injector create(Configuration& config);
    };

    /**
     * @class Provider
     * @brief Lazily resolves a fresh {@code T} from the injector on each {@code get()}.
     *
     * Request a {@code Provider<T>} (at an injection site or via
     * {@code Injector::get<Provider<T>>()}) instead of {@code T} to defer
     * resolution to the point of use. Qualifier annotations at the injection
     * site are captured and applied to every {@code get()}.
     *
     * The provider holds a pointer to the injector and must not outlive it.
     */
    export template <typename T>
    class Provider {
    private:
        Injector* injector; ///< The injector to resolve from.
        Optional<AnnotationKey> annotation; ///< Captured injection-site qualifier.
    public:
        explicit Provider(Injector& injector, Optional<AnnotationKey> annotation = nullopt):
            injector{&injector}, annotation{Ops::move(annotation)} {}

        /**
         * @brief Resolves and returns an instance of {@code T}.
         * @throws NoBindingException if no binding matches.
         * @throws SingletonAccessException if a singleton is requested by value.
         */
        [[nodiscard]]
        [[=Throws<NoBindingException, SingletonAccessException>()]]
        T get() const {
            return injector->get<T>(annotation);
        }
    };

    /**
     * @internal
     * @brief The qualifier annotation on {@code parameter}, if any.
     *
     * Only annotations whose type is marked {@code [[=Qualifier()]]} are
     * considered; all other annotations on the parameter are ignored.
     *
     * @throws ReflectiveOperationException if more than one qualifier is present.
     */
    consteval Optional<Annotation> find_qualifier(const Parameter& parameter) {
        Optional<Annotation> found;
        for (Annotation annotation: parameter.annotations()) {
            if (reflect::annotations_of_with_type(reflect::remove_cv(annotation.type()), ^^Qualifier).empty()) {
                continue;
            }
            if (found.has_value()) {
                throw ReflectiveOperationException("Multiple qualifier annotations on parameter", parameter);
            }
            // Annotation's const Info member deletes assignment; construct in place.
            found.emplace(annotation);
        }
        return found;
    }

    /**
     * @internal
     * @brief Whether {@code T} is singleton-scoped.
     *
     * Inspects {@code T}'s annotations for one whose type is marked
     * {@code [[=Scope()]]}. {@code Singleton} is the only scope the injector
     * implements; any other scope-marked annotation is rejected.
     *
     * @throws ReflectiveOperationException on multiple or unsupported scopes.
     */
    template <typename T>
    consteval bool is_singleton_scoped() {
        bool found = false;
        for (Annotation annotation: Class<T>().annotations()) {
            if (reflect::annotations_of_with_type(reflect::remove_cv(annotation.type()), ^^Scope).empty()) {
                continue;
            }
            if (found) {
                throw ReflectiveOperationException("Multiple scope annotations on class", ^^T);
            }
            found = true;
            if (reflect::remove_cv(annotation.type()) != ^^Singleton) {
                throw ReflectiveOperationException("Unsupported scope annotation", ^^T);
            }
        }
        return found;
    }

    /**
     * @internal
     * @brief Finds the constructor of {@code T} marked {@code [[=Inject()]]},
     * falling back to the default constructor.
     */
    template <typename T>
    consteval Constructor find_inject_constructor() {
        for (Constructor ctor: Class<T>().constructors()) {
            if (!ctor.annotations_with_type<Inject>().empty()) {
                return ctor;
            }
        }

        for (Constructor ctor: Class<T>().constructors()) {
            if (ctor.is_default()) {
                return ctor;
            }
        }

        throw ReflectiveOperationException("No [[=Inject()]] or default constructor found", ^^T);
    }

    /**
     * @internal
     * @brief Resolves each constructor parameter from the injector and
     * constructs a {@code T}, type-erased as {@code SharedPointer<void>}.
     */
    template <typename T, typename... Deps>
    struct ConstructorHelper {
        template <usize... Is>
        static Any make_impl(
            Injector& injector,
            const Optional<AnnotationKey>* annotations,
            IndexSequence<Is...>
        ) {
            return Any(Pointers::static_pointer_cast<void>(
                Pointers::shared<T>(injector.get<Deps...[Is]>(annotations[Is])...)
            ));
        }

        static Any make(Injector& injector, const Optional<AnnotationKey>* annotations) {
            return make_impl(injector, annotations, IndexSequenceFor<Deps...>{});
        }
    };

    /**
     * @internal
     * @brief Builds a factory for {@code T} using constructor {@code Ctor}:
     * captures each parameter's annotation (if any) and specialises
     * {@code ConstructorHelper} on the parameter types.
     */
    template <typename T, Constructor Ctor>
    [[nodiscard]]
    Injector::Factory make_factory_for() {
        constexpr Info helper_specialised = [] consteval -> Info {
            Vector<Info> args = { ^^T };
            for (Parameter p: Ctor.parameters()) {
                args.push_back(p.type());
            }
            return reflect::substitute(^^ConstructorHelper, args);
        }();
        using Helper = [:helper_specialised:];

        constexpr usize N = [] consteval -> usize {
            return Ctor.parameters().size();
        }();

        Array<Optional<AnnotationKey>, N> annotations;
        usize i = 0;
        template for (constexpr Parameter p: Ops::define_static_array(Ctor.parameters())) {
            constexpr bool has_qualifier = find_qualifier(p).has_value();
            if constexpr (has_qualifier) {
                // Rebuild the wrapper from its scalar Info: copying the wrapper
                // straight out of the transient Optional is not a constant expression.
                constexpr Annotation annotation = Annotation(find_qualifier(p)->value());
                using AnnotationType = [:reflect::remove_cv(annotation.type()):];
                constexpr AnnotationType value = annotation.as<AnnotationType>();
                annotations[i] = AnnotationKey(value);
            }
            ++i;
        }

        return [annotations = Ops::move(annotations)](Injector& injector, const Optional<AnnotationKey>&) -> Any {
            return Helper::make(injector, annotations.data());
        };
    }

    /**
     * @class AnnotatedBindingBuilder
     * @brief Completes a binding that is qualified by an annotation.
     */
    export template <typename T, Annotatable A>
    class AnnotatedBindingBuilder {
    private:
        Injector& injector;
        AnnotationKey annotation;

        void finalize(Injector::Factory factory, bool singleton) {
            injector.bind<T>(Ops::move(factory), Ops::move(annotation), singleton);
        }
    public:
        explicit AnnotatedBindingBuilder(Injector& injector, AnnotationKey annotation):
            injector{injector}, annotation{Ops::move(annotation)} {}

        AnnotatedBindingBuilder(const AnnotatedBindingBuilder&) = delete("AnnotatedBindingBuilder is not copyable.");
        AnnotatedBindingBuilder(AnnotatedBindingBuilder&&) = default;

        /**
        * @brief Binds to a fixed value.
        * @param value The instance to return for every resolution.
        */
        void to_instance(T value) {
            finalize([value = Ops::move(value)](Injector&, const Optional<AnnotationKey>&) -> Any {
                return Any(value);
            }, false);
        }

        /**
        * @brief Binds to a provider callable invoked on each resolution.
        *
        * Accepts {@code () -> T}, {@code (const A&) -> T}, {@code (Injector&) -> T},
        * or {@code (Injector&, const A&) -> T}; providers taking the annotation
        * receive the value the resolution site was annotated with, enabling one
        * provider to serve all values of an annotation type.
        */
        template <typename F>
        void to_provider(F&& f) {
            if constexpr (Invocable<F, const A&>) {
                finalize([f = Ops::forward<F>(f)](Injector&, const Optional<AnnotationKey>& annotation) -> Any {
                    return Any(f(Ops::any_cast<const A&>(annotation->value())));
                }, false);
            } else if constexpr (Invocable<F, Injector&>) {
                finalize([f = Ops::forward<F>(f)](Injector& injector, const Optional<AnnotationKey>&) -> Any {
                    return Any(f(injector));
                }, false);
            } else if constexpr (Invocable<F, Injector&, const A&>) {
                finalize([f = Ops::forward<F>(f)](Injector& injector, const Optional<AnnotationKey>& annotation) -> Any {
                    return Any(f(injector, Ops::any_cast<const A&>(annotation->value())));
                }, false);
            } else {
                static_assert(
                    Invocable<F>,
                    "to_provider requires () -> T, (const A&) -> T, (Injector&) -> T, or (Injector&, const A&) -> T"
                );
                finalize([f = Ops::forward<F>(f)](Injector&, const Optional<AnnotationKey>&) -> Any {
                    return Any(f());
                }, false);
            }
        }
    };

    /**
     * @class BindingBuilder
     * @brief Fluent builder for registering a binding of type {@code T}.
     */
    export template <typename T>
    class BindingBuilder {
    private:
        Injector& injector;

        void finalize(Injector::Factory factory, bool singleton) {
            injector.bind<T>(Ops::move(factory), nullopt, singleton);
        }
    public:
        explicit BindingBuilder(Injector& injector):
            injector{injector} {}

        BindingBuilder(const BindingBuilder&) = delete("BindingBuilder is not copyable.");
        BindingBuilder(BindingBuilder&&) = default;

        /**
        * @brief Qualifies the binding to serve any value of annotation type {@code A}.
        */
        template <Annotatable A>
        [[nodiscard]]
        AnnotatedBindingBuilder<T, A> annotated_with_type() {
            return AnnotatedBindingBuilder<T, A>(injector, AnnotationKey::type_only<A>());
        }

        /**
        * @brief Qualifies the binding with a specific annotation value.
        * @param annotation The annotation value (e.g. {@code Named("db")}).
        */
        template <Annotatable A>
        [[nodiscard]]
        AnnotatedBindingBuilder<T, A> annotated_with(A annotation) {
            return AnnotatedBindingBuilder<T, A>(injector, AnnotationKey(Ops::move(annotation)));
        }

        /**
        * @brief Binds to a fixed value.
        * @param value The instance to return for every resolution.
        */
        void to_instance(T value) {
            finalize([value = Ops::move(value)](Injector&, const Optional<AnnotationKey>&) -> Any {
                return Any(value);
            }, false);
        }

        /**
        * @brief Binds to a provider callable invoked on each resolution.
        *
        * Accepts {@code () -> T} or {@code (Injector&) -> T}.
        */
        template <typename F>
        void to_provider(F&& f) {
            if constexpr (Invocable<F, Injector&>) {
                finalize([f = Ops::forward<F>(f)](Injector& injector, const Optional<AnnotationKey>&) -> Any {
                    return Any(f(injector));
                }, false);
            } else {
                static_assert(Invocable<F>, "to_provider requires () -> T or (Injector&) -> T");
                finalize([f = Ops::forward<F>(f)](Injector&, const Optional<AnnotationKey>&) -> Any {
                    return Any(f());
                }, false);
            }
        }
    };

    /**
     * @class Binder
     * @brief The binding registry handed to {@code Configuration::configure()}.
     */
    export class Binder {
    private:
        Injector& injector;
    public:
        explicit Binder(Injector& injector):
            injector{injector} {}

        /**
        * @brief Starts a fluent binding for type {@code T}.
        */
        template <typename T>
        [[nodiscard]]
        BindingBuilder<T> bind() {
            return BindingBuilder<T>(injector);
        }

        /**
        * @brief Registers {@code T} for constructor injection.
        *
        * Reflects over {@code T}'s {@code [[=Inject()]]} constructor (or default
        * constructor) and registers a factory that resolves each parameter.
        * If {@code T} is marked {@code [[=Singleton()]]}, one shared instance is
        * created and cached.
        */
        template <typename T>
        void add_binding() {
            constexpr Constructor ctor = find_inject_constructor<T>();
            constexpr bool singleton = is_singleton_scoped<T>();
            injector.bind<T>(make_factory_for<T, ctor>(), nullopt, singleton);
        }
    };

    /**
     * @class Configuration
     * @brief Base class for binding configuration.
     *
     * Override {@code configure()} to register bindings, then create an
     * injector with {@code Injector::create()}.
     */
    export class Configuration {
    public:
        Configuration() = default;
        virtual ~Configuration() = default;

        /**
        * @brief Registers this module's bindings.
        * @param binder The registry to bind into.
        */
        virtual void configure(Binder& binder) = 0;
    };

    inline Injector Injector::create(Configuration& config) {
        Injector injector;
        Binder binder(injector);
        config.configure(binder);
        return injector;
    }

}

#endif