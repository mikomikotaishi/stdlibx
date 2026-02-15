/**
 * @file any.cppm
 * @module stdx:core.any
 * @brief Module file for importing the Any class from the standard library.
 * 
 * This file exports the Any class operations in the standard library.
 */

module;

#include <any>

#include "Macros.hpp"

export module stdx:core.any;

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
