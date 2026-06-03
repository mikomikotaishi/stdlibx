#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    template <typename T>
    using InitializerList = std::initializer_list<T>;

    using std::begin;
    using std::end;
}
