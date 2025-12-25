/**
 * @file tuple.cppm
 * @module core:tuple
 * @brief Module file for standard library tuple operations.
 *
 * This file contains the implementation of the tuple operations in the standard library.
 */

module;

#include <tuple>

export module core:tuple;

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

    template <std::size_t I, typename T>
    using TupleElement = std::tuple_element<I, T>;

    template <std::size_t I, typename T>
    using TupleElementType = std::tuple_element_t<I, T>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename T>
    constexpr bool TupleSize_v = std::tuple_size_v<T>;

    template <std::size_t I, typename T>
    using TupleElement_t = std::tuple_element_t<I, T>;
    #endif

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

#if (defined(STDLIBX_NO_STD) || defined(STDLIBX_NO_ALLOC)) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_CORE_MODULE_EXPORT_CORE();
#endif
