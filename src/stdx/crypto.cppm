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

using stdx::collections::Vector;
using stdx::io::ByteBuffer;
using stdx::sync::OnceFlag;

using namespace stdx::core;

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
class CryptoException : public Exception {
private:
	String msg; ///< Human-readable error message.
public:
	explicit CryptoException(const String& msg = ""):
		Exception(), msg{msg} {}

	[[nodiscard]]
	const char* what() const noexcept override {
		return msg.c_str();
	}
};

/**
 * @class CryptoInitializationException
 * @brief Thrown when the underlying cryptography library fails to initialise.
 * @extends CryptoException
 */
class CryptoInitializationException: public CryptoException {
public:
	explicit CryptoInitializationException(const String& msg = ""):
		CryptoException(msg) {}
};

/**
 * @class SignatureException
 * @brief Thrown when a signing or verification operation fails.
 * @extends CryptoException
 */
class SignatureException: public CryptoException {
public:
	explicit SignatureException(const String& msg = ""):
		CryptoException(msg) {}
};

/**
 * @class NoSuchAlgorithmException
 * @brief Thrown when a requested algorithm is not supported.
 * @extends CryptoException
 */
class NoSuchAlgorithmException: public CryptoException {
public:
	explicit NoSuchAlgorithmException(const String& msg = ""):
		CryptoException(msg) {}
};

/**
 * @class KeyException
 * @brief Base exception for key-related errors.
 * @extends CryptoException
 */
class KeyException: public CryptoException {
public:
	explicit KeyException(const String& msg = ""):
		CryptoException(msg) {}
};

/**
 * @class InvalidKeyException
 * @brief Thrown when a key is structurally invalid (wrong length, wrong type, etc.).
 * @extends KeyException
 */
class InvalidKeyException: public KeyException {
public:
	explicit InvalidKeyException(const String& msg = ""):
		KeyException(msg) {}
};

/**
 * @class KeyGenerationException
 * @brief Thrown when key generation fails.
 * @extends KeyException
 */
class KeyGenerationException: public KeyException {
public:
	explicit KeyGenerationException(const String& msg = ""):
		KeyException(msg) {}
};

/**
 * @class HashFailedException
 * @brief Thrown when a hash computation fails.
 * @extends CryptoException
 */
class HashFailedException: public CryptoException {
public:
	explicit HashFailedException(const String& msg = ""):
		CryptoException(msg) {}
};

/**
 * @class BadPaddingException
 * @brief Thrown when padding is missing or malformed during decryption.
 * @extends CryptoException
 */
class BadPaddingException: public CryptoException {
public:
	explicit BadPaddingException(const String& msg = ""):
		CryptoException(msg) {}
};

/**
 * @class AEADBadTagException
 * @brief Thrown when AEAD authentication tag verification fails.
 * @extends CryptoException
 */
class AEADBadTagException: public CryptoException {
public:
	explicit AEADBadTagException(const String& msg = ""):
		CryptoException(msg) {}
};

}

namespace stdx::crypto {
	inline constexpr usize ED25519_PUBLIC_KEY_BYTES = 32; ///< Ed25519 public key size in bytes.
	inline constexpr usize ED25519_PRIVATE_KEY_BYTES = 64; ///< Ed25519 private key size in bytes (seed + public key).
	inline constexpr usize ED25519_SIGNATURE_BYTES = 64; ///< Ed25519 detached signature size in bytes.

	inline constexpr usize SECRETBOX_KEY_BYTES = 32; ///< XSalsa20-Poly1305 key size in bytes.
	inline constexpr usize SECRETBOX_NONCE_BYTES = 24; ///< XSalsa20-Poly1305 nonce size in bytes.
	inline constexpr usize SECRETBOX_MAC_BYTES = 16; ///< XSalsa20-Poly1305 authentication tag size in bytes.

