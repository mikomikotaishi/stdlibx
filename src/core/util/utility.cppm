/**
 * @file utility.cppm
 * @module core:util.utility
 * @brief Module file for standard library utility operations.
 *
 * This file contains the implementation of the utility operations in the standard library.
 */

module;

#include <utility>

export module core:util.utility;

import :cstddef;

using core::usize;

/**
 * @namespace core::util
 * @brief Wrapper namespace for standard library utility operations.
 */
export namespace core::util {
    using std::swap;
    using std::exchange;
    using std::forward;
    using std::forward_like;
    using std::move;
    using std::move_if_noexcept;
    using std::as_const;
    using std::declval;
    using std::to_underlying;
    using std::cmp_equal;
    using std::cmp_not_equal;
    using std::cmp_less;
    using std::cmp_greater;
    using std::cmp_less_equal;
    using std::cmp_greater_equal;
    using std::in_range;

    using PiecewiseConstructTag = std::piecewise_construct_t;
    using InPlaceTag = std::in_place_t;
    template <typename T>
    using InPlaceTypeTag = std::in_place_type_t<T>;
    template <usize I>
    using InPlaceIndexTag = std::in_place_index_t<I>;

    inline constexpr PiecewiseConstructTag PiecewiseConstruct = std::piecewise_construct;
    inline constexpr InPlaceTag InPlace = std::in_place;

    template <typename T>
    inline constexpr InPlaceTypeTag InPlaceType = std::in_place_type<T>;

    template <usize I>
    inline constexpr InPlaceIndexTag InPlaceIndex = std::in_place_index<I>;
}
