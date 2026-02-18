#pragma once

/**
 * @namespace stdx::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace stdx::collections {
    #if __has_include(<flat_map>)
    using alloc::collections::FlatMap;
    using alloc::collections::FlatMultimap;

    using alloc::collections::SortedUniqueTag;
    using alloc::collections::SortedUnique;
    using alloc::collections::SortedEquivalentTag;
    using alloc::collections::SortedEquivalent;

    using alloc::collections::erase_if;
    #endif
}
