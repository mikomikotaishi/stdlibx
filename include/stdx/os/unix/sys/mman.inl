#pragma once

/**
 * @namespace stdx::os::unix::sys
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix::sys {
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
