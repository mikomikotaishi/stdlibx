#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #if defined(__unix__) && __has_include(<iconv.h>)
    using ::iconv_close;
    using ::iconv_open;
    using ::iconv;
    #endif
}
