/**
 * @file sendfile.cppm
 * @module stdx:os.linux.sys.sendfile
 * @brief Module file for Unix sendfile operations.
 *
 * This file contains the implementation of the Unix sendfile operations from the POSIX libraries,
 * located in <sys/sendfile.h>.
 */

module;

#ifdef __linux__
#include <sys/sendfile.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.sendfile;
#else
export module stdlibx:os.linux.sys.sendfile;
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
    using ::sendfile;
    using ::sendfile64;
    #endif
}
