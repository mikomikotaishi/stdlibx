/**
 * @file select.cppm
 * @module stdx:os.unix.sys.select
 * @brief Module file for Unix fd_set type and select/pselect declaration operations.
 *
 * This file contains the implementation of the Unix fd_set type and select/pselect declaration operations from the POSIX libraries,
 * located in <sys/select.h>.
 */

module;

#ifdef __unix__
#include <sys/select.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sys.select;
#else
export module stdlibx:os.unix.sys.select;
#endif

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix::sys {
#else 
export namespace stdlibx::os::unix::sys {
#endif
    #ifdef __unix__
    using ::select;
    using ::pselect;
    #endif
}
