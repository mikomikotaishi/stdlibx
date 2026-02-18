#pragma once

/**
 * @namespace stdx::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace stdx::meta {
    #if __has_include(<source_location>)
    using ::core::meta::SourceLocation;
    #endif
}
