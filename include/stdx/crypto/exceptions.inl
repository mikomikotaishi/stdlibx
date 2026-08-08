#pragma once

/**
 * @namespace stdx::crypto
 * @brief Cryptography operations: hashing, signing, and symmetric encryption.
 */
export namespace stdx::crypto {
    /**
     * @class CryptoException
     * @brief Base exception for all cryptography errors.
     * @extends Exception
     */
    class CryptoException: public Exception {
    private:
        String _msg; ///< Human-readable error message.
    public:
        explicit CryptoException(const String& msg = ""):
            Exception(), _msg{msg} {}

        [[nodiscard]]
        const char* what() const noexcept override {
            return _msg.c_str();
        }
    };

    /**
     * @class CryptoInitializationException
     * @brief Thrown when the underlying cryptography library fails to initialize.
     * @extends CryptoException
     */
    class CryptoInitializationException: public CryptoException {
    public:
        using CryptoException::CryptoException;
    };

    /**
     * @class SignatureException
     * @brief Thrown when a signing or verification operation fails.
     * @extends CryptoException
     */
    class SignatureException: public CryptoException {
    public:
        using CryptoException::CryptoException;
    };

    /**
     * @class NoSuchAlgorithmException
     * @brief Thrown when a requested algorithm is not supported.
     * @extends CryptoException
     */
    class NoSuchAlgorithmException: public CryptoException {
    public:
        using CryptoException::CryptoException;
    };

    /**
     * @class KeyException
     * @brief Base exception for key-related errors.
     * @extends CryptoException
     */
    class KeyException: public CryptoException {
    public:
        using CryptoException::CryptoException;
    };

    /**
     * @class InvalidKeyException
     * @brief Thrown when a key is structurally invalid (wrong length, wrong type, etc.).
     * @extends KeyException
     */
    class InvalidKeyException: public KeyException {
    public:
        using KeyException::KeyException;
    };

    /**
     * @class KeyGenerationException
     * @brief Thrown when key generation fails.
     * @extends KeyException
     */
    class KeyGenerationException: public KeyException {
    public:
        using KeyException::KeyException;
    };

    /**
     * @class HashFailedException
     * @brief Thrown when a hash computation fails.
     * @extends CryptoException
     */
    class HashFailedException: public CryptoException {
    public:
        using CryptoException::CryptoException;
    };

    /**
     * @class BadPaddingException
     * @brief Thrown when padding is missing or malformed during decryption.
     * @extends CryptoException
     */
    class BadPaddingException: public CryptoException {
    public:
        using CryptoException::CryptoException;
    };

    /**
     * @class AEADBadTagException
     * @brief Thrown when AEAD authentication tag verification fails.
     * @extends CryptoException
     */
    class AEADBadTagException: public CryptoException {
    public:
        using CryptoException::CryptoException;
    };
}
