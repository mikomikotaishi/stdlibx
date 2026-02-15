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

export module stdx:os.linux.sys.profil;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using Profile = ::prof;

    using ::sprofil;
    #endif
}
