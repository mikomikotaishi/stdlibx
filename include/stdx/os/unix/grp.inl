#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using Group = ::group;

    using ::setgrent;
    using ::endgrent;
    using ::getgrent;
    using ::fgetgrent;
    using ::putgrent;
    using ::getgrgid;
    using ::getgrnam;
    using ::getgrent_r;
    using ::getgrgid_r;
    using ::getgrnam_r;
    using ::fgetgrent_r;
    using ::setgroups;
    using ::getgrouplist;
    using ::initgroups;
    #endif
}
