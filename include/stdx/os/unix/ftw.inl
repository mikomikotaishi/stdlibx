#pragma once

/**
 * @namespace stdx::os::unix
 * @brief Wrapper namespace for Unix POSIX operations.
 */
export namespace stdx::os::unix {
    #ifdef __unix__
    using Ftw = ::FTW;

    using ::ftw;
    using ::ftw64;
    using ::nftw;
    using ::nftw64;
    #endif
}
