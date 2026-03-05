#pragma once

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using ::wait;
    using ::waitpid;
    using ::waitid;
    using ::wait3;
    using ::wait4;
    #endif
}
