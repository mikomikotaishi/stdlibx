#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using Any = std::any;

    using BadAnyCastException = std::bad_any_cast;

    using std::make_any;
    using std::any_cast;

    using std::swap;
}
