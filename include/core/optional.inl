#pragma once

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    template <typename T>
    using Optional = std::optional<T>;

    using std::nullopt;
    using NullOption = std::nullopt_t;

    using BadOptionalAccessException = std::bad_optional_access;

    using std::hash;
    using std::swap;

    using std::make_optional;
}
