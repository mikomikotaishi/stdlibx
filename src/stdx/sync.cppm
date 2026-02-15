/**
 * @file sync.cppm
 * @module stdx:sync
 * @brief Module file for standard library synchronisation operations.
 *
 * This file contains the imports for the synchronisation operations in the standard library.
 */

module;

export module stdx:sync;

export import :sync.atomic;
export import :sync.barrier;
export import :sync.condition_variable;
export import :sync.latch;
export import :sync.mutex;
export import :sync.semaphore;
export import :sync.shared_mutex;
