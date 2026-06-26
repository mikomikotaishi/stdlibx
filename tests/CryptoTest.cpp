import stdx;

#ifdef STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY
using stdx::collections::Vector;
using stdx::crypto::AEADBadTagException;
using stdx::crypto::Cipher;
using stdx::crypto::CipherMode;
using stdx::crypto::InvalidKeyException;
using stdx::crypto::KeyGenerator;
using stdx::crypto::KeyPair;
using stdx::crypto::KeyPairGenerator;
using stdx::crypto::MessageDigest;
using stdx::crypto::MessageDigestAlgorithm;
using stdx::crypto::NoSuchAlgorithmException;
using stdx::crypto::SecretKey;
using stdx::crypto::SecureRandom;
using stdx::crypto::Signature;
using stdx::io::ByteBuffer;
using stdx::math::BigInteger;

using namespace stdx::test;
#endif

#ifdef __GNUC__
using namespace stdx::core;
#endif

#ifdef STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY
/**
 * @brief Convert a span of bytes to a hexadecimal string representation.
 * @param bytes The span of bytes to convert.
 * @return A string containing the hexadecimal representation of the input bytes.
 */
[[nodiscard]]
String to_hex(Span<const u8> bytes) {
    static constexpr StringView DIGITS = "0123456789abcdef";
    String out;
    out.reserve(bytes.size() * 2);
    for (u8 b: bytes) {
        out += DIGITS[b >> 4];
        out += DIGITS[b & 0x0F];
    }
    return out;
}

[[nodiscard]]
Span<const u8> bytes_of(const ByteBuffer& buffer) {
    return Span<const u8>(buffer.data(), buffer.size());
}

/**
 * @brief Reinterpret a string's chars as raw bytes (char and u8 may alias).
 * @param s The string view to convert to bytes.
 * @return A span of bytes representing the input string's data.
 */
[[nodiscard]]
Span<const u8> span_of(StringView s) {
    return Span<const u8>(reinterpret_cast<const u8*>(s.data()), s.size());
}

/**
 * @brief Tests for the SecureRandom class, which provides cryptographically secure random number generation.
 */
void test_secure_random() {
    SecureRandom rng;

    ByteBuffer a = rng.next_bytes(32);
    expect_eq(a.size(), 32uz, "next_bytes(32) returns 32 bytes");
    expect(to_hex(bytes_of(a)) != String(64, '0'), "random bytes are not all zero");

    ByteBuffer b = rng.next_bytes(32);
    expect(to_hex(bytes_of(a)) != to_hex(bytes_of(b)), "two random draws differ");

    bool all_in_range = true;
    for (i32 i = 0; i < 1000; ++i) {
        const i32 v = rng.next<i32>(0, 100);
        if (v < 0 || v >= 100) {
            all_in_range = false;
            break;
        }
    }
    expect(all_in_range, "next(0, 100) stays in range over 1000 draws");

    const f64 u = rng.next_unit<f64>();
    expect(u >= 0.0 && u < 1.0, "next_unit is in [0, 1)");

    expect_throws<InvalidArgumentException>(
        [&] -> void { (void)rng.next<i32>(5, 5); }, "next with min == max throws"
    );
}

/**
 * @brief Tests for the MessageDigest class, which provides cryptographic hash functions such as SHA-256 and BLAKE2b.
 */
