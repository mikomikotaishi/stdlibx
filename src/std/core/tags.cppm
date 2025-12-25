/**
 * @file tags.cppm
 * @module std:core.tags
 * @brief Module file for standard library tags aggregation class.
 *
 * This file contains the implementation of the tags aggregation class in the standard library.
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:core.tags;
#else
export module stdlib:core.tags;
#endif

import :concurrent.stop_token;
import :sync.mutex;
import :time.chrono;

import core;
import alloc;

using core::Unexpect;
#if __has_include(<ranges>)
using core::ranges::FromRange;
#endif
using core::util::InPlace;
using core::util::InPlaceIndex;
using core::util::InPlaceType;
using core::util::PiecewiseConstruct;

using alloc::collections::SortedEquivalent;
using alloc::collections::SortedUnique;
using alloc::mem::AllocatorArgument;
using alloc::mem::DestroyingDelete;
using alloc::mem::NoThrow;

#ifdef STDLIBX_NO_RESERVED_STD_NAMESPACE
#if __has_include(<stop_token>)
using std::concurrent::NoStopState;
#endif
using std::sync::AdoptLock;
using std::sync::DeferLock;
using std::sync::TryToLock;
using std::time::chrono::Last;
#else
#if __has_include(<stop_token>)
using stdlib::concurrent::NoStopState;
#endif
using stdlib::sync::AdoptLock;
using stdlib::sync::DeferLock;
using stdlib::sync::TryToLock;
using stdlib::time::chrono::Last;
#endif

/**
 * @namespace std::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::core {
#else 
export namespace stdlib::core {
#endif
    /**
     * @class Tags
     * @brief A utility class containing standard library tags.
     */
    class Tags {
    private:
        using usize = ::core::usize;
    public:
        Tags() = delete;

        using UnexpectTag = ::core::UnexpectTag;
        #if __has_include(<ranges>)
        using FromRangeTag = ::core::ranges::FromRangeTag;
        #endif
        template <usize I>
        using InPlaceIndexTag = ::core::util::InPlaceIndexTag<I>;
        using InPlaceTag = ::core::util::InPlaceTag;
        template <typename T>
        using InPlaceTypeTag = ::core::util::InPlaceTypeTag<T>;
        using PiecewiseConstructTag = ::core::util::PiecewiseConstructTag;
        #if __has_include(<flat_map>)
        using SortedEquivalentTag = ::alloc::collections::SortedEquivalentTag;
        using SortedUniqueTag = ::alloc::collections::SortedUniqueTag;
        #endif
        using AllocatorArgumentTag = ::alloc::mem::AllocatorArgumentTag;
        using DestroyingDeleteTag = ::alloc::mem::DestroyingDeleteTag;
        using NoThrowTag = ::alloc::mem::NoThrowTag;
        #if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
        #if __has_include(<stop_token>)
        using NoStopStateTag = ::std::concurrent::NoStopStateTag;
        #endif
        using AdoptLockTag = ::std::sync::AdoptLockTag;
        using DeferLockTag = ::std::sync::DeferLockTag;
        using TryToLockTag = ::std::sync::TryToLockTag;
        using LastSpecifier = ::std::time::chrono::LastSpecifier;
        #else
        #if __has_include(<stop_token>)
        using NoStopStateTag = ::stdlib::concurrent::NoStopStateTag;
        #endif
        using AdoptLockTag = ::stdlib::sync::AdoptLockTag;
        using DeferLockTag = ::stdlib::sync::DeferLockTag;
        using TryToLockTag = ::stdlib::sync::TryToLockTag;
        using LastSpecifier = ::stdlib::time::chrono::LastSpecifier;
        #endif

        static constexpr UnexpectTag UNEXPECT = Unexpect;
        #if __has_include(<ranges>)
        static constexpr FromRangeTag FROM_RANGE = FromRange;
        #endif
        static constexpr InPlaceTag IN_PLACE = InPlace;
        template <usize I>
        static constexpr InPlaceIndexTag<I> IN_PLACE_INDEX = InPlaceIndex<I>;
        template <typename T>
        static constexpr InPlaceTypeTag<T> IN_PLACE_TYPE = InPlaceType<T>;
        static constexpr PiecewiseConstructTag PIECEWISE_CONSTRUCT = PiecewiseConstruct;
        #if __has_include(<flat_map>)
        static constexpr SortedEquivalentTag SORTED_EQUIVALENT = SortedEquivalent;
        static constexpr SortedUniqueTag SORTED_UNIQUE = SortedUnique;
        #endif
        static constexpr AllocatorArgumentTag ALLOCATOR_ARGUMENT = AllocatorArgument;
        static constexpr DestroyingDeleteTag DESTROYING_DELETE = DestroyingDelete;
        static constexpr NoThrowTag NO_THROW = NoThrow;
        #if __has_include(<span>)
        static constexpr NoStopStateTag NO_STOP_STATE = NoStopState;
        #endif
        static constexpr AdoptLockTag ADOPT_LOCK = AdoptLock;
        static constexpr DeferLockTag DEFER_LOCK = DeferLock;
        static constexpr TryToLockTag TRY_TO_LOCK = TryToLock;
        static constexpr LastSpecifier LAST = Last;
    };
}

#if !defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_STD_MODULE_EXPORT_CORE();
#endif
