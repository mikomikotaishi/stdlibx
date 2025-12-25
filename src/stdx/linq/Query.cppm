/**
 * @file Query.cppm
 * @module stdx:linq.Query
 * @brief Implementation of the Query class.
 *
 * This file contains the implementation of the Query class, which is used to perform 
 * LINQ-like queries on enumerable collection types.
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:linq.Query;
#else
export module stdlibx:linq.Query;
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
import std;
#else
import stdlib;
#endif

import :core;

#ifdef STDLIBX_NO_RESERVED_STD_NAMESPACE
using std::collections::Vector;
using std::mem::SharedPointer;
using std::meta::ConditionalType;
using std::meta::IsLvalueReferenceValue;
using std::meta::RemoveConstVolatileReferenceType;
using std::meta::TrueType;
using std::ranges::Begin;
using std::ranges::End;
using std::ranges::ForwardRange;
using std::ranges::RangeDifference;
using std::ranges::RangeValue;
using std::ranges::ViewableRange;

namespace mem = std::mem;
namespace ranges = std::ranges;
namespace util = std::util;

using namespace std::ranges::views;
#else
using stdlib::collections::Vector;
using stdlib::mem::SharedPointer;
using stdlib::meta::ConditionalType;
using stdlib::meta::IsLvalueReferenceValue;
using stdlib::meta::RemoveConstVolatileReferenceType;
using stdlib::meta::TrueType;
using stdlib::ranges::Begin;
using stdlib::ranges::End;
using stdlib::ranges::ForwardRange;
using stdlib::ranges::RangeDifference;
using stdlib::ranges::RangeValue;
using stdlib::ranges::ViewableRange;

namespace mem = stdlib::mem;
namespace ranges = stdlib::ranges;
namespace util = stdlib::util;

using namespace stdlib::ranges::views;
#endif

/**
 * @namespace stdx::linq
 * @brief Wrapper namespace for standard library extension LINQ libraries.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::linq {
#else
export namespace stdlibx::linq {
#endif

/**
 * @concept Filterable
 * @brief Concept that checks if Filter can be applied to the range.
 * 
 * @tparam Ran The range type.
 * @tparam Pred The predicate type.
 */
template <typename Ran, typename Pred>
concept Filterable = requires (const Ran& r, Pred&& p) {
    { Filter(r, util::forward<Pred>(p)) };
};

/**
 * @concept Transformable
 * @brief Concept that checks if Transform can be applied to the range.
 * 
 * @tparam Ran The range type.
 * @tparam Func The function type.
 */
