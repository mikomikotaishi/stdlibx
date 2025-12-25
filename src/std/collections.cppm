/**
 * @file collections.cppm
 * @module std:collections
 * @brief Module file for standard library collection operations.
 *
 * This file contains the imports for the collection operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:collections;
#else
export module stdlib:collections;
#endif

export import :collections.bitset;
export import :collections.deque;
export import :collections.hash_map;
export import :collections.hash_set;
export import :collections.linked_list;
export import :collections.queue;
export import :collections.singly_linked_list;
export import :collections.stack;
export import :collections.tree_map;
export import :collections.tree_set;
export import :collections.valarray;
export import :collections.vector;
