#pragma once

using stdx::io::ByteBuffer;

/**
 * @namespace stdx::crypto
 * @brief Cryptography operations: hashing, signing, and symmetric encryption.
 */
export namespace stdx::crypto {
    /**
     * @interface Key
     * @brief Interface for all cryptographic keys
     *
     * Concrete subtypes are {@code SecretKey}, {@code PublicKey}, and {@code PrivateKey}.
     */
    class Key {
    public:
        virtual ~Key() = default;

        /**
         * @brief Returns the standard algorithm name this key belongs to (e.g. {@code "Ed25519"}, {@code "SecretBox"}).
         * @return The algorithm name.
         */
        [[nodiscard]]
        virtual const String& algorithm() const noexcept = 0;

        /**
         * @brief Returns the primary encoding format of the key material (e.g. {@code "RAW"}).
         * @return The encoding format name.
         */
        [[nodiscard]]
        virtual StringView format() const noexcept = 0;

        /**
         * @brief Returns the raw encoded key material.
         * @return A span over the key bytes.
         */
        [[nodiscard]]
        virtual Span<const u8> encoded() const noexcept = 0;

        /**
         * @brief Returns {@code true} if the key holds no material.
         * @return {@code true} if empty.
         */
        [[nodiscard]]
        virtual bool empty() const noexcept = 0;
    };

    /**
     * @class SecretKey
     * @brief A secret (symmetric) key.
     * @implements Key
     *
     * The algorithm name is stored as a {@code String} so that keys constructed
     * from external sources (config files, serialized data) carry their algorithm
     * identity without requiring compile-time knowledge of the full algorithm set.
     */
    class SecretKey: public Key {
    private:
        String _algorithm_name; ///< Standard algorithm name (e.g. {@code "SecretBox"}).
        ByteBuffer _key_bytes; ///< Raw key material.
    public:
        SecretKey() = default;

        /**
         * @param algorithm The algorithm name this key is for.
         * @param key The raw key bytes to copy.
         */
        SecretKey(StringView algorithm, Span<const u8> key):
            _algorithm_name{algorithm}, _key_bytes(key.begin(), key.end()) {}

        [[nodiscard]]
        const String& algorithm() const noexcept override {
            return _algorithm_name;
        }

        [[nodiscard]]
        StringView format() const noexcept override {
            return "RAW";
        }

        [[nodiscard]]
        Span<const u8> encoded() const noexcept override {
            return Span<const u8>(_key_bytes.data(), _key_bytes.size());
        }

        [[nodiscard]]
        bool empty() const noexcept override {
            return _key_bytes.empty();
        }
    };

    /**
     * @class PublicKey
     * @brief An asymmetric public key.
     * @implements Key
     */
    class PublicKey: public Key {
    private:
        String _algorithm_name; ///< Standard algorithm name (e.g. {@code "Ed25519"}).
        ByteBuffer _key_bytes; ///< Raw key material.
    public:
        PublicKey() = default;

        /**
         * @param algorithm The algorithm name this key is for.
         * @param key The raw key bytes to copy.
         */
        PublicKey(StringView algorithm, Span<const u8> key):
            _algorithm_name{algorithm}, _key_bytes(key.begin(), key.end()) {}

        [[nodiscard]]
        const String& algorithm() const noexcept override {
            return _algorithm_name;
        }

        [[nodiscard]]
        StringView format() const noexcept override {
            return "RAW";
        }

        [[nodiscard]]
        Span<const u8> encoded() const noexcept override {
            return Span<const u8>(_key_bytes.data(), _key_bytes.size());
        }

        [[nodiscard]]
        bool empty() const noexcept override {
            return _key_bytes.empty();
        }
    };

    /**
     * @class PrivateKey
     * @brief An asymmetric private key.
     * @implements Key
     */
    class PrivateKey: public Key {
    private:
        String _algorithm_name; ///< Standard algorithm name (e.g. {@code "Ed25519"}).
        ByteBuffer _key_bytes; ///< Raw key material.
    public:
        PrivateKey() = default;

        /**
         * @param algorithm The algorithm name this key is for.
         * @param key The raw key bytes to copy.
         */
        PrivateKey(StringView algorithm, Span<const u8> key):
            _algorithm_name{algorithm}, _key_bytes(key.begin(), key.end()) {}

        [[nodiscard]]
        const String& algorithm() const noexcept override {
            return _algorithm_name;
        }

        [[nodiscard]]
        StringView format() const noexcept override {
            return "RAW";
        }

        [[nodiscard]]
        Span<const u8> encoded() const noexcept override {
            return Span<const u8>(_key_bytes.data(), _key_bytes.size());
        }

        [[nodiscard]]
        bool empty() const noexcept override {
            return _key_bytes.empty();
        }
    };

    /**
     * @struct KeyPair
     * @brief A matched asymmetric key pair.
     */
    struct KeyPair {
        PublicKey public_key; ///< The public half of the key pair.
        PrivateKey private_key; ///< The private half of the key pair.
    };

    /**
     * @enum MessageDigestAlgorithm
     * @brief Compile-time-safe selector for {@code MessageDigest} algorithms.
     *
     * Use this enum with {@code MessageDigest::instance(MessageDigestAlgorithm)} when
     * the algorithm is known at compile time. Use the string overload when the
     * algorithm name comes from configuration or external data.
     */
    enum class MessageDigestAlgorithm: u8 {
        SHA_256, ///< SHA-2 with a 256-bit digest.
        SHA_512, ///< SHA-2 with a 512-bit digest.
        BLAKE2B_256, ///< BLAKE2b truncated to a 256-bit digest.
    };
}
