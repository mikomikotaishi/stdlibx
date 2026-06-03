#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using Exception = std::exception;
    using NestedException = std::nested_exception;

    /// @extends Exception
    using BadExceptionException = std::bad_exception;
    using LogicException = std::logic_error;
    using RuntimeException = std::runtime_error;

    /// @extends LogicException
    using InvalidDomainException = std::domain_error;
    using InvalidArgumentException = std::invalid_argument;
    using LengthException = std::length_error;
    using OutOfRangeException = std::out_of_range;

    /// @extends RuntimeException
    using InvalidRangeException = std::range_error;
    using OverflowException = std::overflow_error;
    using UnderflowException = std::underflow_error;
    
    using TerminateHandler = std::terminate_handler;

    using ExceptionPointer = std::exception_ptr;

    using std::uncaught_exceptions;
    using std::make_exception_ptr;
    using std::current_exception;
    using std::rethrow_exception;
    using std::throw_with_nested;
    using std::rethrow_if_nested;
    using std::terminate;
    using std::get_terminate;
    using std::set_terminate;

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

    /**
     * @class IllegalStateException
     * @brief Exception class for illegal state errors.
     *
     * @extends LogicException
     *
     * The IllegalStateException class is used to handle exceptions related to illegal state errors, such as calling a function at an inappropriate time or when the object is in an inappropriate state.
     */
    class IllegalStateException: public LogicException {
    public:
        /**
         * @brief Constructor that initialises the exception with a message.
         * @param msg The exception message.
         */
        explicit IllegalStateException(const String& msg = ""):
            LogicException(msg) {}
    };

    /**
     * @class NullPointerException
     * @brief Exception class for handling null pointer errors.
     *
     * @extends InvalidArgumentException
     * 
     * This class is used to represent an exception that occurs when a null pointer (nullptr) is 
     * unexpectedly encountered and dereferenced or used in a way that leads to an error. 
     * It does not extend RuntimeException (unlike in Java), as passing a null pointer where it should 
     * not be passed is an invalid argument, and does not occur at runtime.
     */
    class NullPointerException: public InvalidArgumentException {
    public:
        /**
        * @brief Constructor that initialises the exception with a message.
        * @param msg The exception message.
        */
        explicit NullPointerException(const String& msg = ""):
            InvalidArgumentException(msg) {}
    };

    /**
     * @class NumberFormatException
     * @brief Exception class for calls to inappropriate formatting of numbers.
     *
     * @extends InvalidArgumentException
     * 
     * The NumberFormatException class is used to indicate an attempt to
     * convert a string to a numeric type, but the string does not have the appropriate format.
     */
    class NumberFormatException: public InvalidArgumentException {
    public:
        /**
         * @brief Constructor that initialises the exception with a message.
         * @param msg The exception message.
         */
        explicit NumberFormatException(const String& msg = ""):
            InvalidArgumentException(msg) {}
    };

    /**
     * @class UnsupportedOperationException
     * @brief Exception class for invalid/unsupported operations.
     *
     * @extends RuntimeException
     * 
     * The UnsupportedOperationException class is used to handle exceptions related to invalid or unsupported operations.
     */
    class UnsupportedOperationException: public RuntimeException {
    public:
        /**
         * @brief Constructor that initialises the exception with a message.
         * @param msg The exception message.
         */
        explicit UnsupportedOperationException(const String& msg = ""):
            RuntimeException(msg) {}
    };

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

    /**
     * @class NoSuchElementException
     * @brief Exception thrown when attempting to access an element that does not exist.
     *
     * @extends RuntimeException
     */
    class NoSuchElementException: public RuntimeException {
    public:
        explicit NoSuchElementException(const String& msg = ""):
            RuntimeException(msg) {}
    };
}
