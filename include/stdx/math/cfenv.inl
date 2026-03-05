#pragma once

/**
 * @namespace stdx::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace stdx::math {
    using ::core::math::FloatingPointEnvironmentType;
    using ::core::math::FloatingPointExceptionType;

    using ::core::math::feclearexcept;
    using ::core::math::fetestexcept;
    using ::core::math::feraiseexcept;
    using ::core::math::fegetexceptflag;
    using ::core::math::fesetexceptflag;
    using ::core::math::fegetround;
    using ::core::math::fesetround;
    using ::core::math::fegetenv;
    using ::core::math::fesetenv;
    using ::core::math::feholdexcept;
    using ::core::math::feupdateenv;

    using ::core::math::FE_ALL_EXCEPT;
    using ::core::math::FE_DIVBYZERO;
    using ::core::math::FE_INEXACT;
    using ::core::math::FE_INVALID;
    using ::core::math::FE_OVERFLOW;
    using ::core::math::FE_UNDERFLOW;

    using ::core::math::FE_DOWNWARD;
    using ::core::math::FE_TONEAREST;
    using ::core::math::FE_TOWARDZERO;
    using ::core::math::FE_UPWARD;

    using ::core::math::FE_DFL_ENV;
}
