/**
 * @file sequence.cppm
 * @module core:sequence
 * @brief Module file for standard library sequence operations.
 *
 * This file contains the implementation of the sequence operations in the standard library.
 */

module;

#include <utility>

export module core:sequence;

/**
 * @namespace alloc
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    template <typename T, T... Elements>
    using IntegerSequence = std::integer_sequence<T>;

    using std::make_integer_sequence;

    template <std::size_t... I>
    using IndexSequence = std::index_sequence<I...>;

    template <typename... T>
    using IndexSequenceFor = std::index_sequence_for<T...>;

    using std::make_index_sequence;

    using std::begin;
    using std::end;

    using std::swap;
}

#if defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_ALLOC_MODULE_EXPORT_CORE();
#endif
