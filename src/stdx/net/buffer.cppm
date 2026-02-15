/**
 * @file buffer.cppm
 * @module stdx:net.buffer
 * @brief Module file for (experimental) standard library networking buffer operations.
 *
 * This file contains the implementation of the (experimental) networking buffer operations in the standard library.
 */

module;

#if __has_include(<experimental/buffer>)
#include <experimental/buffer>
#endif

export module stdx:net.buffer;

/**
 * @namespace stdx::net
 * @brief Wrapper namespace for (experimental) standard library networking operations.
 */
export namespace stdx::net {
    #if __has_include(<experimental/buffer>)
    using ConstBuffer = std::experimental::net::const_buffer;
    using MutableBuffer = std::experimental::net::mutable_buffer;

    template <typename T, typename Allocator>
    using DynamicVectorBuffer = std::experimental::net::dynamic_vector_buffer<T, Allocator>;

    template <typename CharT, typename Traits, typename Allocator>
    using DynamicStringBuffer = std::experimental::net::dynamic_string_buffer<CharT, Traits, Allocator>;

    using TransferAll = std::experimental::net::transfer_all;
    using TransferAtLeast = std::experimental::net::transfer_at_least;
    using TransferExactly = std::experimental::net::transfer_exactly;
    #endif
}
