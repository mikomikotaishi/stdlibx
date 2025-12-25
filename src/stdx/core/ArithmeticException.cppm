/**
 * @file ArithmeticException.cppm
 * @module stdx:core.ArithmeticException
 * @brief Implementation of the ArithmeticException class.
 *
 * This file contains the implementation of the ArithmeticException class, which is used to handle exception arithmetic conditions.
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:core.ArithmeticException;
#else
export module stdlibx:core.ArithmeticException;
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
 * @class ArithmeticException
 * @brief Exception class for arithmetic conditions.
 *
 * @extends RuntimeException
 * 
 * The ArithmeticException class is used to handle exceptions related to exceptional arithmetic conditions.
 */
class ArithmeticException: public RuntimeException {
public:
    /**
     * @brief Constructor that initialises the exception with a message.
     * @param msg The exception message.
     */
    explicit ArithmeticException(const String& msg = ""):
        RuntimeException(msg) {}
};

}

STDLIBX_STDX_MODULE_EXPORT_CORE();
