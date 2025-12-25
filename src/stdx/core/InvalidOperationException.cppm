/**
 * @file InvalidOperationException.cppm
 * @module stdx:core.InvalidOperationException
 * @brief Implementation of the InvalidOperationException class.
 *
 * This file contains the implementation of the InvalidOperationException class, which is used to handle exception arithmetic conditions.
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:core.InvalidOperationException;
#else
export module stdlibx:core.InvalidOperationException;
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
import std;
#else
import stdlib;
#endif

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
 * @class InvalidOperationException
 * @brief Exception class for arithmetic conditions.
 *
 * @extends RuntimeException
 * 
 * The InvalidOperationException class is used to handle exceptions related to exceptional arithmetic conditions.
 */
class InvalidOperationException: public RuntimeException {
public:
    /**
     * @brief Constructor that initialises the exception with a message.
     * @param msg The exception message.
     */
    explicit InvalidOperationException(const String& msg = ""):
        RuntimeException(msg) {}
};

}

STDLIBX_STDX_MODULE_EXPORT_CORE();
