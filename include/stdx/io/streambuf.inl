#pragma once

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    using ::core::io::BasicStreamBuffer;
    using ::core::io::StreamBuffer;
    using ::core::io::WideStreamBuffer;

    using ::core::io::swap;
}
