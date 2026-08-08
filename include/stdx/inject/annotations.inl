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
        String _msg; ///< Human-readable error message.
    public:
        explicit InjectionException(const String& msg = ""):
            Exception(), _msg{msg} {}

        [[nodiscard]]
        const char* what() const noexcept override {
            return _msg.c_str();
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
     * @struct QualifierAnnotation
     * @brief Meta-annotation marking an annotation type as a binding qualifier.
     *
     * Spell it {@code [[=Qualifier]]} on the annotation type itself; this is the
     * type behind that object, named apart from it the way C# pairs
     * {@code ObsoleteAttribute} with {@code [Obsolete]}. Only qualifier-marked
     * annotations on a constructor parameter participate in binding selection;
     * all other annotations are ignored by the injector.
     */
    struct QualifierAnnotation {};

    /**
     * @brief The {@code [[=Qualifier]]} annotation.
     */
    inline constexpr QualifierAnnotation Qualifier{};

    /**
     * @struct ScopeAnnotation
     * @brief Meta-annotation marking an annotation type as a scope.
     *
     * Spell it {@code [[=Scope]]} on the annotation type itself. The only scope
     * the injector currently implements is {@code Singleton}; a class annotated
     * with any other scope-marked annotation is rejected at compile time.
     */
    struct ScopeAnnotation {};

    /**
     * @brief The {@code [[=Scope]]} annotation.
     */
    inline constexpr ScopeAnnotation Scope{};

    /**
     * @struct InjectAnnotation
     * @brief Annotation type marking the constructor the injector should use.
     *
     * Spell it {@code [[=Inject]]} on exactly one constructor.
     */
    struct InjectAnnotation {};

    /**
     * @brief The {@code [[=Inject]]} annotation.
     */
    inline constexpr InjectAnnotation Inject{};

    /**
     * @struct SingletonAnnotation
     * @brief Scope annotation marking a class as singleton-scoped.
     *
     * Spell it {@code [[=Singleton]]} on the class. The injector creates one
     * shared instance on first resolution and returns it thereafter.
     */
    #ifdef __cpp_impl_reflection
    struct [[=Scope]] SingletonAnnotation {};
    #else
    struct SingletonAnnotation {};
    #endif

    /**
     * @brief The {@code [[=Singleton]]} annotation.
     */
    inline constexpr SingletonAnnotation Singleton{};

    #ifdef __cpp_impl_reflection
    /**
     * @struct Named
     * @brief String-valued qualifier for distinguishing bindings of the same type.
     *
     * Apply to constructor parameters as {@code [[=Named("key")]]} and bind with
     * {@code binder.bind<T>().annotated_with(Named("key"))}.
     */
    struct [[=Qualifier]] Named {
        const char* value; ///< The annotation string, in static storage.

        consteval explicit Named(StringView name):
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
            return Hash<StringView>()(named.value);
        }
    };
}

template <>
struct stdx::core::hash<Named>: public Hash<Named> {};
#endif
