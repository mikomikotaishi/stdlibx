/**
 * @file aio.cppm
 * @module stdx:os.unix.aio
 * @brief Module file for Unix asynchronous I/O operations.
 *
 * This file contains the implementation of the Unix asynchronous I/O operations from the POSIX libraries,
 * located in <aio.h>.
 */

module;

#ifdef __unix__
#include <aio.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.aio;
#else
export module stdlibx:os.unix.aio;
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
    using AIOCB = ::aiocb;
    using AIOCB64 = ::aiocb64;
    using AIOInit = ::aioinit;

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
