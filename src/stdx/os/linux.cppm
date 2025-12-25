/**
 * @file linux.cppm
 * @module stdx:os.linux
 * @brief Module file for Linux operations.
 *
 * This file contains the implementation of the Linux operations.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux;
#else
export module stdlibx:os.linux;
#endif

export import :os.linux.arpa;
export import :os.linux.asmgeneric;
export import :os.linux.net;
export import :os.linux.netinet;
export import :os.linux.sys;
