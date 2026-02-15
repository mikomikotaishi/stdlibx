#pragma once

/**
 * @class Tags
 * @brief A utility class containing standard library tags.
 */
class Tags {
private:
    using usize = ::core::usize;
public:
    Tags() = delete;

    using Unexpect = ::core::UnexpectTag;
    #if __has_include(<ranges>)
    using FromRange = ::core::ranges::FromRangeTag;
    #endif
    template <usize I>
    using InPlaceIndex = ::core::util::InPlaceIndexTag<I>;
    using InPlace = ::core::util::InPlaceTag;
    template <typename T>
    using InPlaceType = ::core::util::InPlaceTypeTag<T>;
    using PiecewiseConstruct = ::core::util::PiecewiseConstructTag;

    #ifndef STDLIBX_NO_STD_NO_ALLOC_FEATURES
    #if __has_include(<flat_map>)
    using SortedEquivalent = ::alloc::collections::SortedEquivalentTag;
    using SortedUnique = ::alloc::collections::SortedUniqueTag;
    #endif
    using AllocatorArgument = ::alloc::mem::AllocatorArgumentTag;
    using DestroyingDelete = ::alloc::mem::DestroyingDeleteTag;
    using NoThrow = ::alloc::mem::NoThrowTag;
    #ifndef STDLIBX_NO_STD_FEATURES
    using AdoptLock = ::stdx::sync::AdoptLockTag;
    using DeferLock = ::stdx::sync::DeferLockTag;
    using TryToLock = ::stdx::sync::TryToLockTag;
    #if __has_include(<stop_token>)
    using NoStopState = ::stdx::thread::NoStopStateTag;
    #endif
    using Last = ::stdx::time::chrono::LastSpecifier;
    #endif
    #endif

    static constexpr Unexpect UNEXPECT = ::core::Unexpect;
    #if __has_include(<ranges>)
    static constexpr FromRange FROM_RANGE = ::core::ranges::FromRange;
    #endif
    static constexpr InPlace IN_PLACE = ::core::util::InPlace;
    template <usize I>
    static constexpr InPlaceIndex<I> IN_PLACE_INDEX = ::core::util::InPlaceIndex<I>;
    template <typename T>
    static constexpr InPlaceType<T> IN_PLACE_TYPE = ::core::util::InPlaceType<T>;
    static constexpr PiecewiseConstruct PIECEWISE_CONSTRUCT = ::core::util::PiecewiseConstruct;
    #ifndef STDLIBX_NO_STD_NO_ALLOC_FEATURES
    #if __has_include(<flat_map>)
    static constexpr SortedEquivalent SORTED_EQUIVALENT = ::alloc::collections::SortedEquivalent;
    static constexpr SortedUnique SORTED_UNIQUE = ::alloc::collections::SortedUnique;
    #endif
    static constexpr AllocatorArgument ALLOCATOR_ARGUMENT = ::alloc::mem::AllocatorArgument;
    static constexpr DestroyingDelete DESTROYING_DELETE = ::alloc::mem::DestroyingDelete;
    static constexpr NoThrow NO_THROW = ::alloc::mem::NoThrow;
    #ifndef STDLIBX_NO_STD_FEATURES
    static constexpr AdoptLock ADOPT_LOCK = ::stdx::sync::AdoptLock;
    static constexpr DeferLock DEFER_LOCK = ::stdx::sync::DeferLock;
    static constexpr TryToLock TRY_TO_LOCK = ::stdx::sync::TryToLock;
    #if __has_include(<stop_token>)
    static constexpr NoStopState NO_STOP_STATE = ::stdx::thread::NoStopState;
    #endif
    static constexpr Last LAST = ::stdx::time::chrono::Last;
    #endif
    #endif
};