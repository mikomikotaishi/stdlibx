/**
 * @file ios.cppm
 * @module stdx:io.ios
 * @brief Module file for standard library base input/output operations.
 *
 * This file contains the implementation of the base input/output operations in the standard library.
 */

module;

#include <ios>

#include "Macros.hpp"

export module stdx:io.ios;

import :core;

import core;

using namespace stdx::core;

export {
    using std::operator|;
    using std::operator&;
    using std::operator^;
    using std::operator~;
    using std::operator|=;
    using std::operator&=;
    using std::operator^=;
}

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
    using ::core::io::IsErrorCodeEnum;

    using ::core::io::StreamOffset;
    using ::core::io::StreamSize;

    using ::core::io::IOException;

    /**
     * @class InterruptedIOException
     * @brief Exception class for handling interrupted I/O operations.
     *
     * @extends IOException
     * 
     * The InterruptedIOException class is used to represent an exception that occurs when an I/O operation is interrupted.
     */
    class InterruptedIOException: public IOException {
    public:
        /**
         * @brief Constructor that initialises the exception with a message.
         * @param msg The exception message.
         */
        explicit InterruptedIOException(const String& msg = ""):
            IOException(msg) {}
    };

    using ::core::io::iostream_category;
    using ::core::io::make_error_code;
    using ::core::io::make_error_condition;
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
 * @namespace stdx::core
 * @brief Wrapper namespace for standard library core operations.
 */
export namespace stdx::core {
    using stdx::io::IOErrc;
    
    using ::core::make_error_code;
    using ::core::make_error_condition;
}
