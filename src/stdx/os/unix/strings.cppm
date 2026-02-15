/**
 * @file strings.cppm
 * @module stdx:os.unix.strings
 * @brief Module file for Unix string comparison operations.
 *
 * This file contains the implementation of the Unix string comparison operations from the POSIX libraries,
 * located in <strings.h>.
 */

module;

#ifdef __unix__
#include <strings.h>
#endif

export module stdx:os.unix.strings;

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
