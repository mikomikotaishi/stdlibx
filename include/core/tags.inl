#pragma once

/**
 * @namespace core::prelude
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core::prelude {
    /**
     * @class Tags
     * @brief A utility class containing standard library tags.
     */
    class Tags final {
    public:
        Tags() = delete;

        #ifdef __cpp_lib_expected
        using Unexpect = std::unexpect_t;
        #endif
        using FromRange = std::from_range_t;
        template <usize I>
        using InPlaceIndex = std::in_place_index_t<I>;
        using InPlace = std::in_place_t;
        template <typename T>
        using InPlaceType = std::in_place_type_t<T>;
        using PiecewiseConstruct = std::piecewise_construct_t;

        #ifdef __cpp_lib_linalg
        using ColumnMajor = std::linalg::column_major_t;
        using RowMajor = std::linalg::row_major_t;
        using UpperTriangle = std::linalg::upper_triangle_t;
        using LowerTriangle = std::linalg::lower_triangle_t;
        using ImplicitUnitDiagonal = std::linalg::implicit_unit_diagonal_t;
        using ExplicitDiagonal = std::linalg::explicit_diagonal_t;
        #endif

        #ifdef __cpp_lib_mdspan
        using FullExtent = std::full_extent_t;
        #endif

        #ifdef __cpp_lib_flat_map
        using SortedEquivalent = std::sorted_equivalent_t;
        using SortedUnique = std::sorted_unique_t;
        #endif
        using AllocatorArgument = std::allocator_arg_t;
        using DestroyingDelete = std::destroying_delete_t;
        using NoThrow = std::nothrow_t;
        using AdoptLock = std::adopt_lock_t;
        using DeferLock = std::defer_lock_t;
        using TryToLock = std::try_to_lock_t;
        using NoStopState = std::nostopstate_t;
        using Last = std::chrono::last_spec;

        #ifdef __cpp_lib_expected
        static constexpr Unexpect UNEXPECT = std::unexpect;
        #endif
        static constexpr FromRange FROM_RANGE = std::from_range;
        static constexpr InPlace IN_PLACE = std::in_place;
        template <usize I>
        static constexpr InPlaceIndex<I> IN_PLACE_INDEX = std::in_place_index<I>;
        template <typename T>
        static constexpr InPlaceType<T> IN_PLACE_TYPE = std::in_place_type<T>;
        static constexpr PiecewiseConstruct PIECEWISE_CONSTRUCT = std::piecewise_construct;
        #ifdef __cpp_lib_linalg
        static constexpr ColumnMajor COLUMN_MAJOR = std::linalg::column_major;
        static constexpr RowMajor ROW_MAJOR = std::linalg::row_major;
        static constexpr UpperTriangle UPPER_TRIANGLE = std::linalg::upper_triangle;
        static constexpr LowerTriangle LOWER_TRIANGLE = std::linalg::lower_triangle;
        static constexpr ImplicitUnitDiagonal IMPLICIT_UNIT_DIAGONAL = std::linalg::implicit_unit_diagonal;
        static constexpr ExplicitDiagonal EXPLICIT_DIAGONAL = std::linalg::explicit_diagonal;
        #endif
        #ifdef __cpp_lib_mdspan
        static constexpr FullExtent FULL_EXTENT = std::full_extent;
        #endif
        #ifdef __cpp_lib_flat_map
        static constexpr SortedEquivalent SORTED_EQUIVALENT = std::sorted_equivalent;
        static constexpr SortedUnique SORTED_UNIQUE = std::sorted_unique;
        #endif
        static constexpr AllocatorArgument ALLOCATOR_ARGUMENT = std::allocator_arg;
        static constexpr DestroyingDelete DESTROYING_DELETE = std::destroying_delete;
        static constexpr NoThrow NO_THROW = std::nothrow;
        static constexpr AdoptLock ADOPT_LOCK = std::adopt_lock;
        static constexpr DeferLock DEFER_LOCK = std::defer_lock;
        static constexpr TryToLock TRY_TO_LOCK = std::try_to_lock;
        static constexpr NoStopState NO_STOP_STATE = std::nostopstate;
        static constexpr Last LAST = std::chrono::last;
    };
}
