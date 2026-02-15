/**
 * @file io_context.cppm
 * @module stdx:net.io_context
 * @brief Module file for (experimental) standard library networking I/O context operations.
 *
 * This file contains the implementation of the (experimental) networking I/O context operations in the standard library.
 */


module;

#if __has_include(<experimental/io_context>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
#include <experimental/io_context>
#endif

export module stdx:net.io_context;

/**
 * @namespace stdx::net
 * @brief Wrapper namespace for (experimental) standard library networking operations.
 */
export namespace stdx::net {
    #if __has_include(<experimental/io_context>) && STDLIBX_COMPILE_EXPERIMENTAL_HEADERS
    using IOContext = std::experimental::net::io_context;
    using ExecutorType = std::experimental::net::io_context::executor_type;
    #endif
}
