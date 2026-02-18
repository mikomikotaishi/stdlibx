#pragma once

/**
 * @namespace alloc
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    template <typename T, T... Elements>
    using IntegerSequence = std::integer_sequence<T>;

    using std::make_integer_sequence;

    template <usize... I>
    using IndexSequence = std::index_sequence<I...>;

    template <typename... T>
    using IndexSequenceFor = std::index_sequence_for<T...>;

    using std::make_index_sequence;

    using std::begin;
    using std::end;

    using std::swap;
}
