#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/gmon.h>)
    using ToStruct = ::tostruct;
    using RawArc = ::rawarc;

    using GmonParameter = ::gmonparam;

    using ::monstartup;
    using ::_mcleanup;
    #endif
}
