#pragma once

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using ::ioctl;

    using WindowSize = ::winsize;
    // using TerminalIO = ::termio;
    #endif
}
