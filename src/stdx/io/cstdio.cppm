/**
 * @file cstdio.cppm
 * @module stdx:io.cstdio
 * @brief Module file for standard library C-style input/output operations.
 *
 * This file contains the implementation of the C-style input/output operations in the standard library.
 */

module;

#include <cstdio>

export module stdx:io.cstdio;

/**
 * @namespace stdx::io
 * @brief Wrapper namespace for standard library input/output operations.
 */
export namespace stdx::io {
    using File = std::FILE;
    using FilePosition = std::fpos_t;

    File* const Stdin = ::stdin;
    File* const Stdout = ::stdout;
    File* const Stderr = ::stderr;

    File* stdin() noexcept {
        return Stdin;
    }

    File* stdout() noexcept {
        return Stdout;
    }

    File* stderr() noexcept {
        return Stderr;
    }

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
