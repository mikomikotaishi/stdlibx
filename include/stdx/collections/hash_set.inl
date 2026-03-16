#pragma once

/**
 * @namespace stdx::collections
 * @brief Wrapper namespace for standard library collection operations.
 */
export namespace stdx::collections {
    using ::alloc::collections::HashSet;
    using ::alloc::collections::HashMultiset;

    using ::alloc::collections::erase_if;

    using ::alloc::collections::operator==;
    using ::alloc::collections::operator<=>;

    using ::alloc::collections::begin;
    using ::alloc::collections::cbegin;
    using ::alloc::collections::end;
    using ::alloc::collections::cend;
    using ::alloc::collections::rbegin;
    using ::alloc::collections::crbegin;
    using ::alloc::collections::rend;
    using ::alloc::collections::crend;
    using ::alloc::collections::size;
    using ::alloc::collections::ssize;
    using ::alloc::collections::empty;
    using ::alloc::collections::data;

    using ::alloc::collections::hash;
    using ::alloc::collections::swap;
}
