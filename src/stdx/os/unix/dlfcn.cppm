/**
 * @file dlfcn.cppm
 * @module stdx:os.unix.dlfcn
 * @brief Module file for Unix dynamic linking operations.
 *
 * This file contains the implementation of the Unix dynamic linking operations from the POSIX libraries,
 * located in <dlfcn.h>.
 */

module;

#ifdef __unix__
#include <dlfcn.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.dlfcn;
#else
export module stdlibx:os.unix.dlfcn;
#endif

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix {
#else 
export namespace stdlibx::os::unix {
#endif
    #ifdef __unix__
    using ::dlopen;
    using ::dlclose;
    using ::dlsym;
    using ::dlmopen;
    using ::dlvsym;
    using ::dlerror;
    using ::dladdr;
    using ::dladdr1;
    using ::dlinfo;

    using DLFindObject = ::dl_find_object;
    #endif
}
