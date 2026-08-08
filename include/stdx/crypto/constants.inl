#pragma once

using stdx::sync::OnceFlag;

namespace stdx::crypto {
    inline constexpr usize ED25519_PUBLIC_KEY_BYTES = 32; ///< Ed25519 public key size in bytes.
    inline constexpr usize ED25519_PRIVATE_KEY_BYTES = 64; ///< Ed25519 private key size in bytes (seed + public key).
    inline constexpr usize ED25519_SIGNATURE_BYTES = 64; ///< Ed25519 detached signature size in bytes.

    inline constexpr usize SECRETBOX_KEY_BYTES = 32; ///< XSalsa20-Poly1305 key size in bytes.
    inline constexpr usize SECRETBOX_NONCE_BYTES = 24; ///< XSalsa20-Poly1305 nonce size in bytes.
    inline constexpr usize SECRETBOX_MAC_BYTES = 16; ///< XSalsa20-Poly1305 authentication tag size in bytes.

    #ifdef STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY
    /**
     * @brief Ensures libsodium is initialized exactly once across all threads.
     * @throws CryptoInitializationException if {@code sodium_init()} returns a negative value.
     */
    THROWS(CryptoInitializationException)
    void ensure_sodium_initialized() {
        static OnceFlag sodium_init_flag;
        stdx::sync::call_once(sodium_init_flag, [] -> void {
            if (sodium_init() < 0) {
                throw CryptoInitializationException("Failed to initialize libsodium");
            }
        });
    }

    /**
     * @brief Returns {@code true} if {@code actual} matches any of {@code candidates}.
     * @param actual The algorithm name to look up.
     * @param candidates The accepted name variants for a single algorithm.
     * @return {@code true} if a match is found.
     */
    [[nodiscard]]
    bool is_algorithm(StringView actual, Span<const StringView> candidates) {
        for (StringView candidate: candidates) {
            if (actual == candidate) {
                return true;
            }
        }
        return false;
    }

    static_assert(crypto_sign_PUBLICKEYBYTES == ED25519_PUBLIC_KEY_BYTES);
    static_assert(crypto_sign_SECRETKEYBYTES == ED25519_PRIVATE_KEY_BYTES);
    static_assert(crypto_sign_BYTES == ED25519_SIGNATURE_BYTES);

    static_assert(crypto_secretbox_KEYBYTES == SECRETBOX_KEY_BYTES);
    static_assert(crypto_secretbox_NONCEBYTES == SECRETBOX_NONCE_BYTES);
    static_assert(crypto_secretbox_MACBYTES == SECRETBOX_MAC_BYTES);
    #endif
}
