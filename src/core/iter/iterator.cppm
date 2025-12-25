/**
 * @file iterator.cppm
 * @module core:iter.iterator
 * @brief Module file for standard library iterator operations.
 *
 * This file contains the implementation of the iterator operations in the standard library.
 */

module;

#include <iterator>

export module core:iter.iterator;

import :concepts;
import :functional;
import :meta.type_traits;

using core::Identity;
using core::Semiregular;
using core::meta::ConditionalType;

namespace {
    template <typename T>
    concept Has_iterator = requires {
        typename T::iterator;
    };

    template <typename T>
    concept Has_Iterator = requires {
        typename T::Iterator;
    };
}

/**
 * @namespace core::iter
 * @brief Wrapper namespace for standard library iterator operations.
 */
export namespace core::iter {
    template <typename In>
    concept IndirectlyReadable = std::indirectly_readable<In>;

    template <typename Out, typename T>
    concept IndirectlyWritable = std::indirectly_writable<Out, T>;

    template <typename Iter>
    concept WeaklyIncrementable = std::weakly_incrementable<Iter>;

    template <typename Iter>
    concept Incrementable = std::incrementable<Iter>;

    template <typename Iter>
    concept InputOrOutputIterator = std::input_or_output_iterator<Iter>;

    template <typename Sent, typename Iter>
    concept SentinelFor = std::sentinel_for<Sent, Iter>;

    template <typename Sent, typename Iter>
    concept SizedSentinelFor = std::sized_sentinel_for<Sent, Iter>;

    template <typename Iter>
    concept InputIterator = std::input_iterator<Iter>;

    template <typename Iter, typename T>
    concept OutputIterator = std::output_iterator<Iter, T>;

    template <typename Iter>
    concept ForwardIterator = std::forward_iterator<Iter>;

    template <typename Iter>
    concept BidirectionalIterator = std::bidirectional_iterator<Iter>;

    template <typename Iter>
    concept RandomAccessIterator = std::random_access_iterator<Iter>;

    template <typename Iter>
    concept ContiguousIterator = std::contiguous_iterator<Iter>;

    template <typename Fn, typename Iter>
    concept IndirectlyUnaryInvocable = std::indirectly_unary_invocable<Fn, Iter>;

    template <typename Fn, typename Iter>
    concept IndirectlyRegularUnaryInvocable = std::indirectly_regular_unary_invocable<Fn, Iter>;

    template <typename Fn, typename Iter>
    concept IndirectUnaryPredicate = std::indirect_unary_predicate<Fn, Iter>;

    template <typename Fn, typename I1, typename I2>
    concept IndirectBinaryPredicate = std::indirect_binary_predicate<Fn, I1, I2>;

    template <typename Fn, typename I1, typename I2 = I1>
    concept IndirectEquivalenceRelation = std::indirect_equivalence_relation<Fn, I1, I2>;

    template <typename Fn, typename I1, typename I2 = I1>
    concept IndirectStrictWeakOrder = std::indirect_strict_weak_order<Fn, I1, I2>;

    template <typename In, typename Out>
    concept IndirectlyMovable = std::indirectly_movable<In, Out>;

    template <typename In, typename Out>
    concept IndirectlyMovableStorable = std::indirectly_movable_storable<In, Out>;

    template <typename In, typename Out>
    concept IndirectlyCopyable = std::indirectly_copyable<In, Out>;

    template <typename In, typename Out>
    concept IndirectlyCopyableStorable = std::indirectly_copyable_storable<In, Out>;

    template <typename I1, typename I2 = I1>
    concept IndirectlySwappable = std::indirectly_swappable<I1, I2>;

    template <typename I1, typename I2, typename Comp, typename Proj1 = Identity, typename Proj2 = Identity>
    concept IndirectlyComparable = std::indirectly_comparable<I1, I2, Comp, Proj1, Proj2>;

    template <typename Iter>
    concept Permutable = std::permutable<Iter>;

