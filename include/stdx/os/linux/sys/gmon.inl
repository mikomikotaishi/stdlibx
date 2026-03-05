#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ToStruct = ::tostruct;
    using RawArc = ::rawarc;

    using GmonParameter = ::gmonparam;

    using ::monstartup;
    using ::_mcleanup;
    #endif
}
