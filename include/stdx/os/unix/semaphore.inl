#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using ::sem_init;
    using ::sem_destroy;
    using ::sem_open;
    using ::sem_close;
    using ::sem_unlink;
    using ::sem_wait;
    using ::sem_timedwait;
    using ::sem_clockwait;
    using ::sem_trywait;
    using ::sem_post;
    using ::sem_getvalue;
    #endif
}
