#pragma once

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    #if __has_include(<print>)
    using std::print;
    using std::println;
    using std::vprint_unicode;
    using std::vprint_nonunicode;
    #endif
}
