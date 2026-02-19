/**
 * @file linq.cppm
 * @module stdx:linq
 * @brief Implementation of the LINQ extension library.
 *
 * This file contains the implementation of the LINQ extension library, which is used to perform 
 * LINQ-like queries on enumerable collection types.
 */

module;

#include "Macros.hpp"

export module stdx:linq;

import :main;

using stdx::collections::Vector;
using stdx::mem::SharedPointer;
using stdx::meta::ConditionalType;
using stdx::meta::IsLvalueReferenceValue;
using stdx::meta::RemoveConstVolatileReferenceType;
using stdx::meta::TrueType;
using stdx::ranges::Begin;
using stdx::ranges::End;
using stdx::ranges::ForwardRange;
using stdx::ranges::RangeDifference;
using stdx::ranges::RangeValue;
using stdx::ranges::ViewableRange;

using namespace stdx::core;
using namespace stdx::ranges::views;

/**
 * @namespace stdx::linq
 * @brief Wrapper namespace for standard library extension LINQ libraries.
 */
export namespace stdx::linq {

/**
 * @concept Filterable
 * @brief Concept that checks if Filter can be applied to the range.
 * 
 * @tparam Ran The range type.
 * @tparam Pred The predicate type.
 */
template <typename Ran, typename Pred>
concept Filterable = requires (Ran& r, Pred&& p) {
    { Filter(r, stdx::util::forward<Pred>(p)) };
};

/**
 * @concept Transformable
 * @brief Concept that checks if Transform can be applied to the range.
 * 
 * @tparam Ran The range type.
 * @tparam Func The function type.
 */
template <typename Ran, typename Func>
concept Transformable = requires (Ran& r, Func&& f) {
    { Transform(r, stdx::util::forward<Func>(f)) };
};

/**
 * @concept Sortable
 * @brief Concept that checks if sort() can be applied to the range.
 * 
 * @tparam Ran The range type.
 * @tparam KeySelector The key selector type.
 * @tparam Collection The collection type (defaults to Vector).
 */
template <typename Ran, typename KeySelector, typename Collection = Vector<RangeValue<Ran>>>
concept Sortable = ForwardRange<Ran> && requires (Collection c, KeySelector&& k) {
    { stdx::ranges::sort(c, Less<>{}, stdx::util::forward<KeySelector>(k)) };
};

/**
 * @concept Distinguishable
 * @brief Concept that checks if unique() can be applied to the range.
 * 
 * @tparam Ran The range type.
 * @tparam Collection The collection type (defaults to Vector).
 */
template <typename Ran, typename Collection = Vector<RangeValue<Ran>>>
concept Distinguishable = ForwardRange<Ran> &&
    requires (Collection c) { { stdx::ranges::sort(c) }; } &&
    requires (Collection c) { { stdx::ranges::unique(c) }; };

/**
 * @concept AnyOfQueryable
 * @brief Concept that checks if any_of() can be applied to the range.
 * 
 * @tparam Ran The range type.
 * @tparam Pred The predicate type.
 */
template <typename Ran, typename Pred>
concept AnyOfQueryable = requires (const Ran& r, Pred&& p) {
    { stdx::ranges::any_of(r, stdx::util::forward<Pred>(p)) };
};

/**
 * @concept AllOfQueryable
 * @brief Concept that checks if all_of() can be applied to the range.
 * 
 * @tparam Ran The range type.
 * @tparam Pred The predicate type.
 */
template <typename Ran, typename Pred>
concept AllOfQueryable = requires (const Ran& r, Pred&& p) {
    { stdx::ranges::all_of(r, stdx::util::forward<Pred>(p)) };
};

/**
 * @concept CountIfQueryable
 * @brief Concept that checks if count_if() can be applied to the range.
 * 
 * @tparam Ran The range type.
 * @tparam Pred The predicate type.
 */
template <typename Ran, typename Pred>
concept CountIfQueryable = requires (const Ran& r, Pred&& p) {
    { stdx::ranges::count_if(r, stdx::util::forward<Pred>(p)) };
};

/**
 * @concept PushBackable
 * @brief Concept that checks if a collection has a push_back method.
 * 
 * @tparam Coll The collection type.
 * @tparam T The element type.
 */
template <typename Coll, typename T>
concept PushBackable = requires (Coll c, T elem) {
    { c.push_back(stdx::util::forward<T>(elem)) };
};

/**
 * @class Query
 * @brief Class for representing LINQ-like queries of collection types.
 * 
 * @tparam Ran A ViewableRange type.
 *
 * The Query class provides LINQ-style operations for any range type that satisfies the ViewableRange concept.
 */
template <ViewableRange Ran>
class Query {
    template <ViewableRange>
    friend class Query;
private:
    SharedPointer<void> owner; ///< Shared pointer to the owner of the range, if applicable.
    Ran range; ///< The underlying range being queried.

