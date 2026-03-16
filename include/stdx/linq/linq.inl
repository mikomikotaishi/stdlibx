#pragma once

using stdx::collections::Vector;
using stdx::meta::IsLvalueReferenceValue;
using stdx::meta::RemoveConstVolatileReferenceType;
using stdx::ranges::Begin;
using stdx::ranges::End;
using stdx::ranges::EmptyView;
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
    { Filter(r, System::forward<Pred>(p)) };
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
    { Transform(r, System::forward<Func>(f)) };
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
    { stdx::ranges::sort(c, Less<>{}, System::forward<KeySelector>(k)) };
};

/**
 * @concept Distinguishable
 * @brief Concept that checks if unique() can be applied to the range.
 *
 * @tparam Ran The range type.
 * @tparam Collection The collection type (defaults to Vector).
 */
template <typename Collection, typename Ran>
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
    { stdx::ranges::any_of(r, System::forward<Pred>(p)) };
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
    { stdx::ranges::all_of(r, System::forward<Pred>(p)) };
};

/**
 * @concept NoneOfQueryable
 * @brief Concept that checks if none_of() can be applied to the range.
 *
 * @tparam Ran The range type.
 * @tparam Pred The predicate type.
 */
template <typename Ran, typename Pred>
concept NoneOfQueryable = requires (const Ran& r, Pred&& p) {
    { stdx::ranges::none_of(r, System::forward<Pred>(p)) };
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
    { stdx::ranges::count_if(r, System::forward<Pred>(p)) };
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
    { c.push_back(System::forward<T>(elem)) };
};

/**
 * @concept Reversible
 * @brief Concept that checks if Reverse can be applied to the range.
 *
 * @tparam Ran The range type.
 */
template <typename Ran>
concept Reversible = requires (Ran& r) {
    { Reverse(r) };
};

/**
 * @concept KeysAccessible
 * @brief Concept that checks if Keys can be applied to the range.
 *
 * @tparam Ran The range type.
 */
template <typename Ran>
concept KeysAccessible = requires (Ran& r) {
    { Keys(r) };
};

/**
 * @concept ValuesAccessible
 * @brief Concept that checks if Values can be applied to the range.
 *
 * @tparam Ran The range type.
 */
template <typename Ran>
concept ValuesAccessible = requires (Ran& r) {
    { Values(r) };
};

/**
 * @concept ElementsAccessible
 * @brief Concept that checks if Elements<N> can be applied to the range.
 *
 * @tparam N The element index.
 * @tparam Ran The range type.
 */
template <usize N, typename Ran>
concept ElementsAccessible = requires (Ran& r) {
    { Elements<N>(r) };
};

/**
 * @concept Joinable
 * @brief Concept that checks if Join can be applied to the range.
 *
 * @tparam Ran The range type.
 */
template <typename Ran>
concept Joinable = requires (Ran& r) {
    { Join(r) };
};

#ifdef __cpp_lib_ranges_fold
/**
 * @concept Foldable
 * @brief Concept that checks if fold_left() can be applied to the range.
 *
 * @tparam Ran The range type.
 * @tparam Init The initial value type.
 * @tparam Func The fold function type.
 */
template <typename Ran, typename Init, typename Func>
concept Foldable = requires (Ran& r, Init&& init, Func&& f) {
    { stdx::ranges::fold_left(r, System::forward<Init>(init), System::forward<Func>(f)) };
};
#endif

/**
 * @class Query
 * @brief Class for representing LINQ-like queries of collection types.
 *
 * @tparam Ran A ViewableRange type.
 *
 * The Query class provides LINQ-style operations for any range type that satisfies the ViewableRange concept.
 */
