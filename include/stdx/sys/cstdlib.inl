#pragma once

/**
 * @namespace stdx::sys
 * @brief Wrapper namespace for standard library system operations.
 */
export namespace stdx::sys {
    using ::core::sys::abort;
    using ::core::sys::exit;
    using ::core::sys::quick_exit;
    using ::core::sys::_Exit;
    using ::core::sys::atexit;
    using ::core::sys::at_quick_exit;
    using ::core::sys::system;
    using ::core::sys::getenv;
}
