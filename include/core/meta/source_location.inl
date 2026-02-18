#pragma once

/**
 * @namespace core::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace core::meta {
    #if __has_include(<source_location>)
    using SourceLocation = std::source_location;
    #endif
}
