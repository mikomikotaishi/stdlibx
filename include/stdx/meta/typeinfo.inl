#pragma once

/**
 * @namespace stdx::meta
 * @brief Standard library metaprogramming operations.
 */
export namespace stdx::meta {
    using TypeInfo = std::type_info;

    using BadCastException = std::bad_cast;
    using BadTypeIdException = std::bad_typeid;
}
