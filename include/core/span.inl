#pragma once

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    #if __has_include(<span>)
    inline constexpr usize DYNAMIC_EXTENT = std::dynamic_extent;

    template <typename T, usize Extent = DYNAMIC_EXTENT>
    using Span = std::span<T, Extent>;

    using std::as_bytes;
    using std::as_writable_bytes;

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
    using std::empty;
    using std::data;
    #endif
}
