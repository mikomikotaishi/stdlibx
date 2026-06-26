#pragma once

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
    /**
     * @struct Throws
     * @brief An annotation indicating the exception types that a function may throw.
     * @tparam Es All exception types that a function may throw. Each must derive
     * from Exception.
     *
     * Apply it to a function (or a callable type's call operator) to document the
     * exceptions it may propagate, then recover the list with Ops::thrown_exceptions():
     * @code
     * [[=Throws<InvalidArgumentException, InvalidRangeException>()]]
     * i32 parse(StringView s);
     *
     * // Tuple<Class<InvalidArgumentException>, Class<InvalidRangeException>>
     * constexpr ThrownExceptions<^^parse> es = Ops::thrown_exceptions<^^parse>();
     * @endcode
     *
     * @note Only the annotation surface lives here, so it can be referenced before
     * the reflection layer (stdx::meta::reflect) is defined - that is what lets
     * reflect.inl annotate its own functions with Throws. The query machinery that
     * turns these annotations back into reflections lives in throws.inl, which is
     * included after the reflection layer.
     */
    template <Extends<Exception>... Es>
    struct Throws {
        static constexpr usize COUNT = sizeof...(Es); ///< Number of exception types named by this annotation.
    };
}