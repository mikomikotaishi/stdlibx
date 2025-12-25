/**
 * @file mem.cppm
 * @module std:mem
 * @brief Module file for standard library memory operations.
 *
 * This file contains the imports for the memory operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:mem;
#else
export module stdlib:mem;
#endif

export import :mem.cstdlib;
export import :mem.memory;
export import :mem.memory_resource;
export import :mem.new_header;
export import :mem.scoped_allocator;
