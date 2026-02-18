#pragma once

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    template <typename T>
    using InitializerList = std::initializer_list<T>;

    using std::begin;
    using std::end;
}
