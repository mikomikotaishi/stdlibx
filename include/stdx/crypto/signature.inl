#pragma once

using stdx::collections::Vector;
using stdx::io::ByteBuffer;

#ifdef STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY
/**
 * @namespace stdx::crypto
 * @brief Cryptography operations: hashing, signing, and symmetric encryption.
 */
export namespace stdx::crypto {
    /**
     * @class Signature
     * @brief Stateful signing and verification engine.
     *
     * Data is fed incrementally via {@code update()}, then finalized by {@code sign()}
     * or {@code verify()}, both of which reset internal state afterwards.
     *
     * @code{.cpp}
     * Signature sig = Signature::instance("Ed25519");
     *
     * sig.init_sign(keyPair.private_key);
     * sig.update(message);
     * ByteBuffer signature = sig.sign();
     *
     * sig.init_verify(keyPair.public_key);
     * sig.update(message);
     * bool ok = sig.verify(signature);
     * @endcode
     */
    class Signature {
    public:
        /**
         * @enum Algorithm
         * @brief Compile-time-safe selector for {@code Signature} algorithms.
         */
        enum class Algorithm: u8 {
            ED25519, ///< Edwards-curve Digital Signature Algorithm on Curve25519.
        };
    private:
        Algorithm _algorithm; ///< The selected signature algorithm.
        PrivateKey _signing_key; ///< Populated by init_sign(); empty otherwise.
        PublicKey _verification_key; ///< Populated by init_verify(); empty otherwise.
        Vector<u8> _message_buffer; ///< Accumulates message data fed via update().
    protected:
        explicit Signature(Algorithm algorithm):
            _algorithm{algorithm} {}
    public:
        /**
         * @brief Returns a {@code Signature} for the named algorithm.
         * @param algorithm The algorithm name.
         * @return A new {@code Signature} instance.
         * @throws NoSuchAlgorithmException if the algorithm is not recognized.
         *
         * Accepted names: {@code "Ed25519"}, {@code "ED25519"}.
         */
        [[nodiscard]]
        THROWS(NoSuchAlgorithmException)
        static Signature instance(StringView algorithm) {
            if (!is_algorithm(algorithm, Array<StringView, 2>{"Ed25519", "ED25519"})) {
                throw NoSuchAlgorithmException("Unsupported Signature algorithm");
            }
            return Signature(Algorithm::ED25519);
        }

        /**
         * @brief Returns a {@code Signature} for the given algorithm enum value.
         * @param algorithm The algorithm enum value.
         * @return A new {@code Signature} instance.
         */
        [[nodiscard]]
        static Signature instance(Algorithm algorithm) {
            return Signature(algorithm);
        }

        /**
         * @brief Initializes this engine for signing with the given private key.
         * @param key The private key to sign with.
         * @throws InvalidKeyException if the key length does not match the algorithm.
         *
         * Clears any previously accumulated message data.
         */
        THROWS(InvalidKeyException)
        void init_sign(const PrivateKey& key) {
            if (key.encoded().size() != ED25519_PRIVATE_KEY_BYTES) {
                throw InvalidKeyException("Invalid Ed25519 private key length");
            }
            _signing_key = key;
            _verification_key = {};
            _message_buffer.clear();
        }

        /**
         * @brief Initializes this engine for verification with the given public key.
         * @param key The public key to verify against.
         * @throws InvalidKeyException if the key length does not match the algorithm.
         *
         * Clears any previously accumulated message data.
         */
        THROWS(InvalidKeyException)
        void init_verify(const PublicKey& key) {
            if (key.encoded().size() != ED25519_PUBLIC_KEY_BYTES) {
                throw InvalidKeyException("Invalid Ed25519 public key length");
            }
            _verification_key = key;
            _signing_key = {};
            _message_buffer.clear();
        }

        /**
         * @brief Feeds message bytes into the engine without finalizing.
         * @param data The message bytes to add.
         *
         * May be called multiple times before {@code sign()} or {@code verify()}.
         */
        void update(Span<const u8> data) {
            _message_buffer.insert(_message_buffer.end(), data.begin(), data.end());
        }

        /**
         * @brief Signs all updated data and resets internal state.
         * @return The detached signature bytes.
         * @throws NoSuchAlgorithmException if the algorithm is not supported.
         * @throws InvalidKeyException if {@code init_sign()} was not called.
         * @throws SignatureException if the underlying signing operation fails.
         */
        [[nodiscard]]
        THROWS(NoSuchAlgorithmException, InvalidKeyException, SignatureException)
        ByteBuffer sign() {
            ensure_sodium_initialized();

            if (_algorithm != Algorithm::ED25519) {
                throw NoSuchAlgorithmException("Unsupported signature algorithm");
            }
            if (_signing_key.empty()) {
                throw InvalidKeyException("Signature object is not initialized for signing");
            }

            ByteBuffer signature(crypto_sign_BYTES, 0);
            unsigned long long signature_length = 0; // must be unsigned long long (cannot be u64) due to libsodium API
            const i32 rc = crypto_sign_detached(
                signature.data(),
                &signature_length,
                _message_buffer.data(),
                static_cast<u64>(_message_buffer.size()),
                _signing_key.encoded().data()
            );
            _message_buffer.clear();
            if (rc != 0) {
                throw SignatureException("Failed to generate signature");
            }
            signature.resize(static_cast<usize>(signature_length));
            return signature;
        }

        /**
         * @brief Verifies {@code signature} against all updated data and resets internal state.
         * @param signature The detached signature to check.
         * @return {@code true} if the signature is valid, {@code false} otherwise.
         * @throws NoSuchAlgorithmException if the algorithm is not supported.
         * @throws InvalidKeyException if {@code init_verify()} was not called, or the signature length is wrong.
         */
        [[nodiscard]]
        THROWS(NoSuchAlgorithmException, InvalidKeyException)
        bool verify(Span<const u8> signature) {
            ensure_sodium_initialized();

            if (_algorithm != Algorithm::ED25519) {
                throw NoSuchAlgorithmException("Unsupported signature algorithm");
            }
            if (_verification_key.empty()) {
                throw InvalidKeyException("Signature object is not initialized for verification");
            }
            if (signature.size() != ED25519_SIGNATURE_BYTES) {
                throw InvalidKeyException("Invalid Ed25519 signature length");
            }

            const bool ok = crypto_sign_verify_detached(
                signature.data(),
                _message_buffer.data(),
                static_cast<u64>(_message_buffer.size()),
                _verification_key.encoded().data()
            ) == 0;
            _message_buffer.clear();
            return ok;
        }
    };
}
#endif
