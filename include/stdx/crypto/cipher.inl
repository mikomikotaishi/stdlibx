#pragma once

using stdx::io::ByteBuffer;

#ifdef STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY
/**
 * @namespace stdx::crypto
 * @brief Cryptography operations: hashing, signing, and symmetric encryption.
 */
export namespace stdx::crypto {
    /**
     * @enum CipherMode
     * @brief Selects whether a {@code Cipher} encrypts or decrypts.
     */
    enum class CipherMode: u8 {
        ENCRYPT_MODE, ///< Cipher will encrypt plaintext to ciphertext.
        DECRYPT_MODE, ///< Cipher will decrypt ciphertext to plaintext.
    };

    /**
     * @class KeyGenerator
     * @brief Generates secret (symmetric) keys.
     *
     * @code{.cpp}
     * // Enum-based (preferred):
     * KeyGenerator kg = KeyGenerator::instance(KeyGenerator::Algorithm::SECRETBOX);
     *
     * // String-based:
     * KeyGenerator kg = KeyGenerator::instance("SecretBox");
     *
     * SecretKey key = kg.generate_key();
     * @endcode
     */
    class KeyGenerator {
    public:
        /**
         * @enum Algorithm
         * @brief Compile-time-safe selector for {@code KeyGenerator} algorithms.
         */
        enum class Algorithm: u8 {
            SECRETBOX, ///< XSalsa20-Poly1305 (libsodium {@code crypto_secretbox}).
        };
    private:
        Algorithm _algorithm; ///< The selected symmetric algorithm.
        mutable SecureRandom _rng; ///< Entropy source for key material.
    protected:
        explicit KeyGenerator(Algorithm algorithm):
            _algorithm{algorithm} {}
    public:
        /**
         * @brief Returns a {@code KeyGenerator} for the named algorithm.
         * @param algorithm The algorithm name.
         * @return A new {@code KeyGenerator} instance.
         * @throws NoSuchAlgorithmException if the algorithm is not recognized.
         *
         * Accepted names: {@code "SecretBox"}, {@code "SECRETBOX"},
         * {@code "XSalsa20-Poly1305"}, {@code "secretbox"}.
         */
        [[nodiscard]]
        THROWS(NoSuchAlgorithmException)
        static KeyGenerator instance(StringView algorithm) {
            if (!is_algorithm(
                    algorithm,
                    Array<StringView, 4>{"SecretBox", "SECRETBOX", "XSalsa20-Poly1305", "secretbox"}
                )) {
                throw NoSuchAlgorithmException("Unsupported KeyGenerator algorithm");
            }
            return KeyGenerator(Algorithm::SECRETBOX);
        }

        /**
        * @brief Returns a {@code KeyGenerator} for the given algorithm enum value.
        * @param algorithm The algorithm enum value.
        * @return A new {@code KeyGenerator} instance.
        */
        [[nodiscard]]
        static KeyGenerator instance(Algorithm algorithm) {
            return KeyGenerator(algorithm);
        }

        /**
        * @brief Generates a new random secret key suitable for the configured algorithm.
        * @return A new {@code SecretKey}.
        * @throws NoSuchAlgorithmException if the algorithm is not supported.
        */
        [[nodiscard]]
        THROWS(NoSuchAlgorithmException)
        SecretKey generate_key() const {
            if (_algorithm != Algorithm::SECRETBOX) {
                throw NoSuchAlgorithmException("Unsupported key generation algorithm");
            }

            ByteBuffer key = _rng.next_bytes(SECRETBOX_KEY_BYTES);
            return SecretKey("SecretBox", Span<const u8>(key.data(), key.size()));
        }
    };

