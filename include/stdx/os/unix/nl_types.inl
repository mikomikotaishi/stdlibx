#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #if defined(__unix__) && __has_include(<nl_types.h>)
    using ::catopen;
    using ::catgets;
    using ::catclose;
    #endif
}
