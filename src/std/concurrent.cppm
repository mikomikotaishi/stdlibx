/**
 * @file concurrent.cppm
 * @module std:concurrent
 * @brief Module file for standard library concurrency operations.
 *
 * This file contains the imports for the concurrency operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:concurrent;
#else
export module stdlib:concurrent;
#endif

export import :concurrent.coroutine;
export import :concurrent.future;
export import :concurrent.stop_token;
export import :concurrent.thread;
