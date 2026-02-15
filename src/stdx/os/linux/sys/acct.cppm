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

export module stdx:os.linux.sys.acct;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    // using Account = ::acct;
    using AccountV3 = ::acct_v3;

    using ::acct;
    #endif
}