	/**
	 * @brief Ensures libsodium is initialised exactly once across all threads.
	 * @throws CryptoInitializationException if {@code sodium_init()} returns a negative value.
	 */
	void ensure_sodium_initialized() throws (CryptoInitializationException) {
		static OnceFlag sodium_init_flag;
		stdx::sync::call_once(sodium_init_flag, []() -> void {
			if (sodium_init() < 0) {
				throw CryptoInitializationException("Failed to initialise libsodium");
			}
		});
	}

	/**
	 * @brief Returns {@code true} if {@code actual} matches any of {@code candidates}.
	 *
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

	#if defined(STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY)
	static_assert(crypto_sign_PUBLICKEYBYTES == ED25519_PUBLIC_KEY_BYTES);
	static_assert(crypto_sign_SECRETKEYBYTES == ED25519_PRIVATE_KEY_BYTES);
	static_assert(crypto_sign_BYTES == ED25519_SIGNATURE_BYTES);

	static_assert(crypto_secretbox_KEYBYTES == SECRETBOX_KEY_BYTES);
	static_assert(crypto_secretbox_NONCEBYTES == SECRETBOX_NONCE_BYTES);
	static_assert(crypto_secretbox_MACBYTES == SECRETBOX_MAC_BYTES);
	#endif
}

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
	String algorithm_name; ///< Standard algorithm name (e.g. {@code "SecretBox"}).
	ByteBuffer key_bytes; ///< Raw key material.
public:
	SecretKey() = default;

	/**
	 * @param algorithm The algorithm name this key is for.
	 * @param key The raw key bytes to copy.
	 */
	SecretKey(StringView algorithm, Span<const u8> key):
		algorithm_name{algorithm}, key_bytes(key.begin(), key.end()) {}

	[[nodiscard]]
	const String& algorithm() const noexcept override {
		return algorithm_name;
	}

	[[nodiscard]]
	StringView format() const noexcept override {
		return "RAW";
	}

	[[nodiscard]]
	Span<const u8> encoded() const noexcept override {
		return Span<const u8>(key_bytes.data(), key_bytes.size());
	}

	[[nodiscard]]
	bool empty() const noexcept override {
		return key_bytes.empty();
	}
};

/**
 * @class PublicKey
 * @brief An asymmetric public key.
 * @implements Key
 */
class PublicKey: public Key {
private:
	String algorithm_name; ///< Standard algorithm name (e.g. {@code "Ed25519"}).
	ByteBuffer key_bytes; ///< Raw key material.
public:
	PublicKey() = default;

	/**
	 * @param algorithm The algorithm name this key is for.
	 * @param key The raw key bytes to copy.
	 */
	PublicKey(StringView algorithm, Span<const u8> key):
		algorithm_name{algorithm}, key_bytes(key.begin(), key.end()) {}

	[[nodiscard]]
	const String& algorithm() const noexcept override {
		return algorithm_name;
	}

	[[nodiscard]]
	StringView format() const noexcept override {
		return "RAW";
	}

	[[nodiscard]]
	Span<const u8> encoded() const noexcept override {
		return Span<const u8>(key_bytes.data(), key_bytes.size());
	}

	[[nodiscard]]
	bool empty() const noexcept override {
		return key_bytes.empty();
	}
};

/**
 * @class PrivateKey
 * @brief An asymmetric private key.
 * @implements Key
 */
class PrivateKey: public Key {
private:
	String algorithm_name; ///< Standard algorithm name (e.g. {@code "Ed25519"}).
	ByteBuffer key_bytes; ///< Raw key material.
public:
	PrivateKey() = default;

	/**
	 * @param algorithm The algorithm name this key is for.
	 * @param key The raw key bytes to copy.
	 */
	PrivateKey(StringView algorithm, Span<const u8> key):
		algorithm_name{algorithm}, key_bytes(key.begin(), key.end()) {}

	[[nodiscard]]
	const String& algorithm() const noexcept override {
		return algorithm_name;
	}

	[[nodiscard]]
	StringView format() const noexcept override {
		return "RAW";
	}

