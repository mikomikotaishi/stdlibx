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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.fmtmsg;
#else
export module stdlibx:os.unix.fmtmsg;
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
    using ::fmtmsg;
    using ::addseverity;
    #endif
}
