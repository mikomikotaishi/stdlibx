/**
 * @file nl_types.cppm
 * @module stdx:os.unix.nl_types
 * @brief Module file for Unix localisation message catalogue operations.
 *
 * This file contains the implementation of the Unix localisation message catalogue operations from the POSIX libraries,
 * located in <nl_types.h>.
 */

module;

#ifdef __unix__
#include <nl_types.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.nl_types;
#else
export module stdlibx:os.unix.nl_types;
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
    using ::catopen;
    using ::catgets;
    using ::catclose;
    #endif
}