void test_message_digest() {
    // FIPS 180-4 SHA-256 test vectors.
    ByteBuffer sha_abc = MessageDigest::digest(MessageDigestAlgorithm::SHA_256, span_of("abc"));
    expect_eq(
        to_hex(bytes_of(sha_abc)),
        "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",
        "SHA-256(\"abc\")"
    );
    ByteBuffer sha_empty = MessageDigest::digest(MessageDigestAlgorithm::SHA_256, span_of(""));
    expect_eq(
        to_hex(bytes_of(sha_empty)),
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
        "SHA-256(\"\")"
    );

    // Digest widths.
    expect_eq(MessageDigest::digest(MessageDigestAlgorithm::SHA_256, span_of("x")).size(), 32uz, "SHA-256 is 32 bytes");
    expect_eq(MessageDigest::digest(MessageDigestAlgorithm::SHA_512, span_of("x")).size(), 64uz, "SHA-512 is 64 bytes");
    expect_eq(MessageDigest::digest(MessageDigestAlgorithm::BLAKE2B_256, span_of("x")).size(), 32uz, "BLAKE2b-256 is 32 bytes");

    // The string and enum factories produce the same engine.
    MessageDigest by_name = MessageDigest::instance("SHA-256");
    MessageDigest by_enum = MessageDigest::instance(MessageDigestAlgorithm::SHA_256);
    expect_eq(
        to_hex(bytes_of(by_name.digest(span_of("abc")))),
        to_hex(bytes_of(by_enum.digest(span_of("abc")))),
        "string and enum factories agree"
    );

    // Incremental updates equal the one-shot digest.
    MessageDigest incremental = MessageDigest::instance(MessageDigestAlgorithm::SHA_256);
    incremental.update(span_of("a"));
    incremental.update(span_of("bc"));
    expect_eq(
        to_hex(bytes_of(incremental.digest())),
        to_hex(bytes_of(sha_abc)),
        "update(a) + update(bc) == digest(abc)"
    );

    expect_throws<NoSuchAlgorithmException>(
        [] -> void { (void)MessageDigest::instance("MD5"); }, "an unknown algorithm throws"
    );
}

/**
 * @brief Tests for the Signature class, which provides digital signature generation and verification using the Ed25519 algorithm.
 */
void test_signature() {
    KeyPairGenerator kpg = KeyPairGenerator::instance(KeyPairGenerator::Algorithm::ED25519);
    KeyPair pair = kpg.generate_key_pair();

    expect_eq(pair.public_key.encoded().size(), 32uz, "Ed25519 public key is 32 bytes");
    expect_eq(pair.private_key.encoded().size(), 64uz, "Ed25519 private key is 64 bytes");
    expect_eq(pair.public_key.algorithm(), "Ed25519", "the public key reports its algorithm");

    static constexpr StringView MESSAGE = "the quick brown fox";

    Signature signer = Signature::instance("Ed25519");
    signer.init_sign(pair.private_key);
    signer.update(span_of(MESSAGE));
    ByteBuffer signature = signer.sign();
    expect_eq(signature.size(), 64uz, "an Ed25519 signature is 64 bytes");

    Signature verifier = Signature::instance(Signature::Algorithm::ED25519);
    verifier.init_verify(pair.public_key);
    verifier.update(span_of(MESSAGE));
    expect(verifier.verify(bytes_of(signature)), "a valid signature verifies");

    // A different message must not verify against the same signature.
    verifier.init_verify(pair.public_key);
    verifier.update(span_of("the quick brown cat"));
    expect(!verifier.verify(bytes_of(signature)), "a tampered message fails verification");

    // Signing without init_sign() is an error.
    Signature uninitialized = Signature::instance(Signature::Algorithm::ED25519);
    uninitialized.update(span_of(MESSAGE));
    expect_throws<InvalidKeyException>(
        [&] -> void { (void)uninitialized.sign(); }, "sign() without init_sign() throws"
    );
}

/**
 * @brief Tests for the Cipher class, which provides authenticated encryption and decryption using the SecretBox construction.
 */
