#pragma once

/**
 * @namespace stdx::inject
 * @brief Annotation-driven dependency injection.
 */
export namespace stdx::inject {

    /**
     * @class InjectionException
     * @brief Base exception for all dependency injection errors.
     * @extends Exception
     */
    class InjectionException: public Exception {
    private:
        String msg; ///< Human-readable error message.
    public:
        explicit InjectionException(const String& msg = ""):
            Exception(), msg{msg} {}

        [[nodiscard]]
        const char* what() const noexcept override {
            return msg.c_str();
        }
    };

    /**
     * @class NoBindingException
     * @brief Thrown when no binding exists for a requested type.
     * @extends InjectionException
     */
    class NoBindingException: public InjectionException {
    public:
        using InjectionException::InjectionException;
    };

    /**
     * @class SingletonAccessException
     * @brief Thrown when a singleton binding is requested by value.
     *
     * Singleton instances are owned by the injector; request them as a
     * reference or as a {@code SharedPointer}.
     *
     * @extends InjectionException
     */
    class SingletonAccessException: public InjectionException {
    public:
        using InjectionException::InjectionException;
    };

    /**
     * @struct Qualifier
     * @brief Meta-annotation marking an annotation type as a binding qualifier.
     *
     * Apply as {@code [[=Qualifier()]]} on the annotation type itself. Only
     * qualifier-marked annotations on a constructor parameter participate in
     * binding selection; all other annotations are ignored by the injector.
     */
    struct Qualifier {};

    /**
     * @struct Scope
     * @brief Meta-annotation marking an annotation type as a scope.
     *
     * Apply as {@code [[=Scope()]]} on the annotation type itself. The only
     * scope the injector currently implements is {@code Singleton}; a class
     * annotated with any other scope-marked annotation is rejected at
     * compile time.
     */
    struct Scope {};

    /**
     * @struct Inject
     * @brief Annotation type marking the constructor the injector should use.
     *
     * Apply as {@code [[=Inject()]]} on exactly one constructor.
     */
    struct Inject {};

    /**
     * @struct Singleton
     * @brief Scope annotation marking a class as singleton-scoped.
     *
     * Apply as {@code [[=Singleton()]]} on the class. The injector creates one
     * shared instance on first resolution and returns it thereafter.
     */
    #ifdef __cpp_impl_reflection
    struct [[=Scope()]] Singleton {};
    #else
    struct Singleton {};
    #endif

    #ifdef __cpp_impl_reflection
    /**
     * @struct Named
     * @brief String-valued qualifier for distinguishing bindings of the same type.
     *
     * Apply to constructor parameters as {@code [[=Named("key")]]} and bind with
     * {@code binder.bind<T>().annotated_with(Named("key"))}.
     */
    struct [[=Qualifier()]] Named {
        const char* value; ///< The annotation string, in static storage.

        consteval Named(StringView name):
            value{Ops::define_static_string(name)} {}
    };
    #endif
}

#ifdef __cpp_impl_reflection
using stdx::inject::Named;

namespace stdx::core {
    template <>
    struct Hash<Named> {
        [[nodiscard]]
        usize operator()(const Named& named) const noexcept {
            return Hash<StringView>{}(named.value);
        }
    };
}

template <>
struct stdx::core::hash<Named> : public Hash<Named> {};
#endif
