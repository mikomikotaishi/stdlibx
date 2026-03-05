#pragma once

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using StatVfs = struct statvfs;
    using StatVfs64 = struct statvfs64;

    using ::statvfs;
    using ::statvfs64;
    using ::fstatvfs;
    using ::fstatvfs64;
    #endif
}
