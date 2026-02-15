/**
 * @file mtio.cppm
 * @module stdx:os.linux.sys.mtio
 * @brief Module file for Unix magnetic tape I/O operations.
 *
 * This file contains the implementation of the Unix magnetic tape I/O operations from the POSIX libraries,
 * located in <sys/mtio.h>.
 */

module;

#ifdef __linux__
#include <sys/mtio.h>
#endif

export module stdx:os.linux.sys.mtio;

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
