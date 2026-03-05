#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using AsynchronousIOControlBlock = ::aiocb;
    using AsynchronousIOControlBlock64 = ::aiocb64;
    using AsynchronousIOInit = ::aioinit;

    using ::aio_init;
    using ::aio_read;
    using ::aio_write;
    using ::lio_listio;
    using ::aio_error;
    using ::aio_return;
    using ::aio_cancel;
    using ::aio_suspend;
    using ::aio_fsync;

    using ::aio_read64;
    using ::aio_write64;
    using ::lio_listio64;
    using ::aio_error64;
    using ::aio_return64;
    using ::aio_cancel64;
    using ::aio_suspend64;
    using ::aio_fsync64;
    #endif
}
