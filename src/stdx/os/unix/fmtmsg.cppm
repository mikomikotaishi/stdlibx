/**
 * @file fmtmsg.cppm
 * @module stdx:os.unix.fmtmsg
 * @brief Module file for Unix message display structure operations.
 *
 * This file contains the implementation of the Unix message display structure operations from the POSIX libraries,
 * located in <fmtmsg.h>.
 */

module;

#ifdef __unix__
#include <fmtmsg.h>
#endif

export module stdx:os.unix.fmtmsg;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using ::fmtmsg;
    using ::addseverity;
    #endif
}
