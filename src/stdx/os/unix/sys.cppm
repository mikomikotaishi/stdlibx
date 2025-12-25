/**
 * @file sys.cppm
 * @module stdx:os.unix.sys
 * @brief Module file for Unix system operations.
 *
 * This file contains the implementation of the Unix system operations from the POSIX libraries,
 * located in all <sys/*.h> files.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.sys;
#else
export module stdlibx:os.unix.sys;
#endif

export import :os.unix.sys.ioctl;
export import :os.unix.sys.ipc;
export import :os.unix.sys.mman;
export import :os.unix.sys.mount;
export import :os.unix.sys.msg;
export import :os.unix.sys.resource;
export import :os.unix.sys.select;
export import :os.unix.sys.sem;
export import :os.unix.sys.shm;
export import :os.unix.sys.socket;
export import :os.unix.sys.stat;
export import :os.unix.sys.statvfs;
export import :os.unix.sys.time;
export import :os.unix.sys.times;
export import :os.unix.sys.ttydefaults;
export import :os.unix.sys.uio;
export import :os.unix.sys.un;
export import :os.unix.sys.utsname;
export import :os.unix.sys.wait;
