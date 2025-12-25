/**
 * @file compare.cppm
 * @module core:compare
 * @brief Module file for standard library comparison operations.
 *
 * This file contains the implementation of the comparison operations in the standard library.
 */

module;

#if __has_include(<compare>)
#include <compare>
#endif

#include "Macros.hpp"

export module core:compare;

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    #if __has_include(<span>)
    template <typename T>
    concept ThreeWayComparable = std::three_way_comparable<T>;

    template <typename T, typename U>
    concept ThreeWayComparableWith = std::three_way_comparable_with<T, U>;

    using PartialOrdering = std::partial_ordering;
    using WeakOrdering = std::weak_ordering;
    using StrongOrdering = std::strong_ordering;

    template <typename... Ts>
    using CommonComparisonCategory = std::common_comparison_category<Ts...>;

    template <typename T>
    using CompareThreeWayResult = std::compare_three_way_result<T>;

    using CompareThreeWay = std::compare_three_way;
    
    using StrongFallback = decltype(std::compare_strong_order_fallback);
    using WeakFallback = decltype(std::compare_weak_order_fallback);
    using PartialFallback = decltype(std::compare_partial_order_fallback);

    inline namespace comparison {
        inline constexpr decltype(std::strong_order) StrongOrder = std::strong_order;
        inline constexpr decltype(std::weak_order) WeakOrder = std::weak_order;
        inline constexpr decltype(std::partial_order) PartialOrder = std::partial_order;

        inline constexpr StrongFallback CompareStrongOrderFallback = std::compare_strong_order_fallback;
        inline constexpr WeakFallback CompareWeakOrderFallback = std::compare_weak_order_fallback;
        inline constexpr PartialFallback ComparePartialOrderFallback = std::compare_partial_order_fallback;
    }

    using std::is_eq;
    using std::is_neq;
    using std::is_lt;
    using std::is_lteq;
    using std::is_gt;
    using std::is_gteq;
    #endif
}

#if (defined(STDLIBX_NO_STD) || defined(STDLIBX_NO_ALLOC)) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_CORE_MODULE_EXPORT_CORE();
#endif

