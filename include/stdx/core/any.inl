#pragma once

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
    using Any = std::any;

    using BadAnyCastException = std::bad_any_cast;
}
