/**
 * @file io.cppm
 * @module std:io
 * @brief Module file for standard library input/output operations.
 *
 * This file contains the imports for the input/output operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:io;
#else
export module stdlib:io;
#endif

export import :io.cstdio;
export import :io.cwchar;
export import :io.fstream;
export import :io.istream;
export import :io.iomanip;
export import :io.ios;
export import :io.iostream;
export import :io.ostream;
export import :io.print;
export import :io.spanstream;
export import :io.sstream;
export import :io.streambuf;
export import :io.string;
export import :io.syncstream;
