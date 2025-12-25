/**
 * @file crypto.cppm
 * @module stdx:crypto
 * @brief Import of cryptography modules.
 *
 * This file imports the modules for the cryptography library.
 */

module;

#if defined(STDLIBX_NO_RESERVED_STD_MODULE) || defined(DOXYGEN)
export module stdx:crypto;
#else
export module stdlibx:crypto;
#endif

export import :crypto.Cipher;
