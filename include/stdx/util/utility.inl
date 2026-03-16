#pragma once

/**
 * @namespace stdx::util
 * @brief Wrapper namespace for standard library utility operations.
 */
export namespace stdx::util {
    using ::core::util::swap;
    using ::core::util::exchange;
    using ::core::util::forward;
    #ifdef __cpp_lib_forward_like
    using ::core::util::forward_like;
    #endif
    using ::core::util::move;
    using ::core::util::move_if_noexcept;
    using ::core::util::as_const;
    using ::core::util::declval;
    #ifdef __cpp_lib_to_underlying
    using ::core::util::to_underlying;
    #endif
    using ::core::util::cmp_equal;
    using ::core::util::cmp_not_equal;
    using ::core::util::cmp_less;
    using ::core::util::cmp_greater;
    using ::core::util::cmp_less_equal;
    using ::core::util::cmp_greater_equal;
    using ::core::util::in_range;

    using ::core::util::PiecewiseConstructTag;
    using ::core::util::InPlaceTag;
    using ::core::util::InPlaceTypeTag;
    using ::core::util::InPlaceIndexTag;
    using ::core::util::PiecewiseConstruct;
    using ::core::util::InPlace;
    using ::core::util::InPlaceType;
    using ::core::util::InPlaceIndex;
}
