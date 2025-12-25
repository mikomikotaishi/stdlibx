/**
 * @file concepts.cppm
 * @module core:concepts
 * @brief Module file for standard library concept operations.
 *
 * This file contains the implementation of the concept operations in the standard library.
 */

module;

#if __has_include(<concepts>)
#include <concepts>
#endif

#include "Macros.hpp"

export module core:concepts;

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    #if __has_include(<concepts>)
    template <typename T, typename U>
    concept SameAs = std::same_as<T, U>;

    template <typename Derived, typename Base>
    concept DerivedFrom = std::derived_from<Derived, Base>;

    template <typename From, typename To>
    concept ConvertibleTo = std::convertible_to<From, To>;

    template <typename T, typename U>
    concept CommonReferenceWith = std::common_reference_with<T, U>;

    template <typename T, typename U>
    concept CommonWith = std::common_with<T, U>;

    template <typename T>
    concept Integral = std::integral<T>;

    template <typename T>
    concept SignedIntegral = std::signed_integral<T>;

    template <typename T>
    concept UnsignedIntegral = std::unsigned_integral<T>;

    template <typename T>
    concept FloatingPoint = std::floating_point<T>;

    template <typename Lhs, typename Rhs>
    concept AssignableFrom = std::assignable_from<Lhs, Rhs>;

    template <typename T>
    concept Swappable = std::swappable<T>;

    template <typename T, typename U>
    concept SwappableWith = std::swappable_with<T, U>;

    template <typename T>
    concept Destructible = std::destructible<T>;

    template <typename T, typename... Args>
    concept ConstructibleFrom = std::constructible_from<T, Args...>;

    template <typename T>
    concept DefaultInitializable = std::default_initializable<T>;

    template <typename T>
    concept MoveConstructible = std::move_constructible<T>;

    template <typename T>
    concept CopyConstructible = std::copy_constructible<T>;

    template <typename T>
    concept EqualityComparable = std::equality_comparable<T>;

    template <typename T, typename U>
    concept EqualityComparableWith = std::equality_comparable_with<T, U>;

    template <typename T>
    concept TotallyOrdered = std::totally_ordered<T>;

    template <typename T, typename U>
    concept TotallyOrderedWith = std::totally_ordered_with<T, U>;

    template <typename T>
    concept Movable = std::movable<T>;

    template <typename T>
    concept Copyable = std::copyable<T>;

    template <typename T>
    concept Semiregular = std::semiregular<T>;

    template <typename T>
    concept Regular = std::regular<T>;

    template <typename Fn, typename... Args>
    concept Invocable = std::invocable<Fn, Args...>;

    template <typename Fn, typename... Args>
    concept RegularInvocable = std::regular_invocable<Fn, Args...>;

    template <typename Fn, typename... Args>
    concept Predicate = std::predicate<Fn, Args...>;

    template <typename Rel, typename T, typename U>
    concept Relation = std::relation<Rel, T, U>;

    template <typename Rel, typename T, typename U>
    concept EquivalenceRelation = std::equivalence_relation<Rel, T, U>;

    template <typename Rel, typename T, typename U>
    concept StrictWeakOrder = std::strict_weak_order<Rel, T, U>;
    #endif
}

#if (defined(STDLIBX_NO_STD) || defined(STDLIBX_NO_ALLOC)) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_CORE_MODULE_EXPORT_CORE();
#endif

