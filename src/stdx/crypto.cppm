/**
 * @file crypto.cppm
 * @module stdx:crypto
 * @brief Cryptography module providing message digests, signatures, and symmetric encryption.
 */

module;

#ifdef STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY
#include <sodium.h>
#endif

#include "Macros.hpp"

export module stdx:crypto;

import :main;

#include "stdx/crypto/exceptions.inl"
#include "stdx/crypto/constants.inl"
#include "stdx/crypto/keys.inl"
#include "stdx/crypto/random.inl"
#include "stdx/crypto/digest.inl"
#include "stdx/crypto/keypairgen.inl"
#include "stdx/crypto/signature.inl"
#include "stdx/crypto/cipher.inl"
