/**
 * @file tags.cppm
 * @module core:tags
 * @brief Module file for standard library tags aggregation class.
 *
 * This file contains the implementation of the tags aggregation class in the standard library.
 */

module;

export module core:tags;

import :cstddef;
import :expected;
import :ranges.ranges;
import :util.utility;

using core::Unexpect;
#if __has_include(<ranges>)
using core::ranges::FromRange;
#endif
using core::util::InPlace;
using core::util::InPlaceIndex;
using core::util::InPlaceType;
using core::util::PiecewiseConstruct;

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    /**
     * @class Tags
     * @brief A utility class containing standard library tags.
     */
    class Tags {
    private:
        using usize = ::core::usize;
    public:
        Tags() = delete;

        using UnexpectTag = ::core::UnexpectTag;
        #if __has_include(<ranges>)
        using FromRangeTag = ::core::ranges::FromRangeTag;
        #endif
        template <usize I>
        using InPlaceIndexTag = ::core::util::InPlaceIndexTag<I>;
        using InPlaceTag = ::core::util::InPlaceTag;
        template <typename T>
        using InPlaceTypeTag = ::core::util::InPlaceTypeTag<T>;
        using PiecewiseConstructTag = ::core::util::PiecewiseConstructTag;

        static constexpr UnexpectTag UNEXPECT = Unexpect;
        #if __has_include(<ranges>)
        static constexpr FromRangeTag FROM_RANGE = FromRange;
        #endif
        static constexpr InPlaceTag IN_PLACE = InPlace;
        template <usize I>
        static constexpr InPlaceIndexTag IN_PLACE_INDEX = InPlaceIndex<I>;
        template <typename T>
        static constexpr InPlaceTypeTag IN_PLACE_TYPE = InPlaceType<T>;
        static constexpr PiecewiseConstructTag PIECEWISE_CONSTRUCT = PiecewiseConstruct;
    };
}

#if (defined(STDLIBX_NO_STD) || defined(STDLIBX_NO_ALLOC)) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_CORE_MODULE_EXPORT_CORE();
#endif
