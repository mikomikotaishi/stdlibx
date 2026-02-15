/**
 * @file iconv.cppm
 * @module stdx:os.unix.iconv
 * @brief Module file for Unix codeset conversion operations.
 *
 * This file contains the implementation of the Unix codeset conversion operations from the POSIX libraries,
 * located in <iconv.h>.
 */

module;

#ifdef __unix__
#include <iconv.h>
#endif

export module stdx:os.unix.iconv;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using ::iconv_close;
    using ::iconv_open;
    using ::iconv;
    #endif
}
