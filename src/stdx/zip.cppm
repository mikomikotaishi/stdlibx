/**
 * @file zip.cppm
 * @module stdx:zip
 * @brief Compression, checksums, and ZIP archive entry support.
 */

module;

#ifdef STDLIBX_EXTENSIONS_COMPILE_ZIP_LIBRARY
#define _BITS_PTHREADTYPES_COMMON_H	1
#include <zlib.h>
#endif

#include "Macros.hpp"

export module stdx:zip;

import :main;

#include "stdx/zip/details.inl"
#include "stdx/zip/exceptions.inl"
#include "stdx/zip/enums.inl"
#include "stdx/zip/checksum.inl"
#include "stdx/zip/entry.inl"
#include "stdx/zip/deflater.inl"
#include "stdx/zip/inflater.inl"
#include "stdx/zip/gzip.inl"
