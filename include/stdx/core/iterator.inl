#pragma once

using stdx::ranges::Range;
using stdx::ranges::RangeDifference;
using stdx::text::CharTraits;

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
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

    template <typename I1, typename I2, typename Out, typename Comp = Less<>, typename Proj1 = Identity, typename Proj2 = Identity>
    concept Mergeable = std::mergeable<I1, I2, Out, Comp, Proj1, Proj2>;

    template <typename Iter, typename Comp = Less<>, typename Proj = Identity>
    concept Sortable = std::sortable<Iter, Comp, Proj>;

    template <typename Fn, typename... Is>
    using IndirectResult = std::indirect_result_t<Fn, Is...>;

    template <IndirectlyReadable Iter, IndirectlyRegularUnaryInvocable<Iter> Proj>
    using Projected = std::projected<Iter, Proj>;

    template <typename T>
    using IncrementableTraits = std::incrementable_traits<T>;

    template <typename T>
    using IndirectlyReadableTraits = std::indirectly_readable_traits<T>;

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

    template <typename Iterable>
        requires
            (requires { typename Iterable::iterator; }) ||
            (requires { typename Iterable::Iterator; })
    using Iterator = ::stdx::meta::ConditionalType<
        (requires { typename Iterable::Iterator; }),
        typename Iterable::Iterator,
        typename Iterable::iterator
    >;

    template <typename Iter>
    using ReverseIterator = std::reverse_iterator<Iter>;

    template <typename Iter>
    using MoveIterator = std::move_iterator<Iter>;

    template <Semiregular Sent>
    using MoveSentinel = std::move_sentinel<Sent>;

    #ifdef __cpp_lib_ranges_as_const
    template <InputIterator Iter>
    using BasicConstIterator = std::basic_const_iterator<Iter>;

    template <InputIterator Iter>
    using ConstIterator = std::const_iterator<Iter>;

    template <Semiregular Sent>
    using ConstSentinel = std::const_sentinel<Sent>;
    #endif

    template <InputOrOutputIterator Iter, SentinelFor<Iter> Sent>
    using CommonIterator = std::common_iterator<Iter, Sent>;

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

    template <typename T, typename CharT = char, typename Traits = CharTraits<CharT>, typename Distance = ptrdiff>
    using InputStreamIterator = std::istream_iterator<T, CharT, Traits, Distance>;

    template <typename T, typename CharT = char, typename Traits = CharTraits<CharT>>
    using OutputStreamIterator = std::ostream_iterator<T, CharT, Traits>;

    template <typename CharT, typename Traits = CharTraits<CharT>>
    using InputStreamBufferIterator = std::istreambuf_iterator<CharT, Traits>;

    template <typename CharT, typename Traits = CharTraits<CharT>>
    using OutputStreamBufferIterator = std::ostreambuf_iterator<CharT, Traits>;

    inline constexpr DefaultSentinel DEFAULT_SENTINEL = std::default_sentinel;
    inline constexpr UnreachableSentinel UNREACHABLE_SENTINEL = std::unreachable_sentinel;


    class Iterators final {
    public:
        Iterators() = delete;

        template <typename Iter>
        [[nodiscard]]
        static ReverseIterator<Iter> reverse_iterator(Iter it) {
            return std::make_reverse_iterator(it);
        }

        template <typename Iter>
        [[nodiscard]]
        static MoveIterator<Iter> move_iterator(Iter it) {
            return std::make_move_iterator(it);
        }

        #ifdef __cpp_lib_ranges_as_const
        template <InputIterator Iter>
        [[nodiscard]]
        static ConstIterator<Iter> const_iterator(Iter it) {
            return std::make_const_iterator(it);
        }

        template <Semiregular Sent>
        [[nodiscard]]
        static ConstSentinel<Sent> const_sentinel(Sent s) {
            return std::make_const_sentinel(s);
        }
        #endif

        template <typename Contr>
        [[nodiscard]]
        static FrontInsertIterator<Contr> front_inserter(Contr& c) {
            return std::front_inserter(c);
        }

        template <typename Contr>
        [[nodiscard]]
        static BackInsertIterator<Contr> back_inserter(Contr& c) {
            return std::back_inserter(c);
        }

        template <typename Contr>
        [[nodiscard]]
        static InsertIterator<Contr> inserter(Contr& c, Iterator<Contr> it) {
            return std::inserter(c, it);
        }

        template <typename InputIt, typename Dist>
        static void advance(InputIt& it, Dist n) {
            std::advance(it, n);
        }

        template <typename InputIt>
        [[nodiscard]]
        static auto distance(InputIt first, InputIt last) {
            return std::distance(first, last);
        }

        template <typename InputIt>
        [[nodiscard]]
        static constexpr InputIt next(InputIt it, typename IteratorTraits<InputIt>::difference_type n = 1) {
            return std::next(it, n);
        }

        template <typename BidirIt>
        [[nodiscard]]
        static constexpr BidirIt prev(BidirIt it, typename IteratorTraits<BidirIt>::difference_type n = 1) {
            return std::prev(it, n);
        }
    };
}
