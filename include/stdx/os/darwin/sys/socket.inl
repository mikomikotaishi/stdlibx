#pragma once

/**
 * @internal
 * @namespace stdx::os::darwin::captured
 * @brief Apple's <sys/socket.h> extensions, read where they exist.
 *
 * Only what POSIX does not have. The socket options whose *values* differ on
 * Darwin keep their POSIX names in unix.constants.inl, which reads them from the
 * same header through stdx::os::unix::sys::captured; duplicating them here would
 * give stdx::net a third platform branch for no gain.
 */
namespace stdx::os::darwin::captured {
    #if defined(__APPLE__) && __has_include(<sys/socket.h>)
    constexpr auto SO_NOSIGPIPE_VALUE = SO_NOSIGPIPE;
    constexpr auto SO_NREAD_VALUE = SO_NREAD;
    #endif
}