    /**
     * @class Cipher
     * @brief Symmetric encryption/decryption engine.
     *
     * The transformation string follows the Java convention
     * {@code "Algorithm/Mode/Padding"} (e.g. {@code "SecretBox/None/NoPadding"}),
     * though single-component names such as {@code "SecretBox"} are also accepted.
     *
     * For SecretBox (AEAD), the ciphertext format is:
     * {@code [ nonce (24 B) | MAC (16 B) | ciphertext ]}
     *
     * @code{.cpp}
     * Cipher cipher = Cipher::instance("SecretBox");
     * cipher.init(CipherMode::ENCRYPT_MODE, secretKey);
     * ByteBuffer ciphertext = cipher.do_final(plaintext);
     *
     * cipher.init(CipherMode::DECRYPT_MODE, secretKey);
     * ByteBuffer plaintext = cipher.do_final(ciphertext);
     * @endcode
     */
    class Cipher {
    private:
        String _transformation; ///< The transformation string used to construct this cipher.
        CipherMode _mode; ///< Current operating mode (set by init()).
        SecretKey _key; ///< The key to encrypt/decrypt with (set by init()).
        bool _initialized; ///< Whether init() has been called successfully.
        mutable SecureRandom _rng; ///< Entropy source for nonce generation.
    protected:
        explicit Cipher(StringView transformation):
            _transformation{transformation}, _mode{CipherMode::ENCRYPT_MODE}, _key{}, _initialized{false} {}
    public:
        /**
         * @brief Returns a {@code Cipher} for the named transformation.
         * @param transformation The transformation name.
         * @return A new, uninitialized {@code Cipher} instance.
         * @throws NoSuchAlgorithmException if the transformation is not recognized.
         *
         * Accepted names: {@code "SecretBox"}, {@code "XSalsa20-Poly1305"},
         * {@code "SecretBox/None/NoPadding"}, {@code "secretbox"}.
         */
        [[nodiscard]]
        THROWS(NoSuchAlgorithmException)
        static Cipher instance(StringView transformation) {
            if (!is_algorithm(
                    transformation,
                    Array<StringView, 4>{"SecretBox", "XSalsa20-Poly1305", "SecretBox/None/NoPadding", "secretbox"}
                )) {
                throw NoSuchAlgorithmException("Unsupported cipher transformation");
            }
            return Cipher(transformation);
        }

        /**
         * @brief Initializes the cipher for encryption or decryption with the given key.
         * @param mode The operating mode.
         * @param key The secret key to use; must be non-empty and the correct size.
         * @throws InvalidKeyException if the key is empty or the wrong size.
         */
        THROWS(InvalidKeyException)
        void init(CipherMode mode, const SecretKey& key) {
            if (key.empty()) {
                throw InvalidKeyException("Cipher key cannot be empty");
            }
            if (key.encoded().size() != SECRETBOX_KEY_BYTES) {
                throw InvalidKeyException("Invalid SecretBox key size");
            }

            this->_mode = mode;
            this->_key = key;
            _initialized = true;
        }

        /**
         * @brief Encrypts or decrypts {@code input} in a single step and returns the result.
         * @param input The plaintext (encrypt) or nonce || MAC || ciphertext (decrypt).
         * @return The ciphertext (encrypt) or plaintext (decrypt).
         * @throws AEADBadTagException if the authentication tag is invalid (decrypt only).
         * @throws InvalidArgumentException if the ciphertext is too short (decrypt only).
         * @throws IllegalStateException if {@code init()} has not been called.
         *
         * In {@code ENCRYPT_MODE}, prepends a random nonce; in {@code DECRYPT_MODE},
         * strips the nonce and verifies the authentication tag before returning plaintext.
         */
        [[nodiscard]]
        THROWS(AEADBadTagException, InvalidArgumentException, IllegalStateException)
        ByteBuffer do_final(Span<const u8> input) const {
            ensure_sodium_initialized();

            if (!_initialized) {
                throw IllegalStateException("Cipher is not initialized");
            }

            const Span<const u8> key_material = _key.encoded();

            if (_mode == CipherMode::ENCRYPT_MODE) {
                ByteBuffer nonce(SECRETBOX_NONCE_BYTES, 0);
                _rng.next_bytes(Span<u8>(nonce.data(), nonce.size()));

                ByteBuffer output(SECRETBOX_NONCE_BYTES + SECRETBOX_MAC_BYTES + input.size(), 0);

                stdx::ranges::copy(nonce, output.begin());

                u8* cipher_out = output.data() + static_cast<ptrdiff>(SECRETBOX_NONCE_BYTES);
                const i32 rc = crypto_secretbox_easy(
                    cipher_out,
                    input.data(),
                    static_cast<u64>(input.size()),
                    nonce.data(),
                    key_material.data()
                );
                if (rc != 0) {
                    throw AEADBadTagException("SecretBox authentication failed");
                }
                return output;
            }

            if (input.size() < SECRETBOX_NONCE_BYTES + SECRETBOX_MAC_BYTES) {
                throw InvalidArgumentException("Ciphertext is too short for SecretBox payload");
            }

            const u8* nonce = input.data();
            const u8* cipher_text = input.data() + static_cast<ptrdiff>(SECRETBOX_NONCE_BYTES);
            const usize cipher_text_size = input.size() - SECRETBOX_NONCE_BYTES;

            ByteBuffer plain(cipher_text_size - SECRETBOX_MAC_BYTES, 0);
            const i32 rc = crypto_secretbox_open_easy(
                plain.data(),
                cipher_text,
                static_cast<u64>(cipher_text_size),
                nonce,
                key_material.data()
            );
            if (rc != 0) {
                throw AEADBadTagException("Cipher authentication failed");
            }
            return plain;
        }
    };
}
#endif
