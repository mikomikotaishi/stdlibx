#pragma once

/**
 * @namespace stdx::util
 * @brief Standard library utility operations.
 */
export namespace stdx::util {
    using std::swap;
    using std::exchange;
    using std::forward;
    #ifdef __cpp_lib_forward_like
    using std::forward_like;
    #endif
    using std::move;
    using std::move_if_noexcept;
    using std::as_const;
    using std::declval;
    #ifdef __cpp_lib_to_underlying
    using std::to_underlying;
    #endif
    using std::cmp_equal;
    using std::cmp_not_equal;
    using std::cmp_less;
    using std::cmp_greater;
    using std::cmp_less_equal;
    using std::cmp_greater_equal;
    using std::in_range;

    template <typename T>
    using InPlaceTypeTag = std::in_place_type_t<T>;
    template <usize I>
    using InPlaceIndexTag = std::in_place_index_t<I>;

    template <typename T>
    inline constexpr InPlaceTypeTag InPlaceType = std::in_place_type<T>;

    template <usize I>
    inline constexpr InPlaceIndexTag InPlaceIndex = std::in_place_index<I>;
}
