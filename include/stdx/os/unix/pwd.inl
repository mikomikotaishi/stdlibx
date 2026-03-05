#pragma once

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
