#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #if defined(__unix__) && __has_include(<mqueue.h>)
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
