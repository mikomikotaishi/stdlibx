/**
 * @file langinfo.cppm
 * @module stdx:os.unix.langinfo
 * @brief Module file for Unix language information operations.
 *
 * This file contains the implementation of the Unix language information operations from the POSIX libraries,
 * located in <langinfo.h>.
 */

module;

#ifdef __unix__
#include <langinfo.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.langinfo;
#else
export module stdlibx:os.unix.langinfo;
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
    using ::nl_langinfo;
    using ::nl_langinfo_l;
    #endif
}
