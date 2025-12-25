/**
 * @file ios.cppm
 * @module std:io.ios
 * @brief Module file for standard library base input/output operations.
 *
 * This file contains the implementation of the base input/output operations in the standard library.
 */

module;

#include <ios>

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:io.ios;
#else
export module stdlib:io.ios;
#endif

import core;

using core::ErrorCode;
using core::ErrorCondition;

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
 * @namespace std::io
 * @brief Wrapper namespace for standard library IO operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::io {
#else 
export namespace stdlib::io {
#endif
    using core::io::IOSBase;
    using core::io::IOS;

    using core::io::OpenMode;
    using core::io::FormatFlags;
    using core::io::IOState;
    using core::io::SeekingDirection;
    using core::io::IOEvent;
    using core::io::IOEventCallback;

    using core::io::operator&;
    using core::io::operator|;
    using core::io::operator^;
    using core::io::operator~;
    using core::io::operator|=;
    using core::io::operator&=;
    using core::io::operator^=;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::io::WIOS;
    using core::io::FPos;
    #endif

    using core::io::WideIOS;
    using core::io::FilePosition;
    using core::io::IOErrc;
    using core::io::IsErrorCodeEnum;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::io::StreamOff;
    #endif

    using core::io::StreamOffset;
    using core::io::StreamSize;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using core::io::IOSBaseException;
    #endif

    using core::io::IOException;

    using core::io::iostream_category;
    using core::io::make_error_code;
    using core::io::make_error_condition;
    using core::io::boolalpha;
    using core::io::noboolalpha;
    using core::io::showbase;
    using core::io::noshowbase;
    using core::io::showpoint;
    using core::io::noshowpoint;
    using core::io::showpos;
    using core::io::noshowpos;
    using core::io::skipws;
    using core::io::noskipws;
    using core::io::uppercase;
    using core::io::nouppercase;
    using core::io::unitbuf;
    using core::io::nounitbuf;
    using core::io::internal;
    using core::io::left;
    using core::io::right;
    using core::io::dec;
    using core::io::hex;
    using core::io::oct;
    using core::io::fixed;
    using core::io::scientific;
    using core::io::hexfloat;
    using core::io::defaultfloat;
}

/**
 * @namespace std::core
 * @brief Wrapper namespace for standard library core operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::core {
#else 
export namespace stdlib::core {
#endif
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
    using std::io::IOErrc;
    #else
    using stdlib::io::IOErrc;
    #endif
    
    using ::core::make_error_code;
    using ::core::make_error_condition;
}

#ifndef STDLIBX_NO_STD
STDLIBX_STD_MODULE_EXPORT_CORE();
#endif
