/**
 * @file ttychars.cppm
 * @module stdx:os.linux.sys.ttychars
 * @brief Module file for Unix terminal handling constants.
 *
 * This file contains the implementation of the Unix terminal handling constants from the POSIX libraries,
 * located in <sys/ttychars.h>.
 */

module;

#ifdef __linux__
#include <sys/ttychars.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.ttychars;
#else
export module stdlibx:os.linux.sys.ttychars;
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
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using TtyChars = ::ttychars;
    #endif

    using TeletypewriterChars = ::ttychars;
    #endif
}
