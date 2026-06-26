#pragma once

#ifdef __cpp_lib_reflection
using stdx::collections::Vector;
using stdx::meta::RemoveConstVolatileReferenceType;
using stdx::meta::reflect::AccessContext;
using stdx::meta::reflect::Class;
using stdx::meta::reflect::Info;
using stdx::meta::reflect::Operators;

namespace reflect = stdx::meta::reflect;

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
namespace stdx::core {
    /**
     * @class InvalidThrowsAnnotationException
     * @brief Thrown (at constant-evaluation time) when a Throws annotation contradicts
     * the function it decorates - currently, when a noexcept function or call operator
     * carries a Throws naming at least one exception type.
     * @extends Exception
     */
    export class InvalidThrowsAnnotationException: public Exception {
    private:
        String message;
        Info about;
        SourceLocation location;
    public:
        consteval InvalidThrowsAnnotationException(StringView msg, Info about, SourceLocation location = SourceLocation::current()) noexcept:
            message{String(msg)}, about{about}, location{location} {}

        consteval InvalidThrowsAnnotationException(const InvalidThrowsAnnotationException&) = default;
        consteval InvalidThrowsAnnotationException(InvalidThrowsAnnotationException&&) = default;

        consteval InvalidThrowsAnnotationException& operator=(const InvalidThrowsAnnotationException&) = default;
        consteval InvalidThrowsAnnotationException& operator=(InvalidThrowsAnnotationException&&) = default;

        [[nodiscard]]
        consteval const char* what() const noexcept override {
            return message.c_str();
        }

        [[nodiscard]]
        consteval Info from() const noexcept {
            return about;
        }

        [[nodiscard]]
        consteval SourceLocation where() const noexcept {
            return location;
        }
    };

    template <typename T>
    constexpr bool IsThrowsValue = false;

    template <Extends<Exception>... Es>
    constexpr bool IsThrowsValue<Throws<Es...>> = true;

    /**
     * @concept IsThrows
     * @brief Satisfied by any Throws specialisation.
     * @tparam T The annotation type to check.
     */
    export template <typename T>
    concept IsThrows = IsThrowsValue<RemoveConstVolatileReferenceType<T>>;

    /**
     * @brief The reflections of the exception types carried by a Throws annotation.
     * @tparam Es The exception types named by the Throws annotation.
     * @return A vector of Info reflections, one per type, in declaration order.
     */
    template <Extends<Exception>... Es>
    [[nodiscard]]
    consteval Vector<Info> throws_pack_infos(Throws<Es...> _) {
        return Vector<Info>{ ^^Es... };
    }

    /**
     * @brief Append @p value to @p out unless an equal reflection is already present.
     * @param out The vector to append to.
     * @param value The reflection to append if not already present.
     */
    consteval void add_unique(Vector<Info>& out, Info value) {
        for (const Info existing: out) {
            if (existing == value) {
                return;
            }
        }
        out.emplace_back(value);
    }

    /**
     * @brief Gets the non-static operator() members of @p t, or an empty vector if @p t
     * is not a (non-union) class type.
     * @param t The reflection to inspect.
     * @return A vector of Info reflections, one per operator() member.
     */
    [[nodiscard]]
    consteval Vector<Info> call_operators_of(Info t) {
        Vector<Info> result;
        const Info cls = reflect::dealias(t);
        if (!reflect::is_class_type(cls) || reflect::is_union_type(cls)) {
            return result;
        }
        for (const Info m: reflect::members_of(cls, AccessContext::unchecked())) {
            if (
                reflect::is_function(m) &&
                reflect::is_operator_function(m) &&
                reflect::operator_of(m) == Operators::PARENTHESES
            ) {
                result.emplace_back(m);
            }
        }
        return result;
    }

    /**
     * @brief Checks if @p t is a class type declaring at least one operator().
     * @param t The reflection to check.
     * @return true if @p t is a class type with at least one operator(), false otherwise.
     */
    [[nodiscard]]
    consteval bool has_call_operator(Info t) {
        return reflect::is_type(t) && !call_operators_of(t).empty();
    }

