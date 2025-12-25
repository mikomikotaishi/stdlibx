/**
 * @file arpa.cppm
 * @module stdx:os.linux.arpa
 * @brief Module file for Unix ARPA operations.
 *
 * This file contains the implementation of the Unix ARPA operations from the POSIX libraries,
 * located in all <arpa/*.h> files.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:os.linux.arpa;
#else
export module stdlibx:os.linux.arpa;
#endif

export import :os.linux.arpa.ftp;
export import :os.linux.arpa.nameser;
export import :os.linux.arpa.telnet;
export import :os.linux.arpa.tftp;
