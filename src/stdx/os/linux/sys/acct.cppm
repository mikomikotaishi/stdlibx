/**
 * @file acct.cppm
 * @module stdx:os.linux.sys.acct
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/acct.h>.
 */

module;

#ifdef __linux__
#include <sys/acct.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.acct;
#else
export module stdlibx:os.linux.sys.acct;
#endif

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::sys {
#else 
export namespace stdlibx::os::linux::sys {
#endif
    #ifdef __linux__
    // using Account = ::acct;
    using AccountV3 = ::acct_v3;

    using ::acct;
    #endif
}