    /**
     * @brief Adds the exception types carried by annotation @p Ann to @p out, if it
     * is a Throws annotation; a no-op for any other annotation.
     * @tparam Ann The reflection of an annotation applied to some entity.
     * @param out The vector to append to.
     */
    template <Info Ann>
    consteval void collect_annotation_throws(Vector<Info>& out) {
        using Annotated = RemoveConstVolatileReferenceType<typename [:reflect::type_of(Ann):]>;
        if constexpr (IsThrows<Annotated>) {
            for (const Info e: throws_pack_infos(Annotated{})) {
                add_unique(out, e);
            }
        }
    }

    /**
     * @brief Retrieves the deduplicated exception-type reflections declared via Throws
     * annotations on @p Fn (a function) or on each operator() of @p Fn (a callable
     * type), in the order first encountered.
     * @tparam Fn The reflection of a function or callable type.
     * @return A vector of Info reflections, one per exception type.
     *
     * A noexcept function (or call operator) carrying a Throws annotation that names
     * at least one exception type is contradictory: it promises to throw nothing yet
     * documents what it throws. Such a function is rejected at the point this query is
     * evaluated, surfacing as a constant-evaluation diagnostic.
     */
    template <Info Fn>
    [[nodiscard]]
    [[=Throws<InvalidThrowsAnnotationException>()]]
    consteval Vector<Info> collect_thrown_infos() {
        Vector<Info> result;
        if constexpr (reflect::is_function(Fn)) {
            template for (constexpr Info ann: reflect::define_static_array(reflect::annotations_of(Fn))) {
                collect_annotation_throws<ann>(result);
            }
            if (reflect::is_noexcept(Fn) && !result.empty()) {
                throw InvalidThrowsAnnotationException("noexcept function annotated with a non-empty Throws", Fn);
            }
        } else {
            template for (constexpr Info op: reflect::define_static_array(call_operators_of(Fn))) {
                Vector<Info> op_throws;
                template for (constexpr Info ann: reflect::define_static_array(reflect::annotations_of(op))) {
                    collect_annotation_throws<ann>(op_throws);
                }
                if (reflect::is_noexcept(op) && !op_throws.empty()) {
                    throw InvalidThrowsAnnotationException("noexcept call operator annotated with a non-empty Throws", op);
                }
                for (const Info e: op_throws) {
                    add_unique(result, e);
                }
            }
        }
        return result;
    }

    /**
     * @brief The deduplicated thrown-exception reflections of @p Fn promoted to a
     * static array, so they can be spliced inside a trailing return type.
     * @tparam Fn The reflection of a function or callable type.
     */
    template <Info Fn>
    constexpr auto thrown_infos = reflect::define_static_array(collect_thrown_infos<Fn>());

    /**
     * @brief Builds the Tuple<Class<E>...> for @p Fn from the indices of its
     * thrown_infos. The element types are spliced directly into the trailing return
     * type, so the result type is fully spelled out rather than deduced.
     * @tparam Fn The reflection of a function or callable type.
     * @tparam I The indices 0 .. thrown_infos<Fn>.size() - 1.
     */
    template <Info Fn, usize... I>
    [[nodiscard]]
    consteval Tuple<Class<typename [:thrown_infos<Fn>[I]:]>...> build_thrown_exceptions(IndexSequence<I...> _) {
        return Tuple<Class<typename [:thrown_infos<Fn>[I]:]>...>{};
    }

    /**
     * @concept FunctionOrCallable
     * @brief Satisfied by a reflection that names a function (free, member,
     * operator, ...) or a callable class type (one declaring operator()).
     *
     * Constrains Ops::thrown_exceptions() so a Throws query can only be made against
     * something that is actually a function or callable.
     * @tparam R The reflection to test.
     */
    export template <Info R>
    concept FunctionOrCallable = reflect::is_function(R) || has_call_operator(R);
}

export namespace stdx::meta::reflect {
    /**
     * @brief The result type of Ops::thrown_exceptions<Fn>(): a Tuple<Class<E>...> of
     * the exception types @p Fn declares via Throws annotations, deduplicated and in
     * declaration order. Empty (Tuple<>) when none are declared.
     * @tparam Fn The reflection of a function or callable type.
     */
    template <Info Fn>
    using ThrownExceptions = decltype(
        stdx::core::build_thrown_exceptions<Fn>(
            IndexSequenceOf<thrown_infos<Fn>.size()>{}
        )
    );
}
#endif
