/**
 * @file iterator.cppm
 * @module std:iter.iterator
 * @brief Module file for standard library iterator operations.
 *
 * This file contains the implementation of the iterator operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:iter.iterator;
#else
export module stdlib:iter.iterator;
#endif

import core;

/**
 * @namespace std::iter
 * @brief Wrapper namespace for standard library iterator operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::iter {
#else 
export namespace stdlib::iter {
#endif
    using core::iter::IndirectlyReadable;
    using core::iter::IndirectlyWritable;
    using core::iter::WeaklyIncrementable;
    using core::iter::Incrementable;
    using core::iter::InputOrOutputIterator;
    using core::iter::SentinelFor;
    using core::iter::SizedSentinelFor;
    using core::iter::InputIterator;
    using core::iter::OutputIterator;
    using core::iter::ForwardIterator;
    using core::iter::BidirectionalIterator;
    using core::iter::RandomAccessIterator;
    using core::iter::ContiguousIterator;
    using core::iter::IndirectlyUnaryInvocable;
    using core::iter::IndirectlyRegularUnaryInvocable;
    using core::iter::IndirectUnaryPredicate;
    using core::iter::IndirectBinaryPredicate;
    using core::iter::IndirectEquivalenceRelation;
    using core::iter::IndirectStrictWeakOrder;
    using core::iter::IndirectlyMovable;
    using core::iter::IndirectlyMovableStorable;
    using core::iter::IndirectlyCopyable;
    using core::iter::IndirectlyCopyableStorable;
    using core::iter::IndirectlySwappable;
    using core::iter::IndirectlyComparable;
    using core::iter::Permutable;
    using core::iter::Mergeable;
    using core::iter::Sortable;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::iter::IndirectResult_t;
    #endif

    using core::iter::IndirectResult;
    using core::iter::Projected;
    using core::iter::IncrementableTraits;
    using core::iter::IndirectlyReadableTraits;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::iter::IterValue;
    using core::iter::IterReference;
    using core::iter::IterConstReference;
    using core::iter::IterDifference;
    using core::iter::IterRValueReference;
    using core::iter::IterCommonReference;
    using core::iter::IterValue_t;
    using core::iter::IterReference_t;
    using core::iter::IterConstReference_t;
    using core::iter::IterDifference_t;
    using core::iter::IterRValueReference_t;
    using core::iter::IterCommonReference_t;
    using core::iter::IteratorValue_t;
    using core::iter::IteratorReference_t;
    using core::iter::IteratorConstReference_t;
    using core::iter::IteratorDifference_t;
    using core::iter::IteratorRValueReference_t;
    using core::iter::IteratorCommonReference_t;
    #endif

    using core::iter::IteratorValue;
    using core::iter::IteratorReference;
    using core::iter::IteratorConstReference;
    using core::iter::IteratorDifference;
    using core::iter::IteratorRValueReference;
    using core::iter::IteratorCommonReference;
    using core::iter::IteratorTraits;
    using core::iter::InputIteratorTag;
    using core::iter::OutputIteratorTag;
    using core::iter::ForwardIteratorTag;
    using core::iter::BidirectionalIteratorTag;
    using core::iter::RandomAccessIteratorTag;
    using core::iter::ContiguousIteratorTag;
    using core::iter::IIterator;
    using core::iter::Iterator;
    using core::iter::ReverseIterator;
    using core::iter::MoveIterator;
    using core::iter::MoveSentinel;
    using core::iter::BasicConstIterator;
    using core::iter::ConstIterator;
    using core::iter::ConstSentinel;
    using core::iter::CommonIterator;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::iter::DefaultSentinel_t;
    using core::iter::UnreachableSentinel_t;
    #endif

    using core::iter::DefaultSentinel;
    using core::iter::UnreachableSentinel;
    using core::iter::CountedIterator;
    using core::iter::BackInsertIterator;
    using core::iter::FrontInsertIterator;
    using core::iter::InsertIterator;
    
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::iter::IStreamIterator;
    using core::iter::OStreamIterator;
    using core::iter::IStreamBufIterator;
    using core::iter::OStreamBufIterator;
    #endif

    using core::iter::InputStreamIterator;
    using core::iter::OutputStreamIterator;
    using core::iter::InputStreamBufferIterator;
    using core::iter::OutputStreamBufferIterator;

    using core::iter::make_reverse_iterator;
    using core::iter::make_move_iterator;
    using core::iter::make_const_iterator;
    using core::iter::make_const_sentinel;
    using core::iter::front_inserter;
    using core::iter::back_inserter;
    using core::iter::inserter;

    using core::iter::operator==;
    using core::iter::operator<;
    using core::iter::operator<=;
    using core::iter::operator>;
    using core::iter::operator>=;
    using core::iter::operator<=>;
    using core::iter::operator+;
    using core::iter::operator-;

    using core::iter::advance;
    using core::iter::distance;
    using core::iter::next;
    using core::iter::prev;

    using core::iter::begin;
    using core::iter::cbegin;
    using core::iter::end;
    using core::iter::cend;
    using core::iter::rbegin;
    using core::iter::crbegin;
    using core::iter::rend;
    using core::iter::crend;
    using core::iter::size;
    using core::iter::ssize;
    using core::iter::data;

    /**
    * @namespace std::iter::ranges
    * @brief Wrapper namespace for standard library ranges operations.
    */
    namespace ranges {
        using core::iter::ranges::advance;
        using core::iter::ranges::distance;
        using core::iter::ranges::next;
        using core::iter::ranges::prev;
    }
}
