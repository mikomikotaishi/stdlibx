/**
 * @file cstring.cppm
 * @module stdx:text.string.cstring
 * @brief Module file for standard library C-style string operations.
 *
 * This file contains the implementation of the C-style string operations in the standard library.
 */

module;

export module stdx:text.string.cstring;

import alloc;

/**
 * @namespace stdx::text
 * @brief Wrapper namespace for standard library text operations.
 */
export namespace stdx::text::string {
    using alloc::text::string::strcpy;
    using alloc::text::string::strncpy;
    using alloc::text::string::strcat;
    using alloc::text::string::strncat;
    using alloc::text::string::strxfrm;
    using alloc::text::string::strlen;
    using alloc::text::string::strcmp;
    using alloc::text::string::strncmp;
    using alloc::text::string::strcoll;
    using alloc::text::string::strchr;
    using alloc::text::string::strrchr;
    using alloc::text::string::strspn;
    using alloc::text::string::strcspn;
    using alloc::text::string::strpbrk;
    using alloc::text::string::strstr;
    using alloc::text::string::strtok;
    using alloc::text::string::memchr;
    using alloc::text::string::memcmp;
    using alloc::text::string::memset;
    using alloc::text::string::memcpy;
    using alloc::text::string::memmove;
    using alloc::text::string::strerror;
}
