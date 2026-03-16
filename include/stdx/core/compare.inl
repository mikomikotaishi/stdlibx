#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::prelude::ThreeWayComparable;
    using ::core::prelude::ThreeWayComparableWith;

    using ::core::prelude::Compare;

    using ::core::prelude::PartialOrdering;
    using ::core::prelude::WeakOrdering;
    using ::core::prelude::StrongOrdering;

    using ::core::prelude::CommonComparisonCategory;
    using ::core::prelude::CompareThreeWayResult;

    using ::core::prelude::CompareThreeWay;
    
    using ::core::prelude::StrongFallback;
    using ::core::prelude::WeakFallback;
    using ::core::prelude::PartialFallback;

    inline namespace comparison {
        using ::core::prelude::comparison::StrongOrder;
        using ::core::prelude::comparison::WeakOrder;
        using ::core::prelude::comparison::PartialOrder;
        using ::core::prelude::comparison::CompareStrongOrderFallback;
        using ::core::prelude::comparison::CompareWeakOrderFallback;
        using ::core::prelude::comparison::ComparePartialOrderFallback;
    }

    using ::core::prelude::is_eq;
    using ::core::prelude::is_neq;
    using ::core::prelude::is_lt;
    using ::core::prelude::is_lteq;
    using ::core::prelude::is_gt;
    using ::core::prelude::is_gteq;

    using ::core::prelude::operator==;
    using ::core::prelude::operator<;
    using ::core::prelude::operator>;
    using ::core::prelude::operator<=;
    using ::core::prelude::operator>=;
    using ::core::prelude::operator<=>;
}
