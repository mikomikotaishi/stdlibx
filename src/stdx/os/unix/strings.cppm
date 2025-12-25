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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.strings;
#else
export module stdlibx:os.unix.strings;
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