    /**
     * @brief Creates a shared pointer to the owner object.
     *
     * @tparam T The type of the owner.
     * @param r The owner object.
     * @return SharedPointer to the owner.
     */
    template <typename T>
    static SharedPointer<RemoveConstVolatileReferenceType<T>> make_owner(T&& r) {
        using U = RemoveConstVolatileReferenceType<T>;
        return stdx::mem::make_shared<U>(stdx::util::forward<T>(r));
    }
public:
    template <typename T>
    using QueryFrom = ConditionalType<
        IsLvalueReferenceValue<T>,
        Query<T>,
        Query<T&>
    >;

    /**
     * @brief Constructs a Query from a range (handles both lvalue and rvalue).
     * @tparam R Deduced type (should be Ran or Ran&).
     * @param r The range to query.
     */
    template <typename R>
        requires stdx::core::SameAs<RemoveConstVolatileReferenceType<R>, RemoveConstVolatileReferenceType<Ran>>
    constexpr explicit Query(R&& r) noexcept:
        range{stdx::util::forward<R>(r)} {}

    /**
     * @brief Constructs a Query from an object and marks it as owned.
     * @tparam T The type of the object.
     * @param t The object to query.
     * @param owned TrueType to indicate ownership.
     */
    template <typename T>
    constexpr explicit Query(T&& t, TrueType):
        owner{make_owner(stdx::util::forward<T>(t))},
        range{*stdx::mem::static_pointer_cast<RemoveConstVolatileReferenceType<T>>(owner)} {}

    /**
     * @brief Returns an iterator to the beginning of the range.
     *
     * @return Iterator to the beginning.
     */
    [[nodiscard]]
    constexpr auto begin() noexcept -> decltype(Begin(range)) {
        return Begin(range);
    }

    /**
     * @brief Returns an iterator to the end of the range.
     *
     * @return Iterator to the end.
     */
    [[nodiscard]]
    constexpr auto end() noexcept -> decltype(End(range)) {
        return End(range);
    }

    /**
     * @brief Filters the range using a predicate.
     *
     * @tparam Pred The predicate type.
     * @param pred The predicate function.
     * @return A Query representing the filtered range.
     */
    template <typename Pred>
        requires Filterable<Ran, Pred>
    [[nodiscard]]
    constexpr auto where(Pred&& pred) noexcept -> Query<decltype(Filter(range, stdx::util::forward<Pred>(pred)))> {
        auto newRange = Filter(range, stdx::util::forward<Pred>(pred));
        Query<decltype(newRange)> q(stdx::util::move(newRange));
        q.owner = owner;
        return q;
    }

    /**
     * @brief Transforms the range using a function.
     *
     * @tparam Func The transformation function type.
     * @param func The transformation function.
     * @return A Query representing the transformed range.
     */
    template <typename Func>
        requires Transformable<Ran, Func>
    [[nodiscard]]
    constexpr auto select(Func&& func) noexcept -> Query<decltype(Transform(range, stdx::util::forward<Func>(func)))> {
        auto newRange = Transform(range, stdx::util::forward<Func>(func));
        Query<decltype(newRange)> q(stdx::util::move(newRange));
        q.owner = owner;
        return q;
    }

    /**
     * @brief Skips the first n elements of the range.
     *
     * @param n The number of elements to skip.
     * @return A Query representing the range after skipping.
     */
    [[nodiscard]]
    constexpr auto skip(RangeDifference<Ran> n) noexcept -> Query<decltype(Drop(range, n))> {
        auto newRange = Drop(range, n);
        Query<decltype(newRange)> q(stdx::util::move(newRange));
        q.owner = owner;
        return q;
    }

    /**
     * @brief Takes the first n elements of the range.
     *
     * @param n The number of elements to take.
     * @return A Query representing the range after taking.
     */
    [[nodiscard]]
    constexpr auto take(RangeDifference<Ran> n) noexcept -> Query<decltype(Take(range, n))> {
        auto newRange = Take(range, n);
        Query<decltype(newRange)> q(stdx::util::move(newRange));
        q.owner = owner;
        return q;
    }

