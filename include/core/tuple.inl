#pragma once

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    template <typename... Elements>
    using Tuple = std::tuple<Elements...>;

    template <typename T>
    using TupleSize = std::tuple_size<T>;

    template <typename T>
    constexpr bool TupleSizeValue = std::tuple_size_v<T>;

    template <usize I, typename T>
    using TupleElement = std::tuple_element<I, T>;

    template <usize I, typename T>
    using TupleElementType = std::tuple_element_t<I, T>;

    using std::operator==;
    using std::operator<=>;

    using std::make_tuple;
    using std::tie;
    using std::forward_as_tuple;
    using std::tuple_cat;
    using std::get;
    using std::apply;
    using std::make_from_tuple;

    using std::swap;
}
