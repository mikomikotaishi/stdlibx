/**
 * @file klog.cppm
 * @module stdx:os.linux.sys.klog
 * @brief Module file for Unix operations.
 *
 * This file contains the implementation of the Unix operations from the POSIX libraries,
 * located in <sys/klog.h>.
 */

module;

#ifdef __linux__
#include <sys/klog.h>
#endif

export module stdx:os.linux.sys.klog;

/**
 * @namespace stdx::os::linux::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::linux::sys {
    #ifdef __linux__
    using ::klogctl;
    #endif
}
