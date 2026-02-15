/**
 * @file alloc.cppm
 * @module alloc
 * @brief Module file for standard library operations requiring a memory allocator.
 *
 * This file contains the imports for the operations in the standard library requiring a memory allocator.
 */

export module alloc;

export import :collections;
export import :mem;
export import :ranges;
export import :string;
export import :tags;
export import :text;

#if defined(STDLIBX_NO_STD) && defined(STDLIBX_IMPLICIT_USING_CORE)
STDLIBX_ALLOC_MODULE_EXPORT_CORE();
#endif
