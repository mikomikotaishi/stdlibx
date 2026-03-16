#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::prelude::SameAs;
    using ::core::prelude::DerivedFrom;
    using ::core::prelude::ConvertibleTo;
    using ::core::prelude::CommonReferenceWith;
    using ::core::prelude::CommonWith;
    using ::core::prelude::Integral;
    using ::core::prelude::SignedIntegral;
    using ::core::prelude::UnsignedIntegral;
    using ::core::prelude::FloatingPoint;
    using ::core::prelude::AssignableFrom;
    using ::core::prelude::Swappable;
    using ::core::prelude::SwappableWith;
    using ::core::prelude::Destructible;
    using ::core::prelude::ConstructibleFrom;
    using ::core::prelude::DefaultInitializable;
    using ::core::prelude::MoveConstructible;
    using ::core::prelude::CopyConstructible;
    using ::core::prelude::EqualityComparable;
    using ::core::prelude::EqualityComparableWith;
    using ::core::prelude::TotallyOrdered;
    using ::core::prelude::TotallyOrderedWith;
    using ::core::prelude::Movable;
    using ::core::prelude::Copyable;
    using ::core::prelude::Semiregular;
    using ::core::prelude::Regular;
    using ::core::prelude::Invocable;
    using ::core::prelude::RegularInvocable;
    using ::core::prelude::Predicate;
    using ::core::prelude::Relation;
    using ::core::prelude::EquivalenceRelation;
    using ::core::prelude::StrictWeakOrder;
    using ::core::prelude::CharacterLike;
    using ::core::prelude::Numeric;
}
