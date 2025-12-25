/**
 * @file arpa.cppm
 * @module stdx:os.unix.arpa
 * @brief Module file for Unix ARPA operations.
 *
 * This file contains the implementation of the Unix ARPA operations from the POSIX libraries,
 * located in all <arpa/*.h> files.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.unix.arpa;
#else
export module stdlibx:os.unix.arpa;
#endif

export import :os.unix.arpa.inet;
