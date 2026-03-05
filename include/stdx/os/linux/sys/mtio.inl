#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using MagneticTapeOperation = ::mtop;
    using MagneticTapeGet = ::mtget;
    using MagneticTapeTapeInfo = ::mt_tape_info;
    using MagneticTapePosition = ::mtpos;
    using MagneticTapeConfigInfo = ::mtconfiginfo;
    #endif
}
