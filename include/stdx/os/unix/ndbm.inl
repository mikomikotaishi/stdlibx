#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using ::dbm_open;
    using ::dbm_close;
    using ::dbm_fetch;
    using ::dbm_store;
    using ::dbm_delete;
    using ::dbm_firstkey;
    using ::dbm_nextkey;
    using ::dbm_error;
    using ::dbm_clearerr;
    using ::dbm_dirfno;
    using ::dbm_pagfno;
    using ::dbm_rdonly;
    #endif
}
