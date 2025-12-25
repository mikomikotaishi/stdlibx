/**
 * @file netinet.cppm
 * @module stdx:os.unix.netinet
 * @brief Module file for Unix network internet operations.
 *
 * This file contains the implementation of the Unix network internet operations from the POSIX libraries,
 * located in all <netinet/*.h> files.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.netinet;
#else
export module stdlibx:os.unix.netinet;
#endif

export import :os.unix.netinet.in;
export import :os.unix.netinet.tcp;
