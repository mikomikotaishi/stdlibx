#pragma once

/**
 * @namespace core::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace core::meta {
    using TypeInfo = std::type_info;

    #ifndef STDLIBX_NO_STD
    using BadCastException = std::bad_cast;
    using BadTypeIDException = std::bad_typeid;
    #endif
}
