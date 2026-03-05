#pragma once

/**
 * @namespace core::math
 * @brief Wrapper namespace for standard library mathematical operations.
 */
export namespace core::math {
    using FloatingPointEnvironmentType = std::fenv_t;
    using FloatingPointExceptionType = std::fexcept_t;

    using std::feclearexcept;
    using std::fetestexcept;
    using std::feraiseexcept;
    using std::fegetexceptflag;
    using std::fesetexceptflag;
    using std::fegetround;
    using std::fesetround;
    using std::fegetenv;
    using std::fesetenv;
    using std::feholdexcept;
    using std::feupdateenv;
}
