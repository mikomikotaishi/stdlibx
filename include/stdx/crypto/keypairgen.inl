#pragma once

using stdx::io::ByteBuffer;

#ifdef STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY
/**
 * @namespace stdx::crypto
 * @brief Cryptography operations: hashing, signing, and symmetric encryption.
 */
export namespace stdx::crypto {
    /**
     * @class KeyPairGenerator
     * @brief Generates asymmetric key pairs.
     *
     * @code{.cpp}
     * // Enum-based (preferred when algorithm is known at compile time):
     * KeyPairGenerator kpg = KeyPairGenerator::instance(KeyPairGenerator::Algorithm::ED25519);
     *
     * // String-based (useful when algorithm comes from config/data):
     * KeyPairGenerator kpg = KeyPairGenerator::instance("Ed25519");
     *
     * KeyPair kp = kpg.generate_key_pair();
     * @endcode
     */
    class KeyPairGenerator {
    public:
        /**
         * @enum Algorithm
         * @brief Compile-time-safe selector for {@code KeyPairGenerator} algorithms.
         */
        enum class Algorithm: u8 {
            ED25519, ///< Edwards-curve Digital Signature Algorithm on Curve25519.
        };
    private:
        Algorithm _algorithm; ///< The selected asymmetric algorithm.
    protected:
        explicit KeyPairGenerator(Algorithm algorithm):
            _algorithm{algorithm} {}
    public:
        /**
         * @brief Returns a {@code KeyPairGenerator} for the named algorithm.
         * @param algorithm The algorithm name.
         * @return A new {@code KeyPairGenerator} instance.
         * @throws NoSuchAlgorithmException if the algorithm is not recognized.
         *
         * Accepted names: {@code "Ed25519"}, {@code "ED25519"}.
         */
        [[nodiscard]]
        THROWS(NoSuchAlgorithmException)
        static KeyPairGenerator instance(StringView algorithm) {
            if (!is_algorithm(algorithm, Array<StringView, 2>{"Ed25519", "ED25519"})) {
                throw NoSuchAlgorithmException("Unsupported KeyPairGenerator algorithm");
            }
            return KeyPairGenerator(Algorithm::ED25519);
        }

        /**
         * @brief Returns a {@code KeyPairGenerator} for the given algorithm enum value.
         * @param algorithm The algorithm enum value.
         * @return A new {@code KeyPairGenerator} instance.
         */
        [[nodiscard]]
        static KeyPairGenerator instance(Algorithm algorithm) {
            return KeyPairGenerator(algorithm);
        }

        /**
         * @brief Generates a new key pair.
         * @return A {@code KeyPair} containing a fresh {@code PublicKey} and {@code PrivateKey}.
         * @throws KeyGenerationException if the underlying key generation fails.
         */
        [[nodiscard]]
        THROWS(KeyGenerationException)
        KeyPair generate_key_pair() const {
            ensure_sodium_initialized();

            if (_algorithm != Algorithm::ED25519) {
                throw NoSuchAlgorithmException("Unsupported key pair algorithm");
            }

            ByteBuffer pub_bytes(crypto_sign_PUBLICKEYBYTES, 0);
            ByteBuffer priv_bytes(crypto_sign_SECRETKEYBYTES, 0);

            const i32 rc = crypto_sign_keypair(pub_bytes.data(), priv_bytes.data());
            if (rc != 0) {
                throw KeyGenerationException("Failed to generate Ed25519 key pair");
            }

            return KeyPair {
                .public_key  = PublicKey("Ed25519",  Span<const u8>(pub_bytes.data(),  pub_bytes.size())),
                .private_key = PrivateKey("Ed25519", Span<const u8>(priv_bytes.data(), priv_bytes.size())),
            };
        }
    };
}
#endif
