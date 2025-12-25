/**
 * @file fs.cppm
 * @module std:fs
 * @brief Module file for standard library file system operations.
 *
 * This file contains the imports for the file system operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:fs;
#else
export module stdlib:fs;
#endif

export import :fs.filesystem;
export import :fs.glob;
