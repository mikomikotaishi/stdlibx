/**
 * @file streambuf.cppm
 * @module stdx:io.streambuf
 * @brief Module file for standard library stream buffer operations.
 *
 * This file contains the implementation of the stream buffer operations in the standard library.
 */

module;

export module stdx:io.streambuf;

import core;

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    using core::io::BasicStreamBuffer;
    using core::io::StreamBuffer;
    using core::io::WideStreamBuffer;

    using core::io::swap;
}
