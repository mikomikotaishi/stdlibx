/**
 * @file net.cppm
 * @module std:net
 * @brief Module file for standard library networking operations.
 *
 * This file contains the imports for the networking operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:net;
#else
export module stdlib:net;
#endif

export import :net.buffer;
export import :net.executor;
export import :net.internet;
export import :net.io_context;
export import :net.socket;
export import :net.timer;
