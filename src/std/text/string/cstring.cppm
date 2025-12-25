/**
 * @file cstring.cppm
 * @module std:text.string.cstring
 * @brief Module file for standard library C-style string operations.
 *
 * This file contains the implementation of the C-style string operations in the standard library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module std:text.string.cstring;
#else
export module stdlib:text.string.cstring;
#endif

import alloc;

/**
 * @namespace std::text
 * @brief Wrapper namespace for standard library text operations.
 */
#if defined(STDLIBX_NO_RESERVED_STD_NAMESPACE) || defined(DOXYGEN)
export namespace std::text::string {
#else 
export namespace stdlib::text::string {
#endif
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
