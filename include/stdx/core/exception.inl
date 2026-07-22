#pragma once

/**
 * @namespace stdx::core
 * @brief The core objects of the standard library.
 */
export namespace stdx::core {
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
     * @extends RuntimeException
     * 
     * The ArithmeticException class is used to handle exceptions related to exceptional arithmetic conditions.
     */
    class ArithmeticException: public RuntimeException {
    public:
        using RuntimeException::RuntimeException;
    };

    /**
     * @class InvalidOperationException
     * @brief Exception class for invalid operation errors.
     * @extends RuntimeException
     * 
     * The InvalidOperationException class is used to handle exceptions related to exceptional arithmetic conditions.
     */
    class InvalidOperationException: public RuntimeException {
    public:
        using RuntimeException::RuntimeException;
    };

    /**
     * @class IllegalStateException
     * @brief Exception class for illegal state errors.
     * @extends LogicException
     *
     * The IllegalStateException class is used to handle exceptions related to illegal state errors, such as calling a function at an inappropriate time or when the object is in an inappropriate state.
     */
    class IllegalStateException: public LogicException {
    public:
        using LogicException::LogicException;
    };

    /**
     * @class NullPointerException
     * @brief Exception class for handling null pointer errors.
     * @extends InvalidArgumentException
     * 
     * This class is used to represent an exception that occurs when a null pointer (nullptr) is 
     * unexpectedly encountered and dereferenced or used in a way that leads to an error. 
     * It does not extend RuntimeException (unlike in Java), as passing a null pointer where it should 
     * not be passed is an invalid argument, and does not occur at runtime.
     */
    class NullPointerException: public InvalidArgumentException {
    public:
        using InvalidArgumentException::InvalidArgumentException;
    };

    /**
     * @class NumberFormatException
     * @brief Exception class for calls to inappropriate formatting of numbers.
     * @extends InvalidArgumentException
     * 
     * The NumberFormatException class is used to indicate an attempt to
     * convert a string to a numeric type, but the string does not have the appropriate format.
     */
    class NumberFormatException: public InvalidArgumentException {
    public:
        using InvalidArgumentException::InvalidArgumentException;
    };

    /**
     * @class UnsupportedOperationException
     * @brief Exception class for invalid/unsupported operations.
     * @extends RuntimeException
     * 
     * The UnsupportedOperationException class is used to handle exceptions related to invalid or unsupported operations.
     */
    class UnsupportedOperationException: public RuntimeException {
    public:
        using RuntimeException::RuntimeException;
    };

    /**
     * @class NotImplementedException
     * @brief Exception class for calls to unimplemented functions.
     * @extends UnsupportedOperationException
     * 
     * The NotImplementedException class is used to handle a function call
     * on a function which has not yet been implemented.
     */
    class NotImplementedException: public UnsupportedOperationException {
    public:
        using UnsupportedOperationException::UnsupportedOperationException;
    };

    /**
     * @class NoSuchElementException
     * @brief Exception thrown when attempting to access an element that does not exist.
     * @extends RuntimeException
     */
    class NoSuchElementException: public RuntimeException {
    public:
        using RuntimeException::RuntimeException;
    };
}