template <typename Ran, typename Func>
concept Transformable = requires (const Ran& r, Func&& f) {
    { Transform(r, util::forward<Func>(f)) };
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
    { ranges::sort(c, Less<>{}, util::forward<KeySelector>(k)) };
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
    requires (Collection c) { { ranges::sort(c) }; } &&
    requires (Collection c) { { ranges::unique(c) }; };

/**
 * @concept AnyOfQueryable
 * @brief Concept that checks if any_of() can be applied to the range.
 * 
 * @tparam Ran The range type.
 * @tparam Pred The predicate type.
 */
template <typename Ran, typename Pred>
concept AnyOfQueryable = requires (const Ran& r, Pred&& p) {
    { ranges::any_of(r, util::forward<Pred>(p)) };
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
    { ranges::all_of(r, util::forward<Pred>(p)) };
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
    { ranges::count_if(r, util::forward<Pred>(p)) };
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
        return mem::make_shared<U>(util::forward<T>(r));
    }
public:
    template <typename T>
    using QueryFrom = ConditionalType<
        IsLvalueReferenceValue<T>,
        Query<T>,
        Query<T&>
    >;

    /**
     * @brief Constructs a Query from an lvalue range.
     * @param r The range to query.
     */
    constexpr explicit Query(Ran& r) noexcept:
        range{r} {}

    /**
     * @brief Constructs a Query from an rvalue range.
     * @param r The range to query.
     */
    constexpr explicit Query(Ran&& r) noexcept:
        range{util::forward<Ran>(r)} {}

    /**
     * @brief Constructs a Query from an object and marks it as owned.
     * @tparam T The type of the object.
     * @param t The object to query.
     * @param owned TrueType to indicate ownership.
     */
    template <typename T>
    constexpr explicit Query(T&& t, TrueType):
        owner{make_owner(util::forward<T>(t))},
        range{*mem::static_pointer_cast<RemoveConstVolatileReferenceType<T>>(owner)} {}

    /**
     * @brief Returns an iterator to the beginning of the range.
     *
     * @return Iterator to the beginning.
     */
    [[nodiscard]]
    constexpr ranges::access::Begin begin() const noexcept {
        return Begin(range);
    }

    /**
     * @brief Returns an iterator to the end of the range.
     *
     * @return Iterator to the end.
     */
    [[nodiscard]]
    constexpr ranges::access::End end() const noexcept {
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
    constexpr auto where(Pred&& pred) noexcept -> Query<decltype(Filter(range, util::forward<Pred>(pred)))> {
        auto newRange = Filter(range, util::forward<Pred>(pred));
        Query<decltype(newRange)> q(util::move(newRange));
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
    constexpr auto select(Func&& func) noexcept -> Query<decltype(Transform(range, util::forward<Func>(func)))> {
        auto newRange = Transform(range, util::forward<Func>(func));
        Query<decltype(newRange)> q(util::move(newRange));
        q.owner = owner;
        return q;
    }

    /**
     * @brief Skips the first n elements of the range.
     *
     * @param n The number of elements to skip.
     * @return A Query representing the range after skipping.
     */
    constexpr auto skip(RangeDifference<Ran> n) noexcept -> Query<decltype(Drop(range, n))> {
        auto newRange = Drop(range, n);
        Query<decltype(newRange)> q(util::move(newRange));
        q.owner = owner;
        return q;
    }

    /**
     * @brief Takes the first n elements of the range.
     *
     * @param n The number of elements to take.
     * @return A Query representing the range after taking.
     */
    constexpr auto take(RangeDifference<Ran> n) noexcept -> Query<decltype(Take(range, n))> {
        auto newRange = Take(range, n);
        Query<decltype(newRange)> q(util::move(newRange));
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
    constexpr auto order_by(KeySelector&& keySelector) noexcept -> Query<Collection> {
        Collection temp(begin(), end());
        ranges::sort(temp, Less<>{}, util::forward<KeySelector>(keySelector));
        return Query<Collection>(util::move(temp), TrueType{});
    }

    /**
     * @brief Removes duplicates.
     * 
     * @tparam Collection The container type (defaults to Vector).
     * @return A query with all duplicates removed.
     */
    template <typename Collection = Vector<RangeValue<Ran>>>
        requires Distinguishable<Ran, Collection>
    constexpr auto distinct() noexcept -> Query<Collection> {
        Collection temp(begin(), end());
        ranges::sort(temp);
        auto last = ranges::unique(temp);
        temp.erase(last.begin(), temp.end());
        return Query<Collection>(util::move(temp), TrueType{});
    }

    /**
     * @brief Returns the first element.
     * 
     * @return The first element.
     *
     * @throws InvalidOperationException if the range is empty.
     */
    constexpr RangeValue<Ran> first() const throws (InvalidOperationException) {
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
    constexpr RangeValue<Ran> first_or(RangeValue<Ran> default_value = {}) const noexcept {
        return (begin() == end()) ? default_value : *begin();
    }

    /**
     * @brief Checks if there is at least one element in the range.
     * 
     * @return True if there is at least one element, false otherwise.
     */
    constexpr bool any() const noexcept {
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
    constexpr bool any(Pred&& pred) const noexcept {
        return ranges::any_of(range, util::forward<Pred>(pred));
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
    constexpr bool all(Pred&& pred) const noexcept {
        return ranges::all_of(range, util::forward<Pred>(pred));
    }

    /**
     * @brief Counts the number of elements in a range.
     * 
     * @return The number of elements in the range.
     */
    constexpr usize count() const noexcept {
        return Distance(begin(), end());
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
    constexpr usize count(Pred&& pred) const noexcept {
        return ranges::count_if(range, util::forward<Pred>(pred));
    }

    /**
     * @brief Reverses the range.
     * 
     * @return The reversed range.
     */
    constexpr auto reverse() noexcept -> Query<decltype(Reverse(range))> {
        auto newRange = Reverse(range);
        Query<decltype(newRange)> q(util::move(newRange));
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
    constexpr RangeValue<Ran> single() const throws (InvalidOperationException) {
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
     * @brief Converts the range to a container.
     *
     * @tparam Collection The container type (defaults to Vector).
     * @return The range as a container.
     */
    template <typename Collection = Vector<RangeValue<Ran>>>
    Collection to() const {
        return Collection(begin(), end());
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
    static constexpr QueryFrom<T> from(T&& t) noexcept {
        if constexpr (IsLvalueReferenceValue<T>) {
            return Query(util::forward<T>(t));
        } else {
            return Query<T>(util::forward<T>(t));
        }
    }
};

}
