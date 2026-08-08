#pragma once

/**
 * @namespace stdx::os::darwin::sys
 * @brief Resolve a filesystem object id back to a path - <sys/fsgetpath.h>.
 */
export namespace stdx::os::darwin::sys {
    #if defined(__APPLE__) && __has_include(<sys/fsgetpath.h>)
    using FileSystemId = ::fsid_t;

    using ::fsgetpath;
    #endif
}
