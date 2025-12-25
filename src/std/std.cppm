/**
 * @file std.cppm
 * @module std
 * @brief Module file for the entire standard library.
 *
 * This file imports all the standard library modules.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std;
#else
export module stdlib;
#endif

export import :core;
export import :collections;
export import :concurrent;
export import :execution;
export import :fmt;
export import :fs;
export import :io;
export import :iter;
export import :math;
export import :mem;
export import :meta;
export import :net;
export import :random;
export import :ranges;
export import :sync;
export import :sys;
export import :text;
export import :time;
export import :util;