    template <typename I1, typename I2, typename Out, typename Comp = std::ranges::less, typename Proj1 = Identity, typename Proj2 = Identity>
    concept Mergeable = std::mergeable<I1, I2, Out, Comp, Proj1, Proj2>;

    template <typename Iter, typename Comp = std::ranges::less, typename Proj = Identity>
    concept Sortable = std::sortable<Iter, Comp, Proj>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename Fn, typename... Is>
    using IndirectResult_t = std::indirect_result_t<Fn, Is>;
    #endif

    template <typename Fn, typename... Is>
    using IndirectResult = std::indirect_result_t<Fn, Is...>;

    template <IndirectlyReadable Iter, IndirectlyRegularUnaryInvocable<Iter> Proj>
    using Projected = std::projected<Iter, Proj>;

    template <typename T>
    using IncrementableTraits = std::incrementable_traits<T>;

    template <typename T>
    using IndirectlyReadableTraits = std::indirectly_readable_traits<T>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename T>
    using IterValue = std::iter_value_t<T>;

    template <typename T>
    using IterReference = std::iter_reference_t<T>;

    template <IndirectlyReadable T>
    using IterConstReference = std::iter_const_reference_t<T>;

    template <typename T>
    using IterDifference = std::iter_difference_t<T>;

    template <typename T>
    using IterRValueReference = std::iter_rvalue_reference_t<T>

    template <IndirectlyReadable T>
    using IterCommonReference = std::iter_common_reference_t<T>;

    template <typename T>
    using IterValue_t = std::iter_value_t<T>;

    template <typename T>
    using IterReference_t = std::iter_reference_t<T>;

    template <IndirectlyReadable T>
    using IterConstReference_t = std::iter_const_reference_t<T>;

    template <typename T>
    using IterDifference_t = std::iter_difference_t<T>;

    template <typename T>
    using IterRValueReference_t = std::iter_rvalue_reference_t<T>

    template <IndirectlyReadable T>
    using IterCommonReference_t = std::iter_common_reference_t<T>;

    template <typename T>
    using IteratorValue_t = std::iter_value_t<T>;

    template <typename T>
    using IteratorReference_t = std::iter_reference_t<T>;

    template <IndirectlyReadable T>
    using IteratorConstReference_t = std::iter_const_reference_t<T>;

    template <typename T>
    using IteratorDifference_t = std::iter_difference_t<T>;

    template <typename T>
    using IteratorRValueReference_t = std::iter_rvalue_reference_t<T>

    template <IndirectlyReadable T>
    using IteratorCommonReference_t = std::iter_common_reference_t<T>;
    #endif

    template <typename T>
    using IteratorValue = std::iter_value_t<T>;

    template <typename T>
    using IteratorReference = std::iter_reference_t<T>;

    template <IndirectlyReadable T>
    using IteratorConstReference = std::iter_const_reference_t<T>;

    template <typename T>
    using IteratorDifference = std::iter_difference_t<T>;

    template <typename T>
    using IteratorRValueReference = std::iter_rvalue_reference_t<T>;

    template <IndirectlyReadable T>
    using IteratorCommonReference = std::iter_common_reference_t<T>;

    template <typename Iter>
    using IteratorTraits = std::iterator_traits<Iter>;

    using InputIteratorTag = std::input_iterator_tag;
    using OutputIteratorTag = std::output_iterator_tag;
    using ForwardIteratorTag = std::forward_iterator_tag;
    using BidirectionalIteratorTag = std::bidirectional_iterator_tag;
    using RandomAccessIteratorTag = std::random_access_iterator_tag;
    using ContiguousIteratorTag = std::contiguous_iterator_tag;

    template <typename Category, typename T, typename Distance = std::ptrdiff_t, typename Pointer = T*, typename Reference = T&>
    using IIterator = std::iterator<Category, T, Distance, Pointer, Reference>;

    template <typename Iterable>
        requires Has_iterator<Iterable> || Has_Iterator<Iterable>
    using Iterator = ConditionalType<
        Has_Iterator<Iterable>,
        typename Iterable::Iterator,
        typename Iterable::iterator
    >;

