#pragma once

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
    template <typename T, T... Elements>
    using IntegerSequence = std::integer_sequence<T>;

    template <typename T, T N>
    using IntegerSequenceOf = std::make_integer_sequence<T, N>;

    template <usize... I>
    using IndexSequence = std::index_sequence<I...>;

    template <typename... T>
    using IndexSequenceFor = std::index_sequence_for<T...>;

    template <usize N>
    using IndexSequenceOf = std::make_index_sequence<N>;

    using std::begin;
    using std::end;

    using std::swap;
}
