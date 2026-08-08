#pragma once

/**
 * @namespace stdx::os::darwin
 * @brief Pseudo-terminal and utmpx helpers - <util.h>.
 */
export namespace stdx::os::darwin {
    #if defined(__APPLE__) && __has_include(<util.h>)
    using ::openpty;
    using ::forkpty;
    using ::login_tty;
    using ::opendev;
    using ::fparseln;
    #endif
}
