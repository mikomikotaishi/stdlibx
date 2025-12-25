/**
 * @file mman.cppm
 * @module stdx:os.unix.sys.mman
 * @brief Module file for Unix memory management operations.
 *
 * This file contains the implementation of the Unix memory management operations from the POSIX libraries,
 * located in <sys/mman.h>.
 */

module;

#ifdef __unix__
#include <sys/mman.h>
#endif

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sys.mman;
#else
export module stdlibx:os.unix.sys.mman;
#endif

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace stdx::os::unix::sys {
#else 
export namespace stdlibx::os::unix::sys {
#endif
    #ifdef __unix__
    using ::mmap;
    using ::mmap64;
    using ::munmap;
    using ::mprotect;
    using ::msync;
    using ::madvise;
    using ::posix_madvise;
    using ::mlock;
    using ::munlock;
    using ::mlockall;
    using ::munlockall;
    using ::mincore;
    using ::mremap;
    using ::remap_file_pages;
    using ::shm_open;
    using ::shm_unlink;
    #endif
}
