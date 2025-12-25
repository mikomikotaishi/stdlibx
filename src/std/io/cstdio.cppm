/**
 * @file cstdio.cppm
 * @module std:io.cstdio
 * @brief Module file for standard library C-style input/output operations.
 *
 * This file contains the implementation of the C-style input/output operations in the standard library.
 */

module;

#include <cstdio>

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:io.cstdio;
#else
export module stdlib:io.cstdio;
#endif

/**
 * @namespace std::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::io {
#else 
export namespace stdlib::io {
#endif
    using File = std::FILE;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using FPos = std::fpos_t;
    using FPos_t = std::fpos_t;
    using FilePosition_t = std::fpos_t;
    #endif
    using FilePosition = std::fpos_t;

    #ifdef STDLIBX_ENABLE_COMPAT_NAMES
    using ::stdin;
    using ::stdout;
    using ::stderr;
    #endif

    FILE*& Stdin = ::stdin;
    FILE*& Stdout = ::stdout;
    FILE*& Stderr = ::stderr;

    // File access
    using std::fopen;
    using std::freopen;
    using std::fclose;
    using std::fflush;
    using std::setbuf;
    using std::setvbuf;

    // Direct I/O
    using std::fread;
    using std::fwrite;

    // Byte/multibyte character unformatted I/O
    using std::fgetc;
    using std::getc;
    using std::fgets;
    using std::fputc;
    using std::putc;
    using std::getchar;
    using std::putchar;
    using std::puts;
    using std::ungetc;

    // Byte/multibyte formatted I/O
    using std::scanf;
    using std::fscanf;
    using std::sscanf;
    using std::vscanf;
    using std::vfscanf;
    using std::vsscanf;
    using std::printf;
    using std::fprintf;
    using std::sprintf;
    using std::snprintf;
    using std::vprintf;
    using std::vfprintf;
    using std::vsprintf;
    using std::vsnprintf;

    // File positioning
    using std::ftell;
    using std::fgetpos;
    using std::fseek;
    using std::fsetpos;
    using std::rewind;

    // Error handling
    using std::clearerr;
    using std::feof;
    using std::ferror;
    using std::perror;

    // Operations
    using std::remove;
    using std::rename;
    using std::tmpfile;
    using std::tmpnam;

    // Process
    using ::popen;
    using ::pclose;
}
