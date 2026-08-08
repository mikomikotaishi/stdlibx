#pragma once

/**
 * @namespace stdx::os::darwin::sys
 * @brief Darwin mount and statfs - <sys/mount.h>.
 *
 * struct statfs is a different shape here than on Linux, and getmntinfo has no
 * Linux counterpart at all.
 */
export namespace stdx::os::darwin::sys {
    #if defined(__APPLE__) && __has_include(<sys/mount.h>)
    using FileSystemStatistics = struct statfs;
    using FileSystemId = ::fsid_t;
    using FileHandle = ::fhandle_t;
    using VolumeAttributes = ::vfsconf;

    using ::statfs;
    using ::fstatfs;
    using ::getfsstat;
    using ::getmntinfo;
    using ::getmntinfo_r_np;
    using ::mount;
    using ::unmount;
    using ::fmount;
    using ::fhopen;
    using ::getfh;
    #endif
}
