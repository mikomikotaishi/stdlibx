#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
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
