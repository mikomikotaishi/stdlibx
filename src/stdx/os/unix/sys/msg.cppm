/**
 * @file msg.cppm
 * @module stdx:os.unix.sys.msg
 * @brief Module file for Unix message queue operations.
 *
 * This file contains the implementation of the Unix message queue operations from the POSIX libraries,
 * located in <sys/msg.h>.
 */

module;

#ifdef __unix__
#include <sys/msg.h>
#endif

export module stdx:os.unix.sys.msg;

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using MessageBuffer = ::msgbuf;

    using ::msgctl;
    using ::msgget;
    using ::msgrcv;
    using ::msgsnd;
    #endif
}
