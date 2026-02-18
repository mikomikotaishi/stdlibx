#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    #if __has_include(<compare>)
    using ::core::ThreeWayComparable;
    using ::core::ThreeWayComparableWith;

    using ::core::PartialOrdering;
    using ::core::WeakOrdering;
    using ::core::StrongOrdering;

    using ::core::CommonComparisonCategory;
    using ::core::CompareThreeWayResult;

    using ::core::CompareThreeWay;
    
    using ::core::StrongFallback;
    using ::core::WeakFallback;
    using ::core::PartialFallback;

    inline namespace comparison {
        using ::core::comparison::StrongOrder;
        using ::core::comparison::WeakOrder;
        using ::core::comparison::PartialOrder;
        using ::core::comparison::CompareStrongOrderFallback;
        using ::core::comparison::CompareWeakOrderFallback;
        using ::core::comparison::ComparePartialOrderFallback;
    }

    using ::core::is_eq;
    using ::core::is_neq;
    using ::core::is_lt;
    using ::core::is_lteq;
    using ::core::is_gt;
    using ::core::is_gteq;
    #endif
}
