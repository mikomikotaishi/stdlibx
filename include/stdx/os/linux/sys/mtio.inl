#pragma once

/**
 * @namespace stdx::os::linux::sys
 * @brief Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #if defined(__linux__) && __has_include(<sys/mtio.h>)
    using MagneticTapeOperation = ::mtop;
    using MagneticTapeGet = ::mtget;
    using MagneticTapeTapeInfo = ::mt_tape_info;
    using MagneticTapePosition = ::mtpos;
    using MagneticTapeConfigInfo = ::mtconfiginfo;
    #endif
}
