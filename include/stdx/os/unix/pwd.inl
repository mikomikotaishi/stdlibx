#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #if defined(__unix__) && __has_include(<pwd.h>)
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
