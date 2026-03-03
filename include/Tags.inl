#pragma once

/**
 * @class Tags
 * @brief A utility class containing standard library tags.
 */
class Tags {
private:
    using usize = ::core::usize;
public:
    Tags() = delete;

    #ifdef __cpp_lib_expected
    using Unexpect = ::core::UnexpectTag;
    #endif
    using FromRange = ::core::ranges::FromRangeTag;
    template <usize I>
    using InPlaceIndex = ::core::util::InPlaceIndexTag<I>;
    using InPlace = ::core::util::InPlaceTag;
    template <typename T>
    using InPlaceType = ::core::util::InPlaceTypeTag<T>;
    using PiecewiseConstruct = ::core::util::PiecewiseConstructTag;

    #ifdef __cpp_lib_linalg
    using ColumnMajor = ::core::math::linalg::ColumnMajorTag;
    using RowMajor = ::core::math::linalg::RowMajorTag;
    using UpperTriangle = ::core::math::linalg::UpperTriangleTag;
    using LowerTriangle = ::core::math::linalg::LowerTriangleTag;
    using ImplicitUnitDiagonal = ::core::math::linalg::ImplicitUnitDiagonalTag;
    using ExplicitDiagonal = ::core::math::linalg::ExplicitDiagonalTag;
    #endif

    #ifdef __cpp_lib_mdspan
    using FullExtent = ::core::mdspan::FullExtentTag;
    #endif

    #ifndef STDLIBX_NO_STD_NO_ALLOC_FEATURES
    #ifdef __cpp_lib_flat_map
    using SortedEquivalent = ::alloc::collections::SortedEquivalentTag;
    using SortedUnique = ::alloc::collections::SortedUniqueTag;
    #endif
    using AllocatorArgument = ::alloc::AllocatorArgumentTag;
    using DestroyingDelete = ::alloc::DestroyingDeleteTag;
    using NoThrow = ::alloc::NoThrowTag;
    #ifndef STDLIBX_NO_STD_FEATURES
    using AdoptLock = ::stdx::sync::AdoptLockTag;
    using DeferLock = ::stdx::sync::DeferLockTag;
    using TryToLock = ::stdx::sync::TryToLockTag;
    using NoStopState = ::stdx::thread::NoStopStateTag;
    using Last = ::stdx::time::LastSpecifier;
    #endif
    #endif

    #ifdef __cpp_lib_expected
    static constexpr Unexpect UNEXPECT = ::core::Unexpect;
    #endif
    static constexpr FromRange FROM_RANGE = ::core::ranges::FromRange;
    static constexpr InPlace IN_PLACE = ::core::util::InPlace;
    template <usize I>
    static constexpr InPlaceIndex<I> IN_PLACE_INDEX = ::core::util::InPlaceIndex<I>;
    template <typename T>
    static constexpr InPlaceType<T> IN_PLACE_TYPE = ::core::util::InPlaceType<T>;
    static constexpr PiecewiseConstruct PIECEWISE_CONSTRUCT = ::core::util::PiecewiseConstruct;
    #ifdef __cpp_lib_linalg
    static constexpr ColumnMajor COLUMN_MAJOR = ::core::math::linalg::ColumnMajor;
    static constexpr RowMajor ROW_MAJOR = ::core::math::linalg::RowMajor;
    static constexpr UpperTriangle UPPER_TRIANGLE = ::core::math::linalg::UpperTriangle;
    static constexpr LowerTriangle LOWER_TRIANGLE = ::core::math::linalg::LowerTriangle;
    static constexpr ImplicitUnitDiagonal IMPLICIT_UNIT_DIAGONAL = ::core::math::linalg::ImplicitUnitDiagonal;
    static constexpr ExplicitDiagonal EXPLICIT_DIAGONAL = ::core::math::linalg::ExplicitDiagonal;
    #endif
    #ifdef __cpp_lib_mdspan
    static constexpr FullExtent FULL_EXTENT = ::core::mdspan::FullExtent;
    #endif
    #ifndef STDLIBX_NO_STD_NO_ALLOC_FEATURES
    #ifdef __cpp_lib_flat_map
    static constexpr SortedEquivalent SORTED_EQUIVALENT = ::alloc::collections::SortedEquivalent;
    static constexpr SortedUnique SORTED_UNIQUE = ::alloc::collections::SortedUnique;
    #endif
    static constexpr AllocatorArgument ALLOCATOR_ARGUMENT = ::alloc::AllocatorArgument;
    static constexpr DestroyingDelete DESTROYING_DELETE = ::alloc::DestroyingDelete;
    static constexpr NoThrow NO_THROW = ::alloc::NoThrow;
    #ifndef STDLIBX_NO_STD_FEATURES
    static constexpr AdoptLock ADOPT_LOCK = ::stdx::sync::AdoptLock;
    static constexpr DeferLock DEFER_LOCK = ::stdx::sync::DeferLock;
    static constexpr TryToLock TRY_TO_LOCK = ::stdx::sync::TryToLock;
    static constexpr NoStopState NO_STOP_STATE = ::stdx::thread::NoStopState;
    static constexpr Last LAST = ::stdx::time::Last;
    #endif
    #endif
};