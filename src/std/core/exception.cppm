/**
 * @file exceptions.cppm
 * @module std:core.exceptions
 * @brief Module file for importing exceptions from the standard library.
 *
 * This file contains the implementation of the exception operations in the standard library.
 */

module;

#include "Macros.hpp"

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:core.exceptions;
#else
export module stdlib:core.exceptions;
#endif

import core;

/**
 * @namespace std::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::core {
#else 
export namespace stdlib::core {
#endif
    using ::core::Exception;
    using ::core::NestedException;

    /// @extends Exception
    using ::core::BadExceptionException;
    using ::core::LogicException;
    using ::core::RuntimeException;

    /// @extends LogicException
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using ::core::DomainException;
    #endif
    using ::core::InvalidDomainException;
    using ::core::InvalidArgumentException;
    using ::core::LengthException;
    using ::core::OutOfRangeException;

    /// @extends RuntimeException
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using ::core::RangeException;
    #endif
    using ::core::InvalidRangeException;
    using ::core::OverflowException;
    using ::core::UnderflowException;
    
    using ::core::TerminateHandler;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using ::core::ExceptionPtr;
    #endif

    using ::core::ExceptionPointer;

    using ::core::uncaught_exceptions;
    using ::core::make_exception_ptr;
    using ::core::current_exception;
    using ::core::rethrow_exception;
    using ::core::throw_with_nested;
    using ::core::rethrow_if_nested;
    using ::core::terminate;
    using ::core::get_terminate;
    using ::core::set_terminate;
}

#if !defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_STD_MODULE_EXPORT_CORE();
#endif
