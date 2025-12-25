/**
 * @file NotImplementedException.cppm
 * @module stdx:core.NotImplementedException
 * @brief Implementation of the NotImplementedException class.
 *
 * This file contains the implementation of the NotImplementedException class, which is thrown to indicate a called function has not been implemented.
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:core.NotImplementedException;
#else
export module stdlibx:core.NotImplementedException;
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
import std;
#else
import stdlib;
#endif

import :core.UnsupportedOperationException;

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for standard library extension core libraries.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::core {
#else
export namespace stdlibx::core {
#endif

/**
 * @class NotImplementedException
 * @brief Exception class for calls to unimplemented functions.
 *
 * @extends UnsupportedOperationException
 * 
 * The NotImplementedException class is used to handle a function call
 * on a function which has not yet been implemented.
 */
class NotImplementedException: public UnsupportedOperationException {
public:
    /**
     * @brief Constructor that initialises the exception with a message.
     * @param msg The exception message.
     */
    explicit NotImplementedException(const String& msg = ""):
        UnsupportedOperationException(msg) {}
};

}

STDLIBX_STDX_MODULE_EXPORT_CORE();