    /**
     * @brief Takes elements from the range while the predicate is true.
     *
     * @tparam Pred The predicate type.
     * @param pred The predicate function.
     * @return A Query representing the range with elements taken while the predicate is satisfied.
     */
    template <typename Pred>
    [[nodiscard]]
    constexpr auto take_while(Pred&& pred) noexcept -> Query<decltype(TakeWhile(range, stdx::util::forward<Pred>(pred)))> {
        auto newRange = TakeWhile(range, stdx::util::forward<Pred>(pred));
        Query<decltype(newRange)> q(stdx::util::move(newRange));
        q.owner = owner;
        return q;
    }

    /**
     * @brief Sorts the range based on a projection.
     * 
     * @tparam KeySelector The key selector type.
     * @tparam Collection The container type (defaults to Vector).
     * @param keySelector The key selector.
     * @return A query representing the range after ordering.
     */
    template <typename KeySelector, typename Collection = Vector<RangeValue<Ran>>>
        requires Sortable<Ran, KeySelector, Collection>
    [[nodiscard]]
    constexpr auto order_by(KeySelector&& keySelector) noexcept -> Query<Collection> {
        Collection temp(begin(), end());
        stdx::ranges::sort(temp, Less<>{}, stdx::util::forward<KeySelector>(keySelector));
        return Query<Collection>(stdx::util::move(temp), TrueType{});
    }

    /**
     * @brief Removes duplicates.
     * 
     * @tparam Collection The container type (defaults to Vector).
     * @return A query with all duplicates removed.
     */
    template <Distinguishable<Ran> Collection = Vector<RangeValue<Ran>>>
    [[nodiscard]]
    constexpr auto distinct() noexcept -> Query<Collection> {
        Collection temp(begin(), end());
        stdx::ranges::sort(temp);
        auto last = stdx::ranges::unique(temp);
        temp.erase(last.begin(), temp.end());
        return Query<Collection>(stdx::util::move(temp), TrueType{});
    }

    /**
     * @brief Returns the first element.
     * 
     * @return The first element.
     *
     * @throws InvalidOperationException if the range is empty.
     */
    [[nodiscard]]
    constexpr RangeValue<Ran> first() throws (InvalidOperationException) {
        if (begin() == end()) {
            throw InvalidOperationException("Range is empty.");
        }
        return *begin();
    }
    
    /**
     * @brief Returns the first element, or a default value if empty.
     * 
     * @param default_value The default value to return if empty.
     * @return The first element.
     */
    [[nodiscard]]
    constexpr RangeValue<Ran> first_or(RangeValue<Ran> default_value = {}) noexcept {
        return (begin() == end()) ? default_value : *begin();
    }

    /**
     * @brief Checks if there is at least one element in the range.
     * 
     * @return True if there is at least one element, false otherwise.
     */
    [[nodiscard]]
    constexpr bool any() noexcept {
        return begin() != end();
    }

    /**
     * @brief Checks if any element of the range satisfies the predicate.
     * 
     * @tparam Pred The predicate type.
     * @param pred The predicate function.
     * @return True if Pred evalutes to true for any element of the range, false otherwise.
     */
    template <typename Pred>
        requires AnyOfQueryable<Ran, Pred>
    [[nodiscard]]
    constexpr bool any(Pred&& pred) const noexcept {
        return stdx::ranges::any_of(range, stdx::util::forward<Pred>(pred));
    }

    /**
     * @brief Checks if all elements of the range satisfy the predicate.
     * 
     * @tparam Pred The predicate type.
     * @param pred The predicate function.
     * @return True if Pred evalutes to true for all elements of the range, false otherwise.
     */
    template <typename Pred>
        requires AllOfQueryable<Ran, Pred>
    [[nodiscard]]
    constexpr bool all(Pred&& pred) const noexcept {
        return stdx::ranges::all_of(range, stdx::util::forward<Pred>(pred));
    }

    /**
     * @brief Counts the number of elements in a range.
     * 
     * @return The number of elements in the range.
     */
    [[nodiscard]]
    constexpr usize count() noexcept {
        return stdx::ranges::distance(begin(), end());
    }

    /**
     * @brief Counts the number of elements satisfying a predicate in a range.
     * 
     * @tparam Pred The predicate type.
     * @param pred The predicate function.
     * @return The number of elements satisfying the predicate.
     */
    template <typename Pred>
        requires CountIfQueryable<Ran, Pred>
    [[nodiscard]]
    constexpr usize count(Pred&& pred) const noexcept {
        return stdx::ranges::count_if(range, stdx::util::forward<Pred>(pred));
    }