void test_cipher() {
    KeyGenerator kg = KeyGenerator::instance(KeyGenerator::Algorithm::SECRETBOX);
    SecretKey key = kg.generate_key();
    expect_eq(key.encoded().size(), 32uz, "a SecretBox key is 32 bytes");

    static constexpr StringView PLAINTEXT = "attack at dawn";

    Cipher cipher = Cipher::instance("SecretBox");
    cipher.init(CipherMode::ENCRYPT_MODE, key);
    ByteBuffer ciphertext = cipher.do_final(span_of(PLAINTEXT));
    expect_eq(ciphertext.size(), 24uz + 16uz + PLAINTEXT.size(), "ciphertext is nonce + MAC + plaintext");

    Cipher decipher = Cipher::instance("SecretBox");
    decipher.init(CipherMode::DECRYPT_MODE, key);
    ByteBuffer recovered = decipher.do_final(bytes_of(ciphertext));
    expect_eq(to_hex(bytes_of(recovered)), to_hex(span_of(PLAINTEXT)), "decryption recovers the plaintext");

    // A fresh nonce per encryption means identical plaintext yields different ciphertext.
    cipher.init(CipherMode::ENCRYPT_MODE, key);
    ByteBuffer ciphertext2 = cipher.do_final(span_of(PLAINTEXT));
    expect(to_hex(bytes_of(ciphertext)) != to_hex(bytes_of(ciphertext2)), "each encryption uses a fresh nonce");

    // Decrypting with the wrong key fails the authentication tag.
    SecretKey other = kg.generate_key();
    Cipher wrong = Cipher::instance("SecretBox");
    wrong.init(CipherMode::DECRYPT_MODE, other);
    expect_throws<AEADBadTagException>(
        [&] -> void { (void)wrong.do_final(bytes_of(ciphertext)); },
        "decrypting with the wrong key fails the tag check"
    );

    // do_final() before init() is an error.
    Cipher uninitialized = Cipher::instance("SecretBox");
    expect_throws<IllegalStateException>(
        [&] -> void { (void)uninitialized.do_final(span_of(PLAINTEXT)); }, "do_final() before init() throws"
    );

    // A ciphertext shorter than nonce + MAC is rejected.
    expect_throws<InvalidArgumentException>(
        [&] -> void {
            ByteBuffer tiny(8, 0);
            (void)decipher.do_final(bytes_of(tiny));
        },
        "a too-short ciphertext throws"
    );
}

/**
 * @brief Tests for the SecretKey class, which represents cryptographic keys and their associated metadata.
 */
void test_keys() {
    static constexpr Array<u8, 4> RAW = {0xDE, 0xAD, 0xBE, 0xEF};
    SecretKey key("SecretBox", Span<const u8>(RAW.data(), RAW.size()));

    expect_eq(key.algorithm(), "SecretBox", "the key reports its algorithm");
    expect_eq(key.format(), "RAW", "the key format is RAW");
    expect_eq(key.encoded().size(), 4uz, "the key encodes its bytes");
    expect(!key.empty(), "a populated key is not empty");
    expect(SecretKey().empty(), "a default-constructed key is empty");
}

/**
 * @brief Demonstrates the safe BigInteger <-> crypto adapter.
 *
 * Fixed-size byte outputs (digests, CSPRNG bytes) reinterpreted
 * as arbitrary-precision integers. The crypto primitives themselves
 * stay on libsodium's constant-time paths.
 */
void test_bigint_integration() {
    // A SHA-256 digest is 32 bytes; read it as a positive 256-bit integer.
    ByteBuffer digest = MessageDigest::digest(MessageDigestAlgorithm::SHA_256, span_of("abc"));
    Vector<u8> digest_bytes(digest.data(), digest.data() + digest.size());
    BigInteger value(1, digest_bytes); // signum = 1, big-endian magnitude

    expect_eq(value.signum(), 1, "a non-zero digest reads as a positive integer");
    expect(value.bit_length() <= 256, "a 256-bit digest needs at most 256 bits");

    // The integer round-trips through its two's-complement byte form.
    BigInteger round_trip(value.to_byte_array());
    expect(round_trip == value, "BigInteger survives a to_byte_array() round trip");

    // ...and equals the digest parsed straight from its hex string.
    BigInteger from_hex("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad", 16);
    expect(value == from_hex, "the digest-as-integer equals parsing its hex form");

    // A CSPRNG can seed a random big integer of a chosen width.
    SecureRandom rng;
    ByteBuffer random_bytes = rng.next_bytes(16);
    Vector<u8> magnitude(random_bytes.data(), random_bytes.data() + random_bytes.size());
    BigInteger random_value(1, magnitude);
    expect(random_value.signum() >= 0, "a CSPRNG-seeded big integer is non-negative");
    expect(random_value.bit_length() <= 128, "16 random bytes yield at most 128 bits");
}
#endif

int main(int argc, char* argv[]) {
    #ifdef STDLIBX_EXTENSIONS_COMPILE_CRYPTO_LIBSODIUM_LIBRARY
    return run(argc, argv, {
        {"crypto.secure_random", test_secure_random},
        {"crypto.message_digest", test_message_digest},
        {"crypto.signature", test_signature},
        {"crypto.cipher", test_cipher},
        {"crypto.keys", test_keys},
        {"crypto.bigint_integration", test_bigint_integration},
    });
    #else
    System::out.println("[test] Crypto disabled (libsodium not compiled in).");
    return System::EXIT_SUCCESS;
    #endif
}
