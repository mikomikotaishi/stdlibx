#pragma once

using stdx::collections::Vector;

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library IO operations.
 */
export namespace stdx::io {
    using ::core::io::IOSBase;
    using ::core::io::IOS;

    using ::core::io::OpenMode;
    using ::core::io::FormatFlags;
    using ::core::io::IOState;
    using ::core::io::SeekingDirection;
    using ::core::io::IOEvent;
    using ::core::io::IOEventCallback;

    using ::core::io::operator&;
    using ::core::io::operator|;
    using ::core::io::operator^;
    using ::core::io::operator~;
    using ::core::io::operator|=;
    using ::core::io::operator&=;
    using ::core::io::operator^=;

    using ::core::io::WideIOS;
    using ::core::io::FilePosition;
    using ::core::io::IOErrc;

    using ::core::io::StreamOffset;
    using ::core::io::StreamSize;

    using ::core::io::IOException;

    using ::alloc::io::ByteBuffer;
    using ::alloc::io::InterruptedIOException;

    using ::core::io::iostream_category;
    using ::core::io::boolalpha;
    using ::core::io::noboolalpha;
    using ::core::io::showbase;
    using ::core::io::noshowbase;
    using ::core::io::showpoint;
    using ::core::io::noshowpoint;
    using ::core::io::showpos;
    using ::core::io::noshowpos;
    using ::core::io::skipws;
    using ::core::io::noskipws;
    using ::core::io::uppercase;
    using ::core::io::nouppercase;
    using ::core::io::unitbuf;
    using ::core::io::nounitbuf;
    using ::core::io::internal;
    using ::core::io::left;
    using ::core::io::right;
    using ::core::io::dec;
    using ::core::io::hex;
    using ::core::io::oct;
    using ::core::io::fixed;
    using ::core::io::scientific;
    using ::core::io::hexfloat;
    using ::core::io::defaultfloat;
}

/**
 * @namespace stdx::meta
 * @brief Wrapper namespace for standard library metaprogramming operations.
 */
export namespace stdx::meta {
    using ::core::meta::IsErrorCodeEnum;
}

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for standard library core operations.
 */
export namespace stdx::core {
    using ::core::prelude::IOErrc;
}
