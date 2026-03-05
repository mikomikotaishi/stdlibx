#pragma once

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
    #ifdef __unix__
    using ::shmctl;
    using ::shmget;
    using ::shmat;
    using ::shmdt;
    #endif
}