    template <typename Iter>
    using ReverseIterator = std::reverse_iterator<Iter>;

    template <typename Iter>
    using MoveIterator = std::move_iterator<Iter>;

    template <Semiregular Sent>
    using MoveSentinel = std::move_sentinel<Sent>;

    template <InputIterator Iter>
    using BasicConstIterator = std::basic_const_iterator<Iter>;

    template <InputIterator Iter>
    using ConstIterator = std::const_iterator<Iter>;

    template <Semiregular Sent>
    using ConstSentinel = std::const_sentinel<Sent>;

    template <InputOrOutputIterator Iter, SentinelFor<Iter> Sent>
    using CommonIterator = std::common_iterator<Iter, Sent>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using DefaultSentinel_t = std::default_sentinel_t;
    using UnreachableSentinel_t = std::unreachable_sentinel_t;
    #endif

    using DefaultSentinel = std::default_sentinel_t;
    using UnreachableSentinel = std::unreachable_sentinel_t;

    template <InputOrOutputIterator Iter>
    using CountedIterator = std::counted_iterator<Iter>;

    template <typename Container>
    using BackInsertIterator = std::back_insert_iterator<Container>;

    template <typename Container>
    using FrontInsertIterator = std::front_insert_iterator<Container>;

    template <typename Container>
    using InsertIterator = std::insert_iterator<Container>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename T, typename CharT = char, typename Traits = std::char_traits<CharT>, typename Distance = std::ptrdiff_t>
    using IStreamIterator = std::istream_iterator<T, CharT, Traits, Distance>;

    template <typename T, typename CharT = char, typename Traits = std::char_traits<CharT>>
    using OStreamIterator = std::ostream_iterator<T, CharT, Traits, Distance>;

    template <typename CharT, typename Traits = std::char_traits<CharT>>
    using IStreamBufIterator = std::istreambuf_iterator<CharT, Traits>;

    template <typename CharT, typename Traits = std::char_traits<CharT>>
    using OStreamBufIterator = std::ostreambuf_iterator<CharT, Traits>;
    #endif

    template <typename T, typename CharT = char, typename Traits = std::char_traits<CharT>, typename Distance = std::ptrdiff_t>
    using InputStreamIterator = std::istream_iterator<T, CharT, Traits, Distance>;

    template <typename T, typename CharT = char, typename Traits = std::char_traits<CharT>>
    using OutputStreamIterator = std::ostream_iterator<T, CharT, Traits>;

    template <typename CharT, typename Traits = std::char_traits<CharT>>
    using InputStreamBufferIterator = std::istreambuf_iterator<CharT, Traits>;

    template <typename CharT, typename Traits = std::char_traits<CharT>>
    using OutputStreamBufferIterator = std::ostreambuf_iterator<CharT, Traits>;

    inline constexpr DefaultSentinel DEFAULT_SENTINEL = std::default_sentinel;
    inline constexpr UnreachableSentinel UNREACHABLE_SENTINEL = std::unreachable_sentinel;

    using std::make_reverse_iterator;
    using std::make_move_iterator;
    using std::make_const_iterator;
    using std::make_const_sentinel;
    using std::front_inserter;
    using std::back_inserter;
    using std::inserter;

    using std::operator==;
    using std::operator<;
    using std::operator<=;
    using std::operator>;
    using std::operator>=;
    using std::operator<=>;
    using std::operator+;
    using std::operator-;
    
    using std::advance;
    using std::distance;
    using std::next;
    using std::prev;
    
    using std::begin;
    using std::cbegin;
    using std::end;
    using std::cend;
    using std::rbegin;
    using std::crbegin;
    using std::rend;
    using std::crend;
    using std::size;
    using std::ssize;
    using std::data;

    namespace ranges {
        using std::ranges::advance;
        using std::ranges::distance;
        using std::ranges::next;
        using std::ranges::prev;

        using std::ranges::iter_move;
        using std::ranges::iter_swap;
    }
}
