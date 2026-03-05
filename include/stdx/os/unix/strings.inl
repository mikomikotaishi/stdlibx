#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using ::bcmp;
    using ::bcopy;
    using ::bzero;
    using ::index;
    using ::rindex;
    using ::ffs;
    using ::ffsl;
    using ::ffsll;
    using ::strcasecmp;
    using ::strncasecmp;
    using ::strcasecmp_l;
    using ::strncasecmp_l;
    #endif
}
