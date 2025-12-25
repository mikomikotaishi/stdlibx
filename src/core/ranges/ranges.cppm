/**
 * @file ranges.cppm
 * @module core:ranges.ranges
 * @brief Module file for standard library ranges operations.
 *
 * This file contains the implementation of the ranges operations in the standard library.
 */

module;

#if __has_include(<ranges>)
#include <ranges>
#endif

export module core:ranges.ranges;

import :concepts;
import :iter.iterator;

using core::iter::SentinelFor;
using core::iter::SizedSentinelFor;
using core::iter::IndirectBinaryPredicate;
using core::iter::IndirectUnaryPredicate;
using core::iter::InputOrOutputIterator;
using core::iter::UnreachableSentinel;
using core::iter::WeaklyIncrementable;

/**
 * @namespace core::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
export namespace core::ranges {
    #if __has_include(<ranges>)
    namespace access {
        using Begin = decltype(std::ranges::begin);
        using End = decltype(std::ranges::end);
        using ConstBegin = decltype(std::ranges::cbegin);
        using ConstEnd = decltype(std::ranges::cend);
        using ReverseBegin = decltype(std::ranges::rbegin);
        using ReverseEnd = decltype(std::ranges::rend);
        using ConstReverseBegin = decltype(std::ranges::crbegin);
        using ConstReverseEnd = decltype(std::ranges::crend);
        using Size = decltype(std::ranges::size);
        using SignedSize = decltype(std::ranges::ssize);
        using Empty = decltype(std::ranges::empty);
        using Data = decltype(std::ranges::data);
        using ConstData = decltype(std::ranges::cdata);
    }

    inline namespace range_cpo {
        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        inline constexpr access::ConstBegin CBegin = std::ranges::cbegin;
        inline constexpr access::ConstEnd CEnd = std::ranges::cend;
        inline constexpr access::ReverseBegin RBegin = std::ranges::rbegin;
        inline constexpr access::ReverseEnd REnd = std::ranges::rend;
        inline constexpr access::ConstReverseBegin CRBegin = std::ranges::crbegin;
        inline constexpr access::ConstReverseEnd CREnd = std::ranges::crend;
        inline constexpr access::SignedSize SSize = std::ranges::ssize;
        inline constexpr access::ConstData CData = std::ranges::cdata;
        #endif
        inline constexpr access::Begin Begin = std::ranges::begin;
        inline constexpr access::End End = std::ranges::end;
        inline constexpr access::ConstBegin ConstBegin = std::ranges::cbegin;
        inline constexpr access::ConstEnd ConstEnd = std::ranges::cend;
        inline constexpr access::ReverseBegin ReverseBegin = std::ranges::rbegin;
        inline constexpr access::ReverseEnd ReverseEnd = std::ranges::rend;
        inline constexpr access::ConstReverseBegin ConstReverseBegin = std::ranges::crbegin;
        inline constexpr access::ConstReverseEnd ConstReverseEnd = std::ranges::crend;
        inline constexpr access::Size Size = std::ranges::size;
        inline constexpr access::SignedSize SignedSize = std::ranges::ssize;
        inline constexpr access::Empty Empty = std::ranges::empty;
        inline constexpr access::Data Data = std::ranges::data;
        inline constexpr access::ConstData ConstData = std::ranges::cdata;
    }

    template <typename T>
    concept Range = std::ranges::range<T>;

    template <typename T>
    concept BorrowedRange = std::ranges::borrowed_range<T>;

    template <typename T>
    constexpr bool EnableBorrowedRange = std::ranges::enable_borrowed_range<T>;

    template <typename T>
    concept SizedRange = std::ranges::sized_range<T>;

    template <typename T>
    constexpr bool DisableSizedRange = std::ranges::disable_sized_range<T>;

    template <typename T>
    concept View = std::ranges::view<T>;

    template <typename T>
    constexpr bool EnableView = std::ranges::enable_view<T>;

    template <typename T>
    concept InputRange = std::ranges::input_range<T>;

    template <typename R, typename T>
    concept OutputRange = std::ranges::output_range<R, T>;

    template <typename T>
    concept ForwardRange = std::ranges::forward_range<T>;

    template <typename T>
    concept BidirectionalRange = std::ranges::bidirectional_range<T>;

    template <typename T>
    concept RandomAccessRange = std::ranges::random_access_range<T>;

    template <typename T>
    concept ContiguousRange = std::ranges::contiguous_range<T>;

    template <typename T>
    concept CommonRange = std::ranges::common_range<T>;

    template <typename T>
    concept ViewableRange = std::ranges::viewable_range<T>;

    template <typename T>
    concept ConstantRange = std::ranges::constant_range<T>;

    using std::ranges::to;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <typename T>
    using Iterator_t = std::ranges::iterator_t<T>;

    template <Range R>
    using ConstIterator_t = std::ranges::const_iterator_t<R>;

    template <Range R>
    using Sentinel_t = std::ranges::sentinel_t<R>;

    template <Range R>
    using ConstSentinel_t = std::ranges::const_sentinel_t<R>;

    template <typename T>
    using RangeIterator_t = std::ranges::iterator_t<T>;

    template <Range R>
    using RangeConstIterator_t = std::ranges::const_iterator_t<R>;

    template <Range R>
    using RangeSentinel_t = std::ranges::sentinel_t<R>;

    template <Range R>
    using RangeConstSentinel_t = std::ranges::const_sentinel_t<R>;

    template <Range R>
    using RangeDifference_t = std::ranges::range_difference_t<R>;

    template <SizedRange R>
    using RangeSize_t = std::ranges::range_size_t<R>;

    template <Range R>
    using RangeValue_t = std::ranges::range_value_t<R>;

    template <Range R>
    using RangeReference_t = std::ranges::range_reference_t<R>;

    template <Range R>
    using RangeConstReference_t = std::ranges::range_const_reference_t<R>;

    template <Range R>
    using RangeRvalueReference_t = std::ranges::range_rvalue_reference_t<R>;

    template <Range R>
    using RangeCommonReference_t = std::ranges::range_common_reference_t<R>;
    #endif

    template <typename T>
    using RangeIterator = std::ranges::iterator_t<T>;

    template <Range R>
    using RangeConstIterator = std::ranges::const_iterator_t<R>;

    template <Range R>
    using RangeSentinel = std::ranges::sentinel_t<R>;

    template <Range R>
    using RangeConstSentinel = std::ranges::const_sentinel_t<R>;

    template <Range R>
    using RangeDifference = std::ranges::range_difference_t<R>;

    template <SizedRange R>
    using RangeSize = std::ranges::range_size_t<R>;

    template <Range R>
    using RangeValue = std::ranges::range_value_t<R>;

    template <Range R>
    using RangeReference = std::ranges::range_reference_t<R>;

    template <Range R>
    using RangeConstReference = std::ranges::range_const_reference_t<R>;

    template <Range R>
    using RangeRvalueReference = std::ranges::range_rvalue_reference_t<R>;

    template <Range R>
    using RangeCommonReference = std::ranges::range_common_reference_t<R>;

    template <typename Derived>
    using ViewInterface = std::ranges::view_interface<Derived>;

    enum class SubrangeKind: bool {
        Unsized = static_cast<bool>(std::ranges::subrange_kind::unsized),
        Sized = static_cast<bool>(std::ranges::subrange_kind::sized)
    };

    template <
        InputOrOutputIterator Iter, 
        SentinelFor<Iter> Sent = Iter, 
        SubrangeKind K = SizedSentinelFor<Sent, Iter> ? SubrangeKind::Sized : SubrangeKind::Unsized
    >
    using Subrange = std::ranges::subrange<Iter, Sent, static_cast<std::ranges::subrange_kind>(K)>;

    using Dangling = std::ranges::dangling;

    template <Range R>
    using BorrowedIteratorType = std::ranges::borrowed_iterator_t<R>;

    template <Range R>
    using BorrowedSubrangeType = std::ranges::borrowed_subrange_t<R>;

    template <typename T>
    using EmptyView = std::ranges::empty_view<T>;

    template <CopyConstructible T>
    using SingleView = std::ranges::single_view<T>;

    template <WeaklyIncrementable W, Semiregular Bound = UnreachableSentinel>
    using IotaView = std::ranges::iota_view<W, Bound>;

    template <MoveConstructible W, Semiregular Bound = UnreachableSentinel>
    using RepeatView = std::ranges::repeat_view<W, Bound>;

    namespace types {
        template <typename T>
        using Empty = decltype(std::ranges::views::empty<T>);
        using Single = decltype(std::ranges::views::single);
        using Iota = decltype(std::ranges::views::iota);
        using Repeat = decltype(std::ranges::views::repeat);
        using All = decltype(std::ranges::views::all);
        using AsRvalue = decltype(std::ranges::views::as_rvalue);
        using Filter = decltype(std::ranges::views::filter);
        using Transform = decltype(std::ranges::views::transform);
        using Take = decltype(std::ranges::views::take);
        using TakeWhile = decltype(std::ranges::views::take_while);
        using Drop = decltype(std::ranges::views::drop);
        using DropWhile = decltype(std::ranges::views::drop_while);
        using Join = decltype(std::ranges::views::join);
        using JoinWith = decltype(std::ranges::views::join_with);
        using LazySplit = decltype(std::ranges::views::lazy_split);
        using Split = decltype(std::ranges::views::split);
        using Counted = decltype(std::ranges::views::counted);
        using Common = decltype(std::ranges::views::common);
        using Reverse = decltype(std::ranges::views::reverse);
        using AsConst = decltype(std::ranges::views::as_const);
        template <std::size_t N>
        using Elements = decltype(std::ranges::views::elements<N>);
        using Keys = decltype(std::ranges::views::keys);
        using Values = decltype(std::ranges::views::values);
        using Enumerate = decltype(std::ranges::views::enumerate);
        using Zip = decltype(std::ranges::views::zip);
        using ZipTransform = decltype(std::ranges::views::zip_transform);
        template <std::size_t N>
        using Adjacent = decltype(std::ranges::views::adjacent<N>);
        using Pairwise = decltype(std::ranges::views::pairwise);
        template <std::size_t N>
        using AdjacentTransform = decltype(std::ranges::views::adjacent_transform<N>);
        using PairwiseTransform = decltype(std::ranges::views::pairwise_transform);
        using Chunk = decltype(std::ranges::views::chunk);
        using Slide = decltype(std::ranges::views::slide);
        using ChunkBy = decltype(std::ranges::views::chunk_by);
        using Stride = decltype(std::ranges::views::stride);
        using CartesianProduct = decltype(std::ranges::views::cartesian_product);
    }

    template <typename Derived>
    using RangeAdaptorClosure = std::ranges::range_adaptor_closure<Derived>;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    template <Range R>
    using RefView = std::ranges::ref_view<R>;
    #endif

    template <Range R>
    using ReferenceView = std::ranges::ref_view<R>;

    template <Range R>
    using OwningView = std::ranges::owning_view<R>;

    template <InputRange V>
    using AsRvalueView = std::ranges::as_rvalue_view<V>;

    template <InputRange V, IndirectUnaryPredicate<RangeIterator<V>> Predicate>
    using FilterView = std::ranges::filter_view<V, Predicate>;

    template <InputRange V, CopyConstructible F>
    using TransformView = std::ranges::transform_view<V, F>;

    template <View V>
    using TakeView = std::ranges::take_view<V>;

    template <View V, typename Predicate>
    using TakeWhileView = std::ranges::take_while_view<V, Predicate>;

    template <View V>
    using DropView = std::ranges::drop_view<V>;

    template <View V, typename Predicate>
    using DropWhileView = std::ranges::drop_while_view<V, Predicate>;

    template <InputRange V>
    using JoinView = std::ranges::join_view<V>;

    template <InputRange V, ForwardRange Pattern>
    using JoinWithView = std::ranges::join_with_view<V, Pattern>;

    template <InputRange V, ForwardRange Pattern>
    using LazySplitView = std::ranges::lazy_split_view<V, Pattern>;

    template <ForwardRange V, ForwardRange Pattern>
    using SplitView = std::ranges::split_view<V, Pattern>;

    template <View V>
    using CommonView = std::ranges::common_view<V>;

    template <View V>
    using ReverseView = std::ranges::reverse_view<V>;

    template <View V>
    using AsConstView = std::ranges::as_const_view<V>;

    template <InputRange V, std::size_t N>
    using ElementsView = std::ranges::elements_view<V, N>;

    template <typename R>
    using KeysView = std::ranges::keys_view<R>;

    template <typename R>
    using ValuesView = std::ranges::values_view<R>;

    template <View V>
    using EnumerateView = std::ranges::enumerate_view<V>;

    template <InputRange... Vs>
    using ZipView = std::ranges::zip_view<Vs...>;

    template <MoveConstructible F, InputRange... Vs>
    using ZipTransformView = std::ranges::zip_transform_view<F, Vs...>;

    template <ForwardRange V, std::size_t N>
    using AdjacentView = std::ranges::adjacent_view<V, N>;

    template <ForwardRange V, MoveConstructible F, std::size_t N>
    using AdjacentTransformView = std::ranges::adjacent_transform_view<V, F, N>;

    template <View V>
    using ChunkView = std::ranges::chunk_view<V>;

    template <ForwardRange V>
    using SlideView = std::ranges::slide_view<V>;

    template <ForwardRange V, IndirectBinaryPredicate<RangeIterator<V>, RangeIterator<V>> Predicate>
    using ChunkByView = std::ranges::chunk_by_view<V, Predicate>;

    template <InputRange V>
    using StrideView = std::ranges::stride_view<V>;

    template <InputRange First, ForwardRange... Vs>
    using CartesianProductView = std::ranges::cartesian_product_view<First, Vs...>;
    
    namespace views {
        template <typename T>
        inline constexpr types::Empty<T> Empty = std::ranges::views::empty<T>;
        inline constexpr types::Single Single = std::ranges::views::single;
        inline constexpr types::Iota Iota = std::ranges::views::iota;
        inline constexpr types::Repeat Repeat = std::ranges::views::repeat;
        template <ViewableRange R>
        using AllTag = std::ranges::views::all_t<R>;
        inline constexpr types::All All = std::ranges::views::all;
        inline constexpr types::AsRvalue AsRvalue = std::ranges::views::as_rvalue;
        inline constexpr types::Filter Filter = std::ranges::views::filter;
        inline constexpr types::Transform Transform = std::ranges::views::transform;
        inline constexpr types::Take Take = std::ranges::views::take;
        inline constexpr types::TakeWhile TakeWhile = std::ranges::views::take_while;
        inline constexpr types::Drop Drop = std::ranges::views::drop;
        inline constexpr types::DropWhile DropWhile = std::ranges::views::drop_while;
        inline constexpr types::Join Join = std::ranges::views::join;
        inline constexpr types::JoinWith JoinWith = std::ranges::views::join_with;
        inline constexpr types::LazySplit LazySplit = std::ranges::views::lazy_split;
        inline constexpr types::Split Split = std::ranges::views::split;
        inline constexpr types::Counted Counted = std::ranges::views::counted;
        inline constexpr types::Common Common = std::ranges::views::common;
        inline constexpr types::Reverse Reverse = std::ranges::views::reverse;
        inline constexpr types::AsConst AsConst = std::ranges::views::as_const;
        template <std::size_t N>
        inline constexpr types::Elements<N> Elements = std::ranges::views::elements<N>;
        inline constexpr types::Keys Keys = std::ranges::views::keys;
        inline constexpr types::Values Values = std::ranges::views::values;
        inline constexpr types::Enumerate Enumerate = std::ranges::views::enumerate;
        inline constexpr types::Zip Zip = std::ranges::views::zip;
        inline constexpr types::ZipTransform ZipTransform = std::ranges::views::zip_transform;
        template <std::size_t N>
        inline constexpr types::Adjacent<N> Adjacent = std::ranges::views::adjacent<N>;
        inline constexpr types::Pairwise Pairwise = std::ranges::views::pairwise;
        template <std::size_t N>
        inline constexpr types::AdjacentTransform<N> AdjacentTransform = std::ranges::views::adjacent_transform<N>;
        inline constexpr types::PairwiseTransform PairwiseTransform = std::ranges::views::pairwise_transform;
        inline constexpr types::Chunk Chunk = std::ranges::views::chunk;
        inline constexpr types::Slide Slide = std::ranges::views::slide;
        inline constexpr types::ChunkBy ChunkBy = std::ranges::views::chunk_by;
        inline constexpr types::Stride Stride = std::ranges::views::stride;
        inline constexpr types::CartesianProduct CartesianProduct = std::ranges::views::cartesian_product;
    }

    using std::tuple_size;
    using std::tuple_element;
    using std::ranges::get;
    using FromRangeTag = std::from_range_t;
    inline constexpr FromRangeTag FromRange = std::from_range;
    #endif
}

export namespace core {
    #if __has_include(<ranges>)
    namespace views = core::ranges;
    #endif
}
