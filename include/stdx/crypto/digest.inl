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
     * @class MessageDigest
     * @brief Stateful hash engine.
     *
     * Data is fed incrementally via {@code update()}, and the final digest is
     * produced by {@code digest()}, which also resets the engine for reuse.
     *
     * @code{.cpp}
     * MessageDigest md = MessageDigest::instance("SHA-256");
     * md.update(chunk1);
     * md.update(chunk2);
     * ByteBuffer hash = md.digest(); // resets internal state
     * @endcode
     */
    class MessageDigest {
    private:
        MessageDigestAlgorithm _algorithm; ///< The selected hash algorithm.
        Vector<u8> _buffer; ///< Accumulates data fed via update().

        /**
         * @brief Computes the hash of {@code data} without touching {@code buffer}.
         * @param data The input bytes.
         * @return The raw hash output.
         * @throws HashFailedException if the underlying hash function reports an error.
         */
        [[nodiscard]]
        THROWS(HashFailedException)
        ByteBuffer compute(Span<const u8> data) const {
            ensure_sodium_initialized();

            switch (_algorithm) {
                case MessageDigestAlgorithm::SHA_256: {
                    ByteBuffer out(crypto_hash_sha256_BYTES, 0);
                    const i32 rc = crypto_hash_sha256(out.data(), data.data(), static_cast<u64>(data.size()));
                    if (rc != 0) {
                        throw HashFailedException("Failed to compute SHA-256 hash!");
                    }
                    return out;
                }
                case MessageDigestAlgorithm::SHA_512: {
                    ByteBuffer out(crypto_hash_sha512_BYTES, 0);
                    const i32 rc = crypto_hash_sha512(out.data(), data.data(), static_cast<u64>(data.size()));
                    if (rc != 0) {
                        throw HashFailedException("Failed to compute SHA-512 hash!");
                    }
                    return out;
                }
                case MessageDigestAlgorithm::BLAKE2B_256: {
                    ByteBuffer out(32, 0);
                    const i32 rc = crypto_generichash(
                        out.data(),
                        out.size(),
                        data.data(),
                        static_cast<u64>(data.size()),
                        nullptr,
                        0
                    );
                    if (rc != 0) {
                        throw HashFailedException("Failed to compute BLAKE2B-256 hash!");
                    }
                    return out;
                }
            }
            Ops::unreachable();
        }
    protected:
        explicit MessageDigest(MessageDigestAlgorithm algorithm = MessageDigestAlgorithm::SHA_256):
            _algorithm{algorithm} {}
    public:
        /**
         * @brief Returns a {@code MessageDigest} for the named algorithm.
         * @param algorithm The algorithm name.
         * @return A new {@code MessageDigest} instance.
         * @throws NoSuchAlgorithmException if the algorithm is not recognized.
         *
         * Accepted names: {@code "SHA-256"}, {@code "SHA256"}, {@code "SHA-512"},
         * {@code "SHA512"}, {@code "BLAKE2b-256"}, {@code "BLAKE2B-256"}, {@code "BLAKE2b256"}.
         */
        [[nodiscard]]
        THROWS(NoSuchAlgorithmException)
        static MessageDigest instance(StringView algorithm) {
            if (is_algorithm(algorithm, Array<StringView, 2>{"SHA-256", "SHA256"})) {
                return MessageDigest(MessageDigestAlgorithm::SHA_256);
            }
            if (is_algorithm(algorithm, Array<StringView, 2>{"SHA-512", "SHA512"})) {
                return MessageDigest(MessageDigestAlgorithm::SHA_512);
            }
            if (is_algorithm(algorithm, Array<StringView, 3>{"BLAKE2b-256", "BLAKE2B-256", "BLAKE2b256"})) {
                return MessageDigest(MessageDigestAlgorithm::BLAKE2B_256);
            }
            throw NoSuchAlgorithmException("Unsupported MessageDigest algorithm");
        }

        /**
         * @brief Returns a {@code MessageDigest} for the given algorithm enum value.
         * @param algorithm The algorithm enum value.
         * @return A new {@code MessageDigest} instance.
         *
         * Prefer this overload when the algorithm is known at compile time.
         */
        [[nodiscard]]
        static MessageDigest instance(MessageDigestAlgorithm algorithm) {
            return MessageDigest(algorithm);
        }

        /**
         * @brief Feeds {@code data} into the running digest without finalizing.
         * @param data The bytes to add.
         */
        void update(Span<const u8> data) {
            _buffer.insert(_buffer.end(), data.begin(), data.end());
        }

        /**
         * @brief Finlalizes the digest of all updated data and resets internal state.
         * @return The raw hash bytes.
         * @throws HashFailedException if the hash computation fails.
         */
        [[nodiscard]]
        THROWS(HashFailedException)
        ByteBuffer digest() {
            ByteBuffer result = compute(Span<const u8>(_buffer.data(), _buffer.size()));
            _buffer.clear();
            return result;
        }

        /**
         * @brief Feeds {@code data} then finalizes, equivalent to {@code update(data); digest()}.
         * @param data The bytes to hash.
         * @return The raw hash bytes.
         * @throws HashFailedException if the hash computation fails.
         */
        [[nodiscard]]
        THROWS(HashFailedException)
        ByteBuffer digest(Span<const u8> data) {
            update(data);
            return digest();
        }

        /**
         * @brief One-shot static convenience that hashes {@code data} without constructing an instance.
         * @param algorithm The algorithm to use.
         * @param data The bytes to hash.
         * @return The raw hash bytes.
         * @throws HashFailedException if the hash computation fails.
         */
        [[nodiscard]]
        THROWS(HashFailedException)
        static ByteBuffer digest(MessageDigestAlgorithm algorithm, Span<const u8> data) {
            return MessageDigest(algorithm).digest(data);
        }
    };
}
#endif
