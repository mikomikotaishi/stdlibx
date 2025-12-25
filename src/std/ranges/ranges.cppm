/**
 * @file ranges.cppm
 * @module std:ranges.ranges
 * @brief Module file for standard library ranges operations.
 *
 * This file contains the implementation of the ranges operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:ranges.ranges;
#else
export module stdlib:ranges.ranges;
#endif

import core;
import alloc;

/**
 * @namespace std::ranges
 * @brief Wrapper namespace for standard library ranges operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::ranges {
#else 
export namespace stdlib::ranges {
#endif
    #if __has_include(<ranges>)
    namespace access {
        using ::core::ranges::access::Begin;
        using ::core::ranges::access::End;
        using ::core::ranges::access::ConstBegin;
        using ::core::ranges::access::ConstEnd;
        using ::core::ranges::access::ReverseBegin;
        using ::core::ranges::access::ReverseEnd;
        using ::core::ranges::access::ConstReverseBegin;
        using ::core::ranges::access::ConstReverseEnd;
        using ::core::ranges::access::Size;
        using ::core::ranges::access::SignedSize;
        using ::core::ranges::access::Empty;
        using ::core::ranges::access::Data;
        using ::core::ranges::access::ConstData;
    }

    inline namespace range_cpo {
        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        using ::core::ranges::range_cpo::CBegin;
        using ::core::ranges::range_cpo::CEnd;
        using ::core::ranges::range_cpo::RBegin;
        using ::core::ranges::range_cpo::REnd;
        using ::core::ranges::range_cpo::CRBegin;
        using ::core::ranges::range_cpo::CREnd;
        using ::core::ranges::range_cpo::SSize;
        using ::core::ranges::range_cpo::CData;
        #endif
        using ::core::ranges::range_cpo::Begin;
        using ::core::ranges::range_cpo::End;
        using ::core::ranges::range_cpo::ConstBegin;
        using ::core::ranges::range_cpo::ConstEnd;
        using ::core::ranges::range_cpo::ReverseBegin;
        using ::core::ranges::range_cpo::ReverseEnd;
        using ::core::ranges::range_cpo::ConstReverseBegin;
        using ::core::ranges::range_cpo::ConstReverseEnd;
        using ::core::ranges::range_cpo::Size;
        using ::core::ranges::range_cpo::SignedSize;
        using ::core::ranges::range_cpo::Empty;
        using ::core::ranges::range_cpo::Data;
        using ::core::ranges::range_cpo::ConstData;
    }

    using ::core::ranges::Range;
    using ::core::ranges::BorrowedRange;
    using ::core::ranges::EnableBorrowedRange;
    using ::core::ranges::SizedRange;
    using ::core::ranges::DisableSizedRange;
    using ::core::ranges::View;
    using ::core::ranges::EnableView;
    using ::core::ranges::InputRange;
    using ::core::ranges::OutputRange;
    using ::core::ranges::ForwardRange;
    using ::core::ranges::BidirectionalRange;
    using ::core::ranges::RandomAccessRange;
    using ::core::ranges::ContiguousRange;
    using ::core::ranges::CommonRange;
    using ::core::ranges::ViewableRange;
    using ::core::ranges::ConstantRange;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using ::core::ranges::Iterator_t;
    using ::core::ranges::ConstIterator_t;
    using ::core::ranges::Sentinel_t;
    using ::core::ranges::ConstSentinel_t;
    using ::core::ranges::RangeIterator_t;
    using ::core::ranges::RangeConstIterator_t;
    using ::core::ranges::RangeSentinel_t;
    using ::core::ranges::RangeConstSentinel_t;
    using ::core::ranges::RangeDifference_t;
    using ::core::ranges::RangeSize_t;
    using ::core::ranges::RangeValue_t;
    using ::core::ranges::RangeReference_t;
    using ::core::ranges::RangeConstReference_t;
    using ::core::ranges::RangeRvalueReference_t;
    using ::core::ranges::RangeCommonReference_t;
    #endif
    using ::core::ranges::RangeIterator;
    using ::core::ranges::RangeConstIterator;
    using ::core::ranges::RangeSentinel;
    using ::core::ranges::RangeConstSentinel;
    using ::core::ranges::RangeDifference;
    using ::core::ranges::RangeSize;
    using ::core::ranges::RangeValue;
    using ::core::ranges::RangeReference;
    using ::core::ranges::RangeConstReference;
    using ::core::ranges::RangeRvalueReference;
    using ::core::ranges::RangeCommonReference;
    using ::core::ranges::ViewInterface;
    using ::core::ranges::SubrangeKind;
    using ::core::ranges::Subrange;
    using ::core::ranges::Dangling;
    using ::core::ranges::BorrowedIteratorType;
    using ::core::ranges::BorrowedSubrangeType;
    using alloc::ranges::ElementsOf;
    using ::core::ranges::EmptyView;
    using ::core::ranges::SingleView;
    using ::core::ranges::IotaView;
    using ::core::ranges::RepeatView;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using alloc::ranges::BasicIStreamView;
    #endif
    using alloc::ranges::BasicInputStreamView;

    namespace types {
        using ::core::ranges::types::Empty;
        using ::core::ranges::types::Single;
        using ::core::ranges::types::Iota;
        using ::core::ranges::types::Repeat;
        using alloc::ranges::types::InputStream;
        using ::core::ranges::types::All;
        using ::core::ranges::types::AsRvalue;
        using ::core::ranges::types::Filter;
        using ::core::ranges::types::Transform;
        using ::core::ranges::types::Take;
        using ::core::ranges::types::TakeWhile;
        using ::core::ranges::types::Drop;
        using ::core::ranges::types::DropWhile;
        using ::core::ranges::types::Join;
        using ::core::ranges::types::JoinWith;
        using ::core::ranges::types::LazySplit;
        using ::core::ranges::types::Split;
        using ::core::ranges::types::Counted;
        using ::core::ranges::types::Common;
        using ::core::ranges::types::Reverse;
        using ::core::ranges::types::AsConst;
        using ::core::ranges::types::Elements;
        using ::core::ranges::types::Keys;
        using ::core::ranges::types::Values;
        using ::core::ranges::types::Enumerate;
        using ::core::ranges::types::Zip;
        using ::core::ranges::types::ZipTransform;
        using ::core::ranges::types::Adjacent;
        using ::core::ranges::types::Pairwise;
        using ::core::ranges::types::AdjacentTransform;
        using ::core::ranges::types::PairwiseTransform;
        using ::core::ranges::types::Chunk;
        using ::core::ranges::types::Slide;
        using ::core::ranges::types::ChunkBy;
        using ::core::ranges::types::Stride;
        using ::core::ranges::types::CartesianProduct;
    }

    using ::core::ranges::RangeAdaptorClosure;
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using ::core::ranges::RefView;
    #endif
    using ::core::ranges::ReferenceView;
    using ::core::ranges::OwningView;
    using ::core::ranges::AsRvalueView;
    using ::core::ranges::FilterView;
    using ::core::ranges::TransformView;
    using ::core::ranges::TakeView;
    using ::core::ranges::TakeWhileView;
    using ::core::ranges::DropView;
    using ::core::ranges::DropWhileView;
    using ::core::ranges::JoinView;
    using ::core::ranges::JoinWithView;
    using ::core::ranges::LazySplitView;
    using ::core::ranges::SplitView;
    using ::core::ranges::CommonView;
    using ::core::ranges::ReverseView;
    using ::core::ranges::AsConstView;
    using ::core::ranges::ElementsView;
    using ::core::ranges::KeysView;
    using ::core::ranges::ValuesView;
    using ::core::ranges::EnumerateView;
    using ::core::ranges::ZipView;
    using ::core::ranges::ZipTransformView;
    using ::core::ranges::AdjacentView;
    using ::core::ranges::AdjacentTransformView;
    using ::core::ranges::ChunkView;
    using ::core::ranges::SlideView;
    using ::core::ranges::ChunkByView;
    using ::core::ranges::StrideView;
    using ::core::ranges::CartesianProductView;

    namespace views {
        using ::core::ranges::views::Empty;
        using ::core::ranges::views::Single;
        using ::core::ranges::views::Iota;
        using ::core::ranges::views::Repeat;
        #ifdef STDLIBX_ENABLE_COMPAT_NAMES
        using alloc::ranges::views::IStream;
        #endif
        using alloc::ranges::views::InputStream;
        using ::core::ranges::views::AllTag;
        using ::core::ranges::views::All;
        using ::core::ranges::views::AsRvalue;
        using ::core::ranges::views::Filter;
        using ::core::ranges::views::Transform;
        using ::core::ranges::views::Take;
        using ::core::ranges::views::TakeWhile;
        using ::core::ranges::views::Drop;
        using ::core::ranges::views::DropWhile;
        using ::core::ranges::views::Join;
        using ::core::ranges::views::JoinWith;
        using ::core::ranges::views::LazySplit;
        using ::core::ranges::views::Split;
        using ::core::ranges::views::Counted;
        using ::core::ranges::views::Common;
        using ::core::ranges::views::Reverse;
        using ::core::ranges::views::AsConst;
        using ::core::ranges::views::Elements;
        using ::core::ranges::views::Keys;
        using ::core::ranges::views::Values;
        using ::core::ranges::views::Enumerate;
        using ::core::ranges::views::Zip;
        using ::core::ranges::views::ZipTransform;
        using ::core::ranges::views::Adjacent;
        using ::core::ranges::views::Pairwise;
        using ::core::ranges::views::AdjacentTransform;
        using ::core::ranges::views::PairwiseTransform;
        using ::core::ranges::views::Chunk;
        using ::core::ranges::views::Slide;
        using ::core::ranges::views::ChunkBy;
        using ::core::ranges::views::Stride;
        using ::core::ranges::views::CartesianProduct;
    }

    using ::core::ranges::to;
    using ::core::ranges::get;

    using ::core::ranges::FromRangeTag;
    using ::core::ranges::FromRange;

    using ::core::ranges::tuple_size;
    using ::core::ranges::tuple_element;
    #endif
}

#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std {
#else
export namespace stdlib {
#endif
    #if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
    namespace views = std::ranges;
    #else
    namespace views = stdlib::ranges;
    #endif
}
