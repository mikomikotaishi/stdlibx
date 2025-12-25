/**
 * @file wordexp.cppm
 * @module stdx:os.unix.wordexp
 * @brief Module file for Unix word expansion operations.
 *
 * This file contains the implementation of the Unix word expansion operations from the POSIX libraries,
 * located in <wordexp.h>.
 */

module;

#ifdef __unix__
#include <wordexp.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.wordexp;
#else
export module stdlibx:os.unix.wordexp;
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
    using ::wordexp;
    using ::wordfree;
    #endif
}
