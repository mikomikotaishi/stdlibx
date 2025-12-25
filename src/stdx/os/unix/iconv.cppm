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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.iconv;
#else
export module stdlibx:os.unix.iconv;
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
    using ::iconv_close;
    using ::iconv_open;
    using ::iconv;
    #endif
}
