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

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.sys.mtio;
#else
export module stdlibx:os.linux.sys.mtio;
#endif

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::linux::sys {
#else 
export namespace stdlibx::os::linux::sys {
#endif
    #ifdef __linux__
    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using MTOp = ::mtop;
    using MTGet = ::mtget;
    using MTTapeInfo = ::mt_tape_info;
    using MTPos = ::mtpos;
    using MTConfigInfo = ::mtconfiginfo;
    #endif

    using MagneticTapeOperation = ::mtop;
    using MagneticTapeGet = ::mtget;
    using MagneticTapeTapeInfo = ::mt_tape_info;
    using MagneticTapePosition = ::mtpos;
    using MagneticTapeConfigInfo = ::mtconfiginfo;
    #endif
}
