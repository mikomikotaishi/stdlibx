/**
 * @file net.cppm
 * @module stdx:os.unix.net
 * @brief Module file for Unix networking operations.
 *
 * This file contains the implementation of the Unix networking operations from the POSIX libraries,
 * located in all <net/*.h> files.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.net;
#else
export module stdlibx:os.unix.net;
#endif

export import :os.unix.net.if_header;
