#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #if defined(__unix__) && __has_include(<fmtmsg.h>)
    using ::fmtmsg;
    using ::addseverity;
    #endif
}
