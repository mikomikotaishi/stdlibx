/**
 * @file alloc.cppm
 * @module alloc
 * @brief Module file for standard library operations requiring a memory allocator.
 *
 * This file contains the imports for the operations in the standard library requiring a memory allocator.
 */

module;

#include "Macros.hpp"

#define STDLIBX_NO_STD_FEATURES

export module alloc;

import std;

import core;

using namespace core;

#include "alloc/mem/cstdlib.inl"
#include "alloc/mem/memory.inl"
#include "alloc/mem/memory_resource.inl"
#include "alloc/mem/new.inl"
#include "alloc/mem/scoped_allocator.inl"

#include "alloc/text/regex.inl"
#include "alloc/text/string/string.inl"

#include "alloc/string.inl"

#include "alloc/ranges/memory.inl"
#include "alloc/ranges/ranges.inl"

#include "alloc/collections/vector.inl"
#include "alloc/collections/deque.inl"
#include "alloc/collections/flat_map.inl"
#include "alloc/collections/flat_set.inl"
#include "alloc/collections/hash_map.inl"
#include "alloc/collections/hash_set.inl"
#include "alloc/collections/linked_list.inl"
#include "alloc/collections/queue.inl"
#include "alloc/collections/singly_linked_list.inl"
#include "alloc/collections/stack.inl"
#include "alloc/collections/tree_map.inl"
#include "alloc/collections/tree_set.inl"
#include "alloc/collections/valarray.inl"

#include "alloc/tags.inl"

#if defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_ALLOC_MODULE_EXPORT_CORE();
#endif
