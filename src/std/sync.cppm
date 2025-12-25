/**
 * @file sync.cppm
 * @module std:sync
 * @brief Module file for standard library synchronisation operations.
 *
 * This file contains the imports for the synchronisation operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:sync;
#else
export module stdlib.sync;
#endif

export import :sync.atomic;
export import :sync.barrier;
export import :sync.condition_variable;
export import :sync.latch;
export import :sync.mutex;
export import :sync.semaphore;
export import :sync.shared_mutex;