    /**
     * @brief Reverses the range.
     * 
     * @return The reversed range.
     */
    [[nodiscard]]
    constexpr auto reverse() noexcept -> Query<decltype(Reverse(range))> {
        auto newRange = Reverse(range);
        Query<decltype(newRange)> q(stdx::util::move(newRange));
        q.owner = owner;
        return q;
    }

    /**
     * @brief Expect exactly one element.
     * 
     * @return Returns the sole single element in the range.
     * 
     * @throws InvalidOperationException if the range has zero or more than one element.
     */
    [[nodiscard]]
    constexpr RangeValue<Ran> single() throws (InvalidOperationException) {
        auto it = begin();
        if (it == end()) {
            throw InvalidOperationException("Range is empty.");
        }
        RangeValue<Ran> value = *it;
        if (++it != end()) {
            throw InvalidOperationException("More than one element in range.");
        }
        return value;
    }

    /**
     * @brief Expect exactly one element in the range satisfying the predicate.
     * 
     * @tparam Pred The predicate type.
     * @param pred The predicate function.
     * @return Returns the sole single element 
     *
     * @throws InvalidOperationException if the range has zero or more than one element satisfying the predicate.
     */
    template <typename Pred>
    [[nodiscard]]
    constexpr RangeValue<Ran> single(Pred&& pred) const throws (InvalidOperationException) {
        Optional<RangeValue<Ran>> found;
        for (auto&& element: range) {
            if (pred(element)) {
                if (found.has_value()) {
                    throw InvalidOperationException("More than one element matches predicate in range.");
                }
                found = element;
            }
        }
        if (!found.has_value()) {
            throw InvalidOperationException("No elements match predicate in range.");
        }
        return *found;
    }

    /**
     * @brief Expect exactly one element in the range satisfying the predicate, otherwise returns default value.
     * 
     * @tparam Pred The predicate type.
     * @param pred The predicate function.
     * @param default_value The default value to return otherwise.
     * @return Returns the sole single element, or the default value.
     */
    template <typename Pred>
    [[nodiscard]]
    constexpr RangeValue<Ran> single(Pred&& pred, RangeValue<Ran> default_value = {}) const noexcept {
        Optional<RangeValue<Ran>> found;
        for (auto&& element: range) {
            if (pred(element)) {
                if (found.has_value()) {
                    return default_value;
                }
                found = element;
            }
        }
        if (!found.has_value()) {
            throw default_value;
        }
        return *found;
    }

    /**
     * @brief Converts the range to a Collection.
     *
     * @tparam Collection The collection type (defaults to Vector).
     * @return The range as a collection.
     */
    template <PushBackable<RangeValue<Ran>> Collection = Vector<RangeValue<Ran>>>
    [[nodiscard]]
    Collection to() {
        Collection result;
        for (auto&& elem : range) {
            result.push_back(stdx::util::forward<decltype(elem)>(elem));
        }
        return result;
    }

    /**
     * @brief Converts the range to a collection (template template parameter version).
     *
     * @tparam Collection The collection template (e.g., Vector).
     * @return The range as a collection with deduced element type.
     */
    template <template<typename...> class Collection>
        requires PushBackable<Collection<RangeValue<Ran>>, RangeValue<Ran>>
    [[nodiscard]]
    Collection<RangeValue<Ran>> to() {
        Collection<RangeValue<Ran>> result;
        for (auto&& elem : range) {
            result.push_back(stdx::util::forward<decltype(elem)>(elem));
        }
        return result;
    }

    /**
     * @brief Creates a Query from an object or range.
     *
     * @tparam T The type of the object or range.
     * @param t The object or range.
     * @return A Query for the given object or range.
     */
    template <typename T>
    [[nodiscard]]
    static constexpr auto from(T&& t) noexcept {
        if constexpr (IsLvalueReferenceValue<T>) {
            return Query<T>(stdx::util::forward<T>(t));
        } else {
            return Query<RemoveConstVolatileReferenceType<T>>(stdx::util::forward<T>(t));
        }
    }
};

// Deduction guides for Query CTAD
template <typename T>
Query(T&) -> Query<T&>;

template <typename T>
Query(T&&) -> Query<RemoveConstVolatileReferenceType<T>>;

}