	[[nodiscard]]
	Span<const u8> encoded() const noexcept override {
		return Span<const u8>(key_bytes.data(), key_bytes.size());
	}

	[[nodiscard]]
	bool empty() const noexcept override {
		return key_bytes.empty();
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

/**
 * @class SecureRandom
 * @brief Cryptographically secure random number and byte generator.
 *
 * All output is produced by libsodium's {@code randombytes_*} family, which
 * sources entropy from the operating system's secure random facility.
 */
class SecureRandom {
public:
	SecureRandom() = default;

	/**
	 * @brief Returns a cryptographically secure random value in {@code [min, max)}.
	 *
	 * For integer types up to 32 bits, {@code randombytes_uniform()} is used,
	 * which internally applies rejection sampling to eliminate modulo bias.
	 * For wider integer types and floating-point types, {@code randombytes_buf()}
	 * is used directly with appropriate scaling.
	 *
	 * @param min Inclusive lower bound.
	 * @param max Exclusive upper bound.
	 * @return A random value in {@code [min, max)}.
	 * @throws InvalidArgumentException if {@code min >= max}.
	 */
	template <Numeric T>
		requires (!SameAs<T, bool>)
	[[nodiscard]]
	T next(T min, T max) throws (InvalidArgumentException) {
		ensure_sodium_initialized();
		if (min >= max) {
			throw InvalidArgumentException("min must be less than max");
		}
		if constexpr (Integral<T>) {
			if constexpr (sizeof(T) <= 4) {
				// randombytes_uniform applies rejection sampling internally
				const u32 range = static_cast<u32>(max - min);
				return static_cast<T>(randombytes_uniform(range)) + min;
			} else {
				// 64-bit rejection sampling.
				//
				// Threshold = 2^64 mod range, computed as (-range) % range.
				// Values in [threshold, 2^64) are accepted; their count is
				// floor(2^64/range)*range, which is exactly divisible by range,
				// guaranteeing an unbiased result.
				//
				// Crucially, (-range) % range == 0 when range is a power of 2
				// or range == 1, so no samples are ever rejected in those cases.
				// The old formula (UINT64_MAX - UINT64_MAX%range)+1 overflows to
				// 0 for range==1, causing an infinite loop.
				const u64 range = static_cast<u64>(max) - static_cast<u64>(min);
				const u64 threshold = (-range) % range;
				u64 raw;
				do {
					randombytes_buf(&raw, sizeof(raw));
				} while (raw < threshold);
				// Reconstruct the signed result via unsigned offset arithmetic,
				// which handles negative min and cross-zero ranges correctly.
				return static_cast<T>(static_cast<u64>(min) + (raw % range));
			}
		} else if constexpr (FloatingPoint<T>) {
			u64 bits;
			randombytes_buf(&bits, sizeof(bits));
			// Convert 53 random bits to a value in [0, 1)
			const f64 unit = static_cast<f64>(bits >> 11) * (1.0 / static_cast<f64>(1ULL << 53));
			return static_cast<T>(unit) * (max - min) + min;
		}
	}

	/**
	 * @brief Returns a cryptographically secure random integer in {@code [0, max)}.
	 * @param max Exclusive upper bound.
	 * @throws InvalidArgumentException if {@code max <= 0}.
	 */
	template <Integral Int>
		requires (!SameAs<Int, bool>)
	[[nodiscard]]
	Int next(Int max = NumericLimits<Int>::max()) throws (InvalidArgumentException) {
		return next<Int>(Int{0}, max);
	}

	/**
	 * @brief Returns a cryptographically secure random floating-point value in {@code [0, max)}.
	 * @param max Exclusive upper bound.
	 * @throws InvalidArgumentException if {@code max <= 0}.
	 */
	template <FloatingPoint Flt>
	[[nodiscard]]
	Flt next(Flt max = NumericLimits<Flt>::max()) throws (InvalidArgumentException) {
		return next<Flt>(Flt{0}, max);
	}

	/**
	 * @brief Returns a cryptographically secure random value in {@code [0, 1)}.
	 *
	 * Generates 53 bits of OS entropy and scales to the double-precision unit interval,
	 * matching the bit-width used by {@code Random::next_unit<f64>()}.
	 *
	 * Note: {@code sodium_init()} is not called here because {@code randombytes_buf}
	 * initialises the OS entropy source itself, and this method is {@code noexcept}.
	 */
	template <FloatingPoint Flt = f64>
	[[nodiscard]]
	Flt next_unit() noexcept {
		u64 bits;
		randombytes_buf(&bits, sizeof(bits));
		return static_cast<Flt>(static_cast<f64>(bits >> 11) * (1.0 / static_cast<f64>(1ULL << 53)));
	}

	/**
	 * @brief Returns {@code true} with cryptographically secure probability {@code probability}.
	 * @param probability Clamped to {@code [0, 1]}.
	 */
	[[nodiscard]]
	bool chance(f64 probability = 0.5) noexcept {
		// Must static_cast 0.0 and 1.0 to f64 due to GCC treating _Float64 and double differently
		return next_unit() < Math::clamp(probability, static_cast<f64>(0.0), static_cast<f64>(1.0));
	}

	/**
	 * @brief Fills {@code out} with cryptographically secure random bytes in-place.
	 * @param out The buffer to fill.
	 */
	void next_bytes(Span<u8> out) {
		ensure_sodium_initialized();
		if (!out.empty()) {
			randombytes_buf(out.data(), out.size());
		}
	}

	/**
	 * @brief Returns a new buffer of {@code size} cryptographically secure random bytes.
	 * @param size The number of random bytes to generate.
	 * @return A {@code ByteBuffer} of length {@code size} filled with random data.
	 */
	[[nodiscard]]
	ByteBuffer next_bytes(usize size) {
		ByteBuffer out(size, 0);
		next_bytes(Span<u8>(out.data(), out.size()));
		return out;
	}
};

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
	MessageDigestAlgorithm algorithm; ///< The selected hash algorithm.
	Vector<u8> buffer; ///< Accumulates data fed via update().

	/**
	 * @brief Computes the hash of {@code data} without touching {@code buffer}.
	 * @param data The input bytes.
	 * @return The raw hash output.
	 * @throws HashFailedException if the underlying hash function reports an error.
	 */
	[[nodiscard]]
	ByteBuffer compute(Span<const u8> data) const throws (HashFailedException) {
		ensure_sodium_initialized();

		switch (algorithm) {
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
			default:
				throw NoSuchAlgorithmException("Unsupported message digest algorithm");
		}
	}

protected:
	explicit MessageDigest(MessageDigestAlgorithm algorithm = MessageDigestAlgorithm::SHA_256):
		algorithm{algorithm} {}

public:
	/**
	 * @brief Returns a {@code MessageDigest} for the named algorithm.
	 *
	 * Accepted names: {@code "SHA-256"}, {@code "SHA256"}, {@code "SHA-512"},
	 * {@code "SHA512"}, {@code "BLAKE2b-256"}, {@code "BLAKE2B-256"}, {@code "BLAKE2b256"}.
	 *
	 * @param algorithm The algorithm name.
	 * @return A new {@code MessageDigest} instance.
	 * @throws NoSuchAlgorithmException if the algorithm is not recognised.
	 */
	[[nodiscard]]
	static MessageDigest instance(StringView algorithm) throws (NoSuchAlgorithmException) {
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
	 *
	 * Prefer this overload when the algorithm is known at compile time.
	 *
	 * @param algorithm The algorithm enum value.
	 * @return A new {@code MessageDigest} instance.
	 */
	[[nodiscard]]
	static MessageDigest instance(MessageDigestAlgorithm algorithm) {
		return MessageDigest(algorithm);
	}

	/**
	 * @brief Feeds {@code data} into the running digest without finalising.
	 * @param data The bytes to add.
	 */
	void update(Span<const u8> data) {
		buffer.insert(buffer.end(), data.begin(), data.end());
	}

	/**
	 * @brief Finalises the digest of all updated data and resets internal state.
	 * @return The raw hash bytes.
	 * @throws HashFailedException if the hash computation fails.
	 */
	[[nodiscard]]
	ByteBuffer digest() throws (HashFailedException) {
		ByteBuffer result = compute(Span<const u8>(buffer.data(), buffer.size()));
		buffer.clear();
		return result;
	}

	/**
	 * @brief Feeds {@code data} then finalises, equivalent to {@code update(data); digest()}.
	 * @param data The bytes to hash.
	 * @return The raw hash bytes.
	 * @throws HashFailedException if the hash computation fails.
	 */
	[[nodiscard]]
	ByteBuffer digest(Span<const u8> data) throws (HashFailedException) {
		update(data);
		return digest();
	}

	/**
	 * @brief One-shot static convenience that hashes {@code data} without constructing an instance.
	 * @param algorithm The algorithm to use.
	 * @param data      The bytes to hash.
	 * @return The raw hash bytes.
	 * @throws HashFailedException if the hash computation fails.
	 */
	[[nodiscard]]
	static ByteBuffer digest(MessageDigestAlgorithm algorithm, Span<const u8> data) throws (HashFailedException) {
		return MessageDigest(algorithm).digest(data);
	}
};

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
	Algorithm algorithm; ///< The selected asymmetric algorithm.

protected:
	explicit KeyPairGenerator(Algorithm algorithm):
		algorithm{algorithm} {}

public:
	/**
	 * @brief Returns a {@code KeyPairGenerator} for the named algorithm.
	 *
	 * Accepted names: {@code "Ed25519"}, {@code "ED25519"}.
	 *
	 * @param algorithm The algorithm name.
	 * @return A new {@code KeyPairGenerator} instance.
	 * @throws NoSuchAlgorithmException if the algorithm is not recognised.
	 */
	[[nodiscard]]
	static KeyPairGenerator instance(StringView algorithm) throws (NoSuchAlgorithmException) {
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
	KeyPair generate_key_pair() const throws (KeyGenerationException) {
		ensure_sodium_initialized();

		if (algorithm != Algorithm::ED25519) {
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

/**
 * @class Signature
 * @brief Stateful signing and verification engine.
 *
 * Data is fed incrementally via {@code update()}, then finalised by {@code sign()}
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
	Algorithm algorithm; ///< The selected signature algorithm.
	PrivateKey signing_key; ///< Populated by init_sign(); empty otherwise.
	PublicKey verification_key; ///< Populated by init_verify(); empty otherwise.
	Vector<u8> message_buffer; ///< Accumulates message data fed via update().

protected:
	explicit Signature(Algorithm algorithm):
		algorithm{algorithm} {}

public:
	/**
	 * @brief Returns a {@code Signature} for the named algorithm.
	 *
	 * Accepted names: {@code "Ed25519"}, {@code "ED25519"}.
	 *
	 * @param algorithm The algorithm name.
	 * @return A new {@code Signature} instance.
	 * @throws NoSuchAlgorithmException if the algorithm is not recognised.
	 */
	[[nodiscard]]
	static Signature instance(StringView algorithm) throws (NoSuchAlgorithmException) {
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
	 * @brief Initialises this engine for signing with the given private key.
	 *
	 * Clears any previously accumulated message data.
	 *
	 * @param key The private key to sign with.
	 * @throws InvalidKeyException if the key length does not match the algorithm.
	 */
	void init_sign(const PrivateKey& key) throws (InvalidKeyException) {
		if (key.encoded().size() != ED25519_PRIVATE_KEY_BYTES) {
			throw InvalidKeyException("Invalid Ed25519 private key length");
		}
		signing_key = key;
		verification_key = {};
		message_buffer.clear();
	}

	/**
	 * @brief Initialises this engine for verification with the given public key.
	 *
	 * Clears any previously accumulated message data.
	 *
	 * @param key The public key to verify against.
	 * @throws InvalidKeyException if the key length does not match the algorithm.
	 */
	void init_verify(const PublicKey& key) throws (InvalidKeyException) {
		if (key.encoded().size() != ED25519_PUBLIC_KEY_BYTES) {
			throw InvalidKeyException("Invalid Ed25519 public key length");
		}
		verification_key = key;
		signing_key = {};
		message_buffer.clear();
	}

	/**
	 * @brief Feeds message bytes into the engine without finalising.
	 *
	 * May be called multiple times before {@code sign()} or {@code verify()}.
	 *
	 * @param data The message bytes to add.
	 */
	void update(Span<const u8> data) {
		message_buffer.insert(message_buffer.end(), data.begin(), data.end());
	}

	/**
	 * @brief Signs all updated data and resets internal state.
	 *
	 * @return The detached signature bytes.
	 * @throws NoSuchAlgorithmException if the algorithm is not supported.
	 * @throws InvalidKeyException if {@code init_sign()} was not called.
	 * @throws SignatureException if the underlying signing operation fails.
	 */
	[[nodiscard]]
	ByteBuffer sign() throws (NoSuchAlgorithmException, InvalidKeyException, SignatureException) {
		ensure_sodium_initialized();

		if (algorithm != Algorithm::ED25519) {
			throw NoSuchAlgorithmException("Unsupported signature algorithm");
		}
		if (signing_key.empty()) {
			throw InvalidKeyException("Signature object is not initialised for signing");
		}

		ByteBuffer signature(crypto_sign_BYTES, 0);
		unsigned long long signature_length = 0; // must be unsigned long long (cannot be u64) due to libsodium API
		const i32 rc = crypto_sign_detached(
			signature.data(),
			&signature_length,
			message_buffer.data(),
			static_cast<u64>(message_buffer.size()),
			signing_key.encoded().data()
		);
		message_buffer.clear();
		if (rc != 0) {
			throw SignatureException("Failed to generate signature");
		}
		signature.resize(static_cast<usize>(signature_length));
		return signature;
	}

	/**
	 * @brief Verifies {@code signature} against all updated data and resets internal state.
	 *
	 * @param signature The detached signature to check.
	 * @return {@code true} if the signature is valid, {@code false} otherwise.
	 * @throws NoSuchAlgorithmException if the algorithm is not supported.
	 * @throws InvalidKeyException if {@code init_verify()} was not called, or the signature length is wrong.
	 */
	[[nodiscard]]
	bool verify(Span<const u8> signature) throws (NoSuchAlgorithmException, InvalidKeyException) {
		ensure_sodium_initialized();

		if (algorithm != Algorithm::ED25519) {
			throw NoSuchAlgorithmException("Unsupported signature algorithm");
		}
		if (verification_key.empty()) {
			throw InvalidKeyException("Signature object is not initialised for verification");
		}
		if (signature.size() != ED25519_SIGNATURE_BYTES) {
			throw InvalidKeyException("Invalid Ed25519 signature length");
		}

		const bool ok = crypto_sign_verify_detached(
			signature.data(),
			message_buffer.data(),
			static_cast<u64>(message_buffer.size()),
			verification_key.encoded().data()
		) == 0;
		message_buffer.clear();
		return ok;
	}
};

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
	Algorithm algorithm; ///< The selected symmetric algorithm.
	mutable SecureRandom rng; ///< Entropy source for key material.

protected:
	explicit KeyGenerator(Algorithm algorithm):
		algorithm{algorithm} {}

public:
	/**
	 * @brief Returns a {@code KeyGenerator} for the named algorithm.
	 *
	 * Accepted names: {@code "SecretBox"}, {@code "SECRETBOX"},
	 * {@code "XSalsa20-Poly1305"}, {@code "secretbox"}.
	 *
	 * @param algorithm The algorithm name.
	 * @return A new {@code KeyGenerator} instance.
	 * @throws NoSuchAlgorithmException if the algorithm is not recognised.
	 */
	[[nodiscard]]
	static KeyGenerator instance(StringView algorithm) throws (NoSuchAlgorithmException) {
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
	SecretKey generate_key() const throws (NoSuchAlgorithmException) {
		if (algorithm != Algorithm::SECRETBOX) {
			throw NoSuchAlgorithmException("Unsupported key generation algorithm");
		}

		ByteBuffer key = rng.next_bytes(SECRETBOX_KEY_BYTES);
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
	String transformation; ///< The transformation string used to construct this cipher.
	CipherMode mode; ///< Current operating mode (set by init()).
	SecretKey key; ///< The key to encrypt/decrypt with (set by init()).
	bool initialized; ///< Whether init() has been called successfully.
	mutable SecureRandom rng; ///< Entropy source for nonce generation.

protected:
	explicit Cipher(StringView transformation):
		transformation{transformation}, mode{CipherMode::ENCRYPT_MODE}, key{}, initialized{false} {}

public:
	/**
	 * @brief Returns a {@code Cipher} for the named transformation.
	 *
	 * Accepted names: {@code "SecretBox"}, {@code "XSalsa20-Poly1305"},
	 * {@code "SecretBox/None/NoPadding"}, {@code "secretbox"}.
	 *
	 * @param transformation The transformation name.
	 * @return A new, uninitialised {@code Cipher} instance.
	 * @throws NoSuchAlgorithmException if the transformation is not recognised.
	 */
	[[nodiscard]]
	static Cipher instance(StringView transformation) throws (NoSuchAlgorithmException) {
		if (!is_algorithm(
				transformation,
				Array<StringView, 4>{"SecretBox", "XSalsa20-Poly1305", "SecretBox/None/NoPadding", "secretbox"}
			)) {
			throw NoSuchAlgorithmException("Unsupported cipher transformation");
		}
		return Cipher(transformation);
	}

	/**
	 * @brief Initialises the cipher for encryption or decryption with the given key.
	 * @param mode The operating mode.
	 * @param key The secret key to use; must be non-empty and the correct size.
	 * @throws InvalidKeyException if the key is empty or the wrong size.
	 */
	void init(CipherMode mode, const SecretKey& key) throws (InvalidKeyException) {
		if (key.empty()) {
			throw InvalidKeyException("Cipher key cannot be empty");
		}
		if (key.encoded().size() != SECRETBOX_KEY_BYTES) {
			throw InvalidKeyException("Invalid SecretBox key size");
		}

		this->mode = mode;
		this->key = key;
		initialized = true;
	}

	/**
	 * @brief Encrypts or decrypts {@code input} in a single step and returns the result.
	 *
	 * In {@code ENCRYPT_MODE}, prepends a random nonce; in {@code DECRYPT_MODE},
	 * strips the nonce and verifies the authentication tag before returning plaintext.
	 *
	 * @param input The plaintext (encrypt) or nonce‖MAC‖ciphertext (decrypt).
	 * @return The ciphertext (encrypt) or plaintext (decrypt).
	 * @throws AEADBadTagException if the authentication tag is invalid (decrypt only).
	 * @throws InvalidArgumentException if the ciphertext is too short (decrypt only).
	 * @throws IllegalStateException if {@code init()} has not been called.
	 */
	[[nodiscard]]
	ByteBuffer do_final(Span<const u8> input) const throws (AEADBadTagException, InvalidArgumentException, IllegalStateException) {
		ensure_sodium_initialized();

		if (!initialized) {
			throw IllegalStateException("Cipher is not initialised");
		}

		const Span<const u8> key_material = key.encoded();

		if (mode == CipherMode::ENCRYPT_MODE) {
			ByteBuffer nonce(SECRETBOX_NONCE_BYTES, 0);
			rng.next_bytes(Span<u8>(nonce.data(), nonce.size()));

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
