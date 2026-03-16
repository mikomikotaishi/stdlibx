#pragma once

/**
 * @namespace stdx::core
 * @brief Wrapper namespace for the core objects of the standard library.
 */
export namespace stdx::core {
    using ::core::prelude::Exception;
    using ::core::prelude::NestedException;

    /// @extends Exception
    using ::core::prelude::BadExceptionException;
    using ::core::prelude::LogicException;
    using ::core::prelude::RuntimeException;

    /// @extends LogicException
    using ::core::prelude::InvalidDomainException;
    using ::core::prelude::InvalidArgumentException;
    using ::core::prelude::LengthException;
    using ::core::prelude::OutOfRangeException;

    /// @extends RuntimeException
    using ::core::prelude::InvalidRangeException;
    using ::core::prelude::OverflowException;
    using ::core::prelude::UnderflowException;

    using ::core::prelude::ArithmeticException;
    using ::core::prelude::InvalidOperationException;
    using ::core::prelude::IllegalStateException;
    using ::core::prelude::NullPointerException;
    using ::core::prelude::NumberFormatException;
    using ::core::prelude::UnsupportedOperationException;
    using ::core::prelude::NotImplementedException;
    using ::core::prelude::NoSuchElementException;
    
    using ::core::prelude::TerminateHandler;

    using ::core::prelude::ExceptionPointer;

    using ::core::prelude::uncaught_exceptions;
    using ::core::prelude::make_exception_ptr;
    using ::core::prelude::current_exception;
    using ::core::prelude::rethrow_exception;
    using ::core::prelude::throw_with_nested;
    using ::core::prelude::rethrow_if_nested;
    using ::core::prelude::terminate;
    using ::core::prelude::get_terminate;
    using ::core::prelude::set_terminate;
}
