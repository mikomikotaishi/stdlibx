#pragma once

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using ::readv;
    using ::writev;
    using ::preadv;
    using ::pwritev;
    using ::preadv64;
    using ::pwritev64;
    using ::preadv2;
    using ::pwritev2;
    using ::preadv64v2;
    using ::pwritev64v2;
    #endif
}
