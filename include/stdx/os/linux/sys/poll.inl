#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using PollFileDescriptor = ::pollfd;

    using ::poll;
    using ::ppoll;
    #endif
}
