/**
 * @file span.cppm
 * @module core:span
 * @brief Module file for importing the Span class from the standard library.
 * 
 * This file exports the Span class operations in the standard library.
 */

module;

#if __has_include(<span>)
#include <span>
#endif

#include "Macros.hpp"

export module core:span;

/**
 * @namespace core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace core {
    #if __has_include(<span>)
    inline constexpr std::size_t DYNAMIC_EXTENT = std::dynamic_extent;

    template <typename T, std::size_t Extent = DYNAMIC_EXTENT>
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

#if (defined(STDLIBX_NO_STD) || defined(STDLIBX_NO_ALLOC)) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_CORE_MODULE_EXPORT_CORE();
#endif
