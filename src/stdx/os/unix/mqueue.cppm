/**
 * @file cpio.cppm
 * @module stdx:os.unix.mqueue
 * @brief Module file for Unix message queue operations.
 *
 * This file contains the implementation of the Unix message queue operations from the POSIX libraries,
 * located in <mqueue.h>.
 */

module;

#ifdef __unix__
#include <mqueue.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.mqueue;
#else
export module stdlibx:os.unix.mqueue;
#endif

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix {
#else 
export namespace stdlibx::os::unix {
#endif
    #ifdef __unix__
    using ::mq_open;
    using ::mq_close;
    using ::mq_getattr;
    using ::mq_setattr;
    using ::mq_unlink;
    using ::mq_notify;
    using ::mq_receive;
    using ::mq_send;
    using ::mq_timedreceive;
    using ::mq_timedsend;
    #endif
}
