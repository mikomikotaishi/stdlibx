/**
 * @file profil.cppm
 * @module stdx:os.linux.sys.profil
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/profil.h>.
 */

module;

#ifdef __linux__
#include <sys/profil.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.profil;
#else
export module stdlibx:os.linux.sys.profil;
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
    using Prof = ::prof;
    #endif

    using Profile = ::prof;

    using ::sprofil;
    #endif
}