template <ViewableRange Ran = EmptyView<Monostate>>
class Query {
private:
    Ran range; ///< The underlying range being queried.
public:
    /**
     * @brief Constructs a Query from a range (handles both lvalue and rvalue).
     * @tparam R Deduced type (should be Ran or Ran&).
     * @param r The range to query.
     */
    template <typename R>
        requires SameAs<RemoveConstVolatileReferenceType<R>, RemoveConstVolatileReferenceType<Ran>>
    constexpr explicit Query(R&& r) noexcept:
        range{System::forward<R>(r)} {}

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
    constexpr auto where(Pred&& pred) noexcept -> Query<decltype(Filter(range, System::forward<Pred>(pred)))> {
        return Query<decltype(Filter(range, System::forward<Pred>(pred)))>(
            Filter(range, System::forward<Pred>(pred))
        );
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
    constexpr auto select(Func&& func) noexcept -> Query<decltype(Transform(range, System::forward<Func>(func)))> {
        return Query<decltype(Transform(range, System::forward<Func>(func)))>(
            Transform(range, System::forward<Func>(func))
        );
    }

    /**
     * @brief Transforms and flattens the range (flat map / SelectMany).
     *
     * Each element is transformed by func into a range, and the results are flattened into a single sequence.
     *
     * @tparam Func The transformation function type (must return a range).
     * @param func The transformation function.
     * @return A Query representing the flattened, transformed range.
     */
    template <typename Func>
        requires Transformable<Ran, Func> &&
            requires (Ran& r, Func&& f) { Join(Transform(r, System::forward<Func>(f))); }
    [[nodiscard]]
    constexpr auto select_many(Func&& func) noexcept
        -> Query<decltype(Join(Transform(range, System::forward<Func>(func))))> {
        return Query<decltype(Join(Transform(range, System::forward<Func>(func))))>(
            Join(Transform(range, System::forward<Func>(func)))
        );
    }

    /**
     * @brief Skips the first n elements of the range.
     *
     * @param n The number of elements to skip.
     * @return A Query representing the range after skipping.
     */
    [[nodiscard]]
    constexpr auto skip(RangeDifference<Ran> n) noexcept -> Query<decltype(Drop(range, n))> {
        return Query<decltype(Drop(range, n))>(Drop(range, n));
    }

    /**
     * @brief Skips elements from the range while the predicate is true.
     *
     * @tparam Pred The predicate type.
     * @param pred The predicate function.
     * @return A Query representing the range with elements skipped while the predicate is satisfied.
     */
    template <typename Pred>
    [[nodiscard]]
    constexpr auto skip_while(Pred&& pred) noexcept -> Query<decltype(DropWhile(range, System::forward<Pred>(pred)))> {
        return Query<decltype(DropWhile(range, System::forward<Pred>(pred)))>(
            DropWhile(range, System::forward<Pred>(pred))
        );
    }

    /**
     * @brief Takes the first n elements of the range.
     *
     * @param n The number of elements to take.
     * @return A Query representing the range after taking.
     */
    [[nodiscard]]
    constexpr auto take(RangeDifference<Ran> n) noexcept -> Query<decltype(Take(range, n))> {
        return Query<decltype(Take(range, n))>(Take(range, n));
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
    constexpr auto take_while(Pred&& pred) noexcept -> Query<decltype(TakeWhile(range, System::forward<Pred>(pred)))> {
        return Query<decltype(TakeWhile(range, System::forward<Pred>(pred)))>(
            TakeWhile(range, System::forward<Pred>(pred))
        );
    }

    /**
     * @brief Reverses the range.
     *
     * @return The reversed range.
     */
    template <typename R = Ran>
        requires Reversible<R>
    [[nodiscard]]
    constexpr auto reverse() noexcept {
        return Query<decltype(Reverse(range))>(Reverse(range));
    }

    #ifdef __cpp_lib_ranges_enumerate
    /**
     * @brief Pairs each element with its zero-based index.
     *
     * @return A Query of (index, element) pairs.
     */
    [[nodiscard]]
    constexpr auto enumerate() noexcept -> Query<decltype(Enumerate(range))> {
        return Query<decltype(Enumerate(range))>(Enumerate(range));
    }
    #endif

    /**
     * @brief Zips this range with one or more other ranges.
     *
     * @tparam Others The other range types.
     * @param others The other ranges to zip with.
     * @return A Query of tuples containing elements from each range.
     */
    template <typename... Others>
    [[nodiscard]]
    constexpr auto zip(Others&&... others) noexcept
        -> Query<decltype(Zip(range, System::forward<Others>(others)...))> {
        return Query<decltype(Zip(range, System::forward<Others>(others)...))>(
            Zip(range, System::forward<Others>(others)...)
        );
    }

    #ifdef __cpp_lib_ranges_stride
    /**
     * @brief Takes every nth element from the range.
     *
     * @param n The stride length.
     * @return A Query representing every nth element.
     */
    [[nodiscard]]
    constexpr auto stride(RangeDifference<Ran> n) noexcept -> Query<decltype(Stride(range, n))> {
        return Query<decltype(Stride(range, n))>(Stride(range, n));
    }
    #endif

    #ifdef __cpp_lib_ranges_chunk
    /**
     * @brief Splits the range into chunks of n elements.
     *
     * @param n The chunk size.
     * @return A Query of chunks (sub-ranges).
     */
    [[nodiscard]]
    constexpr auto chunk(RangeDifference<Ran> n) noexcept -> Query<decltype(Chunk(range, n))> {
        return Query<decltype(Chunk(range, n))>(Chunk(range, n));
    }
    #endif

    #ifdef __cpp_lib_ranges_slide
    /**
     * @brief Creates a sliding window of n elements over the range.
     *
     * @param n The window size.
     * @return A Query of sliding windows (sub-ranges).
     */
    [[nodiscard]]
    constexpr auto slide(RangeDifference<Ran> n) noexcept -> Query<decltype(Slide(range, n))> {
        return Query<decltype(Slide(range, n))>(Slide(range, n));
    }
    #endif

    /**
     * @brief Extracts keys from a range of pair-like elements.
     *
     * @return A Query over the first element of each pair/tuple.
     */
    template <typename R = Ran>
        requires KeysAccessible<R>
    [[nodiscard]]
    constexpr auto keys() noexcept {
        return Query<decltype(Keys(range))>(Keys(range));
    }

    /**
     * @brief Extracts values from a range of pair-like elements.
     *
     * @return A Query over the second element of each pair/tuple.
     */
    template <typename R = Ran>
        requires ValuesAccessible<R>
    [[nodiscard]]
    constexpr auto values() noexcept {
        return Query<decltype(Values(range))>(Values(range));
    }

    /**
     * @brief Extracts the Nth element from a range of tuple-like elements.
     *
     * @tparam N The element index.
     * @return A Query over the Nth element of each tuple.
     */
    template <usize N>
        requires ElementsAccessible<N, Ran>
    [[nodiscard]]
    constexpr auto elements() noexcept {
        return Query<decltype(Elements<N>(range))>(Elements<N>(range));
    }

    /**
     * @brief Flattens a range of ranges into a single range.
     *
     * @return A Query representing the flattened range.
     */
    template <typename R = Ran>
        requires Joinable<R>
    [[nodiscard]]
    constexpr auto flatten() noexcept {
        return Query<decltype(Join(range))>(Join(range));
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
        stdx::ranges::sort(temp, Less<>{}, System::forward<KeySelector>(keySelector));
        return Query<Collection>(System::move(temp));
    }

    /**
     * @brief Sorts the range in descending order based on a projection.
     *
     * @tparam KeySelector The key selector type.
     * @tparam Collection The container type (defaults to Vector).
     * @param keySelector The key selector.
     * @return A query representing the range after ordering in descending order.
     */
    template <typename KeySelector, typename Collection = Vector<RangeValue<Ran>>>
        requires Sortable<Ran, KeySelector, Collection>
    [[nodiscard]]
    constexpr auto order_by_descending(KeySelector&& keySelector) noexcept -> Query<Collection> {
        Collection temp(begin(), end());
        stdx::ranges::sort(temp, Greater<>{}, System::forward<KeySelector>(keySelector));
        return Query<Collection>(System::move(temp));
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
        return Query<Collection>(System::move(temp));
    }

    /**
     * @brief Concatenates another range to this one.
     *
     * @tparam Other The other range type.
     * @tparam Collection The container type (defaults to Vector).
     * @param other The range to append.
     * @return A Query over the concatenated elements.
     */
    template <typename Other, typename Collection = Vector<RangeValue<Ran>>>
        requires PushBackable<Collection, RangeValue<Ran>>
    [[nodiscard]]
    constexpr auto concat(Other&& other) noexcept -> Query<Collection> {
        Collection temp(begin(), end());
        for (auto&& elem : other) {
            temp.push_back(System::forward<decltype(elem)>(elem));
        }
        return Query<Collection>(System::move(temp));
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
     * @brief Returns the last element.
     *
     * @return The last element.
     *
     * @throws InvalidOperationException if the range is empty.
     */
    [[nodiscard]]
    constexpr RangeValue<Ran> last() throws (InvalidOperationException) {
        auto it = begin();
        auto e = end();
        if (it == e) {
            throw InvalidOperationException("Range is empty.");
        }
        auto last_it = it;
        while (++it != e) {
            last_it = it;
        }
        return *last_it;
    }

    /**
     * @brief Returns the last element, or a default value if empty.
     *
     * @param default_value The default value to return if empty.
     * @return The last element, or the default value.
     */
    [[nodiscard]]
    constexpr RangeValue<Ran> last_or(RangeValue<Ran> default_value = {}) noexcept {
        auto it = begin();
        auto e = end();
        if (it == e) {
            return default_value;
        }
        auto last_it = it;
        while (++it != e) {
            last_it = it;
        }
        return *last_it;
    }

    /**
     * @brief Returns the element at the given index.
     *
     * @param index The zero-based index.
     * @return The element at the given index.
     *
     * @throws InvalidOperationException if the index is out of range.
     */
    [[nodiscard]]
    constexpr RangeValue<Ran> element_at(RangeDifference<Ran> index) throws (InvalidOperationException) {
        auto it = begin();
        auto e = end();
        for (RangeDifference<Ran> i = 0; i < index && it != e; ++i) {
            ++it;
        }
        if (it == e) {
            throw InvalidOperationException("Index out of range.");
        }
        return *it;
    }

    /**
     * @brief Returns the element at the given index, or a default value if out of range.
     *
     * @param index The zero-based index.
     * @param default_value The default value to return if out of range.
     * @return The element at the given index, or the default value.
     */
    [[nodiscard]]
    constexpr RangeValue<Ran> element_at_or(RangeDifference<Ran> index, RangeValue<Ran> default_value = {}) noexcept {
        auto it = begin();
        auto e = end();
        for (RangeDifference<Ran> i = 0; i < index && it != e; ++i) {
            ++it;
        }
        return (it == e) ? default_value : *it;
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
        return stdx::ranges::any_of(range, System::forward<Pred>(pred));
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
        return stdx::ranges::all_of(range, System::forward<Pred>(pred));
    }

    /**
     * @brief Checks if no elements of the range satisfy the predicate.
     *
     * @tparam Pred The predicate type.
     * @param pred The predicate function.
     * @return True if Pred evalutes to false for all elements of the range.
     */
    template <typename Pred>
        requires NoneOfQueryable<Ran, Pred>
    [[nodiscard]]
    constexpr bool none(Pred&& pred) const noexcept {
        return stdx::ranges::none_of(range, System::forward<Pred>(pred));
    }

    /**
     * @brief Checks if the range contains the given value.
     *
     * @tparam T The value type.
     * @param value The value to search for.
     * @return True if the range contains the value.
     */
    template <typename T>
    [[nodiscard]]
    constexpr bool contains(const T& value) const noexcept {
        return stdx::ranges::contains(range, value);
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
        return stdx::ranges::count_if(range, System::forward<Pred>(pred));
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
    constexpr RangeValue<Ran> single(Pred&& pred, RangeValue<Ran> default_value) const noexcept {
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
            return default_value;
        }
        return *found;
    }

    #ifdef __cpp_lib_ranges_fold
    /**
     * @brief Folds the range from the left with an initial value and a binary function.
     *
     * @tparam Init The initial accumulator type.
     * @tparam Func The binary function type.
     * @param init The initial accumulator value.
     * @param func The binary function (accumulator, element) -> accumulator.
     * @return The final accumulated value.
     */
    template <typename Init, typename Func>
        requires Foldable<Ran, Init, Func>
    [[nodiscard]]
    constexpr auto aggregate(Init&& init, Func&& func) const noexcept {
        return stdx::ranges::fold_left(range, System::forward<Init>(init), System::forward<Func>(func));
    }

    /**
     * @brief Computes the sum of the elements.
     *
     * @return The sum of all elements, starting from value-initialized zero.
     */
    [[nodiscard]]
    constexpr auto sum() noexcept {
        return stdx::ranges::fold_left(range, RangeValue<Ran>{}, Plus<>{});
    }
    #endif

    /**
     * @brief Returns the minimum element.
     *
     * @return The minimum element.
     *
     * @throws InvalidOperationException if the range is empty.
     */
    [[nodiscard]]
    constexpr RangeValue<Ran> min() throws (InvalidOperationException) {
        auto it = stdx::ranges::min_element(range);
        if (it == End(range)) {
            throw InvalidOperationException("Range is empty.");
        }
        return *it;
    }

    /**
     * @brief Returns the maximum element.
     *
     * @return The maximum element.
     *
     * @throws InvalidOperationException if the range is empty.
     */
    [[nodiscard]]
    constexpr RangeValue<Ran> max() throws (InvalidOperationException) {
        auto it = stdx::ranges::max_element(range);
        if (it == End(range)) {
            throw InvalidOperationException("Range is empty.");
        }
        return *it;
    }

    /**
     * @brief Applies a function to each element of the range.
     *
     * @tparam Func The function type.
     * @param func The function to apply.
     */
    template <typename Func>
    constexpr void for_each(Func&& func) {
        stdx::ranges::for_each(range, System::forward<Func>(func));
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
            result.push_back(System::forward<decltype(elem)>(elem));
        }
        return result;
    }

    /**
     * @brief Converts the range to a collection (template template parameter version).
     *
     * @tparam Collection The collection template (e.g., Vector).
     * @return The range as a collection with deduced element type.
     */
    template <template <typename...> class Collection>
        requires PushBackable<Collection<RangeValue<Ran>>, RangeValue<Ran>>
    [[nodiscard]]
    Collection<RangeValue<Ran>> to() {
        Collection<RangeValue<Ran>> result;
        for (auto&& elem : range) {
            result.push_back(System::forward<decltype(elem)>(elem));
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
            return Query<T>(System::forward<T>(t));
        } else {
            return Query<RemoveConstVolatileReferenceType<T>>(System::forward<T>(t));
        }
    }
};

// Deduction guides for Query CTAD
template <typename T>
Query(T&) -> Query<T&>;

template <typename T>
Query(T&&) -> Query<RemoveConstVolatileReferenceType<T>>;

}
