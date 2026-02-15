/**
 * @file pwd.cppm
 * @module stdx:os.unix.pwd
 * @brief Module file for Unix password access/control operations.
 *
 * This file contains the implementation of the Unix password access/control operations from the POSIX libraries,
 * located in <pwd.h>.
 */

module;

#ifdef __unix__
#include <pwd.h>
#endif

export module stdx:os.unix.pwd;

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using Password = ::passwd;

    using ::setpwent;
    using ::endpwent;
    using ::getpwent;
    using ::fgetpwent;
    using ::putpwent;
    using ::getpwuid;
    using ::getpwnam;
    using ::getpwent_r;
    using ::getpwuid_r;
    using ::getpwnam_r;
    using ::fgetpwent_r;
    using ::getpw;
    #endif
}
