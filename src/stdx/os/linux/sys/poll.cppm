/**
 * @file poll.cppm
 * @module stdx:os.linux.sys.poll
 * @brief Module file for Unix System V poll interface compatibility operations.
 *
 * This file contains the implementation of the Unix System V poll interface compatibility operations from the POSIX libraries,
 * located in <sys/poll.h>.
 */

module;

#ifdef __linux__
#include <sys/poll.h>
#endif

export module stdx:os.linux.sys.poll;

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
