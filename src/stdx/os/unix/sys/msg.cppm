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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sys.msg;
#else
export module stdlibx:os.unix.sys.msg;
#endif

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix::sys {
#else 
export namespace stdlibx::os::unix::sys {
#endif
    #ifdef __unix__
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using MsgBuf = ::msgbuf;
    #endif

    using MessageBuffer = ::msgbuf;

    using ::msgctl;
    using ::msgget;
    using ::msgrcv;
    using ::msgsnd;
    #endif
}
