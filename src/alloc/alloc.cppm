/**
 * @file alloc.cppm
 * @module alloc
 * @brief Module file for standard library operations requiring a memory allocator.
 *
 * This file contains the imports for the operations in the standard library requiring a memory allocator.
 */

module;

// Needed for feature testing macros
#include <version>

#include "Macros.hpp"

#define STDLIBX_NO_STD_FEATURES

export module alloc;

import std;

import core;

STDLIBX_CORE_PREPARE_IMPORT_PRELUDE();
STDLIBX_CORE_PREPARE_IMPORT_LITERALS();

#include "alloc/cstdlib.inl"
#include "alloc/memory.inl"
#include "alloc/new.inl"
#include "alloc/scoped_allocator.inl"

#include "alloc/thread/hazard_pointer.inl"

#include "alloc/mem/cstring.inl"
#include "alloc/mem/cwchar.inl"
#include "alloc/mem/memory.inl"
#include "alloc/mem/new.inl"

#include "alloc/text/regex.inl"
#include "alloc/text/string/string.inl"

#include "alloc/pmr/collections.inl"
#include "alloc/pmr/memory.inl"
#include "alloc/pmr/memory_resource.inl"
#include "alloc/pmr/regex.inl"
#include "alloc/pmr/string.inl"

#include "alloc/string.inl"
#include "alloc/exception.inl"

#include "alloc/ranges/memory.inl"
#include "alloc/ranges/ranges.inl"

#include "alloc/collections/colony.inl"
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

#include "alloc/io/ios.inl"

#ifndef STDLIBX_EXPORT_IMPORT_STD
#include "operators.inl"
#endif

#if defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
export using namespace core::prelude;
export using namespace core::prelude::util;
#endif

#if defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_LITERALS)
export using namespace core::literals;
#endif
