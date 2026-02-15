/**
 * @file raw.cppm
 * @module stdx:os.linux.sys.raw
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/raw.h>.
 */

module;

#ifdef __linux__
#include <sys/raw.h>
#endif

export module stdx:os.linux.sys.raw;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using RawConfigRequest = ::raw_config_request;
    #endif
}
