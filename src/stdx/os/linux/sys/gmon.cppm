/**
 * @file gmon.cppm
 * @module stdx:os.linux.sys.gmon
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/gmon.h>.
 */

module;

#ifdef __linux__
#include <sys/gmon.h>
#endif

export module stdx:os.linux.sys.gmon;

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
