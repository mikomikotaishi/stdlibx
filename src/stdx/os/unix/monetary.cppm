/**
 * @file monetary.cppm
 * @module stdx:os.unix.monetary
 * @brief Module file for Unix monetary unit string formatting operations.
 *
 * This file contains the implementation of the Unix monetary unit string formatting operations from the POSIX libraries,
 * located in <monetary.h>.
 */

module;

#ifdef __unix__
#include <monetary.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.monetary;
#else
export module stdlibx:os.unix.monetary;
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
    using ::strfmon;
    using ::strfmon_l;
    #endif
}
