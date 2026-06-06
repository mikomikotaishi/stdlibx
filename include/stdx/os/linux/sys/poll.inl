#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/poll.h>)
    using PollFileDescriptor = ::pollfd;
    using FileDescriptorCount = ::nfds_t;

    using ::poll;
    using ::ppoll;
    #endif
}
