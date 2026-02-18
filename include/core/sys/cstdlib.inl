#pragma once

/**
 * @namespace core::sys
 * @brief Wrapper namespace for standard library system operations.
 */
export namespace core::sys {
    using std::abort;
    using std::exit;
    using std::quick_exit;
    using std::_Exit;
    using std::atexit;
    using std::at_quick_exit;
    using std::system;
    using std::getenv;
}
