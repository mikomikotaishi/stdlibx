/**
 * @file cstring.cppm
 * @module alloc:text.string.cstring
 * @brief Module file for standard library C-style string operations.
 *
 * This file contains the implementation of the C-style string operations in the standard library.
 */

module;

#include <cstring>

export module alloc:text.string.cstring;

/**
 * @namespace alloc::text
 * @brief Wrapper namespace for standard library string operations.
 */
export namespace alloc::text::string {
    using std::strcpy;
    using std::strncpy;
    using std::strcat;
    using std::strncat;
    using std::strxfrm;
    using std::strlen;
    using std::strcmp;
    using std::strncmp;
    using std::strcoll;
    using std::strchr;
    using std::strrchr;
    using std::strspn;
    using std::strcspn;
    using std::strpbrk;
    using std::strstr;
    using std::strtok;
    using std::memchr;
    using std::memcmp;
    using std::memset;
    using std::memcpy;
    using std::memmove;
    using std::strerror;
}
