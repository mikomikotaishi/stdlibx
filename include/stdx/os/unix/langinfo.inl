#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #if defined(__unix__) && __has_include(<langinfo.h>)
    using ::nl_langinfo;
    using ::nl_langinfo_l;
    #endif
}
