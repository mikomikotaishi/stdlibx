/**
 * @file libgen.cppm
 * @module stdx:os.unix.libgen
 * @brief Module file for Unix pathname manipulation operations.
 *
 * This file contains the implementation of the Unix pathname manipulation operations from the POSIX libraries,
 * located in <libgen.h>.
 */

module;

#ifdef __unix__
#include <libgen.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.libgen;
#else
export module stdlibx:os.unix.libgen;
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
    using ::dirname;
    #endif
}
