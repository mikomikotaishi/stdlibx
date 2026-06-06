#pragma once

/**
 * @namespace stdx::sys
 * @brief Standard library system operations.
 */
export namespace stdx::sys {
    using std::abort;
    using std::exit;
    using std::quick_exit;
    using std::_Exit;
    using std::atexit;
    using std::at_quick_exit;
    using std::system;
    using std::getenv;
}
